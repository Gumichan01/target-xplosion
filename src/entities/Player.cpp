
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "Player.hpp"
#include "PlayerVisitor.hpp"
#include "BasicMissile.hpp"
#include "Bomb.hpp"
#include "Rocket.hpp"
#include "Laser.hpp"

#include "../level/Level.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../game/engine/AudioHandler.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/engine/Hud.hpp"
#include "../game/Scoring.hpp"
#include "../asset/TX_Asset.hpp"
#include "../resources/ResourceManager.hpp"
#include "../resources/WinID.hpp"

#include <Lunatix/Random.hpp>
#include <Lunatix/Graphics.hpp>
#include <Lunatix/Physics.hpp>
#include <Lunatix/Time.hpp>

using namespace AudioHandler;
using namespace MissileInfo;
using namespace FloatBox;

lx::Physics::FloatPosition Player::last_position = { { 0.0f }, { 0.0f } };

namespace
{
const unsigned int PLAYER_RADIUS = 8;
const unsigned int NB_ROCKET_ADD = 10;
const unsigned int NB_BOMB_ADD = 2;
const unsigned int NBMAX_BOMB = 5;
const unsigned int NBMAX_ROCKET = 50;

const int PLAYER_MISSILE_SPEED = 32;
const unsigned long HEALTH_SCORE = 10000000;
const unsigned long ROCKET_SCORE = 1000000;
const unsigned long BOMB_SCORE = 5000000;

const unsigned int BULLET_SHOT_ID = 0;
const unsigned int ROCKET_SHOT_ID = 1;
const unsigned int BOMB_SHOT_ID = 2;
const unsigned int LASER_SHOT_ID = 3;
const unsigned int HITBOX_SPRITE_ID = 8;

const unsigned int PLAYER_EXPLOSION_ID = 7;
const unsigned int PLAYER_EXPLOSION_DELAY = 620;

const float SHOT_SIDE_OFFSET = 3.5f;
const float SHOT_SIDE_OFFSET_SLOW = 1.75f;

const short HIT_SOFT     = 1;
const short HIT_NORMAL   = 2;
const short HIT_HARD     = 3;
const short HIT_CRITICAL = 4;

const unsigned long BONUS_SCORE = 50;
const int PLAYER_BULLET_W = 24;
const int PLAYER_BULLET_H = 24;

const unsigned int SHIELD_TIME = 10000;
const unsigned int HITS_UNDER_SHIELD = 16;
const unsigned int PLAYER_INVICIBILITY_DELAY = 2000;

constexpr Float DEATH_VEL = fbox( 16.8f );

double setAngle( const bool is_dying, const lx::Physics::Vector2D& sp )
{
    constexpr double ran = BulletPattern::PI / 24.0;
    constexpr double iran = -ran;
    return !is_dying ?
           ( sp.vy != FNIL ?
             ( sp.vy > FNIL ?
               iran : ran ) : 0.0 ) : 0.0;
}

void bonus()
{
    Score * sc = Engine::getInstance()->getScore();
    unsigned int n = static_cast<int>( sc->getCombo() );
    sc->bonusScore( BONUS_SCORE * n );
}

lx::Graphics::AnimatedSprite * getExplosionSprite()
{
    using namespace lx::Graphics;
    const TX_Asset * const a = TX_Asset::getInstance();
    const std::string& str   = a->getExplosionSpriteFile( PLAYER_EXPLOSION_ID );
    const TX_Anima * const anima = a->getExplosionAnimation( PLAYER_EXPLOSION_ID );
    lx::Win::Window& w   = lx::Win::getWindowManager().getWindow( WinID::getWinID() );

    return BufferedImage( str ).generateAnimatedSprite( w, anima->v, anima->delay, false );
}

inline unsigned int random100()
{
    return lx::Random::xrand(0U, 100U);
}

}


Player::Player( unsigned int hp, unsigned int att, unsigned int sh,
                unsigned int critic, lx::Graphics::Sprite * image,
                lx::Graphics::ImgRect& rect, lx::Physics::Vector2D& sp )
    : Character( hp, att, sh, image, rect, sp ), GAME_WLIM( Engine::getMaxXlim() ),
      GAME_HLIM( Engine::getMaxYlim() ), critical_rate( critic ), nb_bomb( 3 ),
      nb_rocket( 10 ), has_shield( false ), ptimer(), shtimer(), latimer(),
      invtimer(), extimer(), laser_activated( false ),
      hit_count( HITS_UNDER_SHIELD ), deaths( 0 ), slow_mode( false ),
      display( new PlayerHUD( *this ) ),
      sprite_hitbox( ResourceManager::getInstance()->getMenuResource( HITBOX_SPRITE_ID ) ),
      sprite_explosion( getExplosionSprite() )
{
    initHitboxRadius();
    HudHandler::getInstance().addHUD( *display );
    invtimer.start();
    ptimer.start();

    if ( Level::getLevelNum() < Level::BOMB_LEVEL_MIN )
        nb_bomb = 0;
}


Player::~Player()
{
    HudHandler::getInstance().removeHUD( *display );

    delete sprite_explosion;
    delete display;
    sprite_explosion = nullptr;
    display = nullptr;
}


// A missile can get the last position of the player
void Player::accept( PlayerVisitor& pv ) noexcept
{
    pv.visit( last_position );
}


void Player::initHitboxRadius() noexcept
{
    const Float PLAYER_RADIUSF = fbox<decltype( PLAYER_RADIUS )>( PLAYER_RADIUS );
    circle_box.radius = PLAYER_RADIUS;
    circle_box.center.y += PLAYER_RADIUSF;
}

void Player::updateStatus( unsigned int prev_health ) noexcept
{
    const unsigned int HEALTH_25 = max_health_point / 4U;
    const unsigned int HEALTH_50 = max_health_point / 2U;
    const unsigned int HEALTH_75 = max_health_point - max_health_point / 4U;

    if ( health_point == 0U )
        die();

    else
    {
        if ( health_point <= HEALTH_25 )
            AudioHandler::AudioHDL::getInstance()->playHit( HIT_CRITICAL );

        else if ( health_point <= HEALTH_50 )
            AudioHandler::AudioHDL::getInstance()->playHit( HIT_HARD );

        else if ( health_point < HEALTH_75 )
            AudioHandler::AudioHDL::getInstance()->playHit( HIT_NORMAL );

        else
            AudioHandler::AudioHDL::getInstance()->playHit( HIT_SOFT );

        if ( health_point <= HEALTH_25 && prev_health > HEALTH_25 )
            AudioHandler::AudioHDL::getInstance()->playAlert( true );

        else if ( health_point <= HEALTH_50 && prev_health > HEALTH_50 )
            AudioHandler::AudioHDL::getInstance()->playAlert();
    }
}

void Player::receiveDamages( unsigned int attacks ) noexcept
{
    const unsigned int prev_health = health_point;

    // Take less damages if the shied is activated
    if ( has_shield )
    {
        attacks /= 4;
        hit_count--;

        if ( hit_count == 0 )
            setShield( false );
    }

    Character::receiveDamages( attacks );
    display->update();
    updateStatus( prev_health );
}


void Player::checkLaserShot() noexcept
{
    if ( laser_activated )
    {
        if ( latimer.getTicks() < LASER_LIFETIME )
        {
            laserShot();
            EntityHandler::getInstance().bulletCancel();
        }
        else
            laser_activated = false;
    }
}


// It only concerns the double shots and the large shot
void Player::normalShot() noexcept
{
    if ( isLaserActivated() )
        return;

    const int offset_y1 = imgbox.w / 4;
    const int offset_y2 = imgbox.h - offset_y1;
    const int offset_x  = imgbox.w - PLAYER_BULLET_W;
    const float b_offset = slow_mode ? SHOT_SIDE_OFFSET_SLOW : SHOT_SIDE_OFFSET;
    const float vy[] = { -b_offset, b_offset };
    const int SHOTS = 4;

    lx::Graphics::ImgRect pos[SHOTS] =
    {
        {
            imgbox.p.x + offset_x, imgbox.p.y + offset_y1,
            MISSILE_WIDTH, MISSILE_HEIGHT
        },
        {
            imgbox.p.x + offset_x, imgbox.p.y + offset_y2,
            MISSILE_WIDTH, MISSILE_HEIGHT
        },
        {
            imgbox.p.x + PLAYER_BULLET_W,
            imgbox.p.y + ( imgbox.w - PLAYER_BULLET_H ) / 2 - 1,
            PLAYER_BULLET_W, PLAYER_BULLET_H
        },
        { 0, 0, 0, 0 }
    };

    pos[3] = pos[2];

    lx::Physics::Vector2D pvel[SHOTS] =
    {
        lx::Physics::Vector2D{ PLAYER_MISSILE_SPEED, FNIL  },
        lx::Physics::Vector2D{ PLAYER_MISSILE_SPEED, FNIL  },
        lx::Physics::Vector2D{ PLAYER_MISSILE_SPEED, vy[0] },
        lx::Physics::Vector2D{ PLAYER_MISSILE_SPEED, vy[1] }

    };

    unsigned int crit = ( random100() <= critical_rate ? critical_rate : 0 );

    // Basic shot sound
    AudioHandler::AudioHDL::getInstance()->playShot( lx::Graphics::toPixelPosition( phybox.p ) );

    const ResourceManager * rc = ResourceManager::getInstance();
    lx::Graphics::Sprite * tmp = rc->getResource( RC_MISSILE, BULLET_SHOT_ID );
    EntityHandler& hdl = EntityHandler::getInstance();

    for ( int i = 0; i < SHOTS; ++i )
    {
        hdl.pushPlayerMissile( *( new BasicMissile( attack_val + crit,
                                  tmp, pos[i], pvel[i] ) ) );
    }

    display->update();
}


void Player::rocketShot() noexcept
{
    if ( nb_rocket > 0 && !isLaserActivated() )
    {
        nb_rocket--;

        lx::Graphics::ImgRect mpos =
        {
            imgbox.p.x + ( imgbox.w / 2 ),
            imgbox.p.y + ( ( imgbox.h - ROCKET_HEIGHT ) / 2 ),
            ROCKET_WIDTH,
            ROCKET_HEIGHT
        };

        lx::Physics::Vector2D vel = lx::Physics::Vector2D{ROCKET_SPEED, FNIL};
        unsigned int crit = ( random100() <= critical_rate ? critical_rate : 0 );

        const ResourceManager * rc = ResourceManager::getInstance();
        lx::Graphics::Sprite * tmp = rc->getResource( RC_MISSILE, ROCKET_SHOT_ID );

        AudioHandler::AudioHDL::getInstance()->playRocketShot();
        EntityHandler& hdl = EntityHandler::getInstance();

        hdl.pushPlayerMissile( *( new PlayerRocket( attack_val + crit, tmp, mpos, vel ) ) );
        display->update();
    }
}


void Player::bombShot() noexcept
{
    if ( nb_bomb > 0 && !isLaserActivated() )
    {
        nb_bomb--;
        lx::Physics::Vector2D vel{BOMB_SPEED, FNIL};

        const ResourceManager * rc = ResourceManager::getInstance();
        lx::Graphics::Sprite * tmp = rc->getResource( RC_MISSILE, BOMB_SHOT_ID );
        unsigned int crit = ( random100() <= critical_rate ? critical_rate : 0 );

        lx::Graphics::ImgRect mpos =
        {
            imgbox.p.x + ( imgbox.w / 2 ),
            imgbox.p.y + ( ( imgbox.h - BOMB_HEIGHT ) / 2 ),
            BOMB_WIDTH,
            BOMB_HEIGHT
        };

        EntityHandler& hdl = EntityHandler::getInstance();
        hdl.pushPlayerMissile( *( new Bomb( attack_val + crit, tmp, mpos, vel ) ) );
        display->update();
    }
}


void Player::laserShot() noexcept
{
    const ResourceManager * const rc = ResourceManager::getInstance();
    lx::Graphics::Sprite * tmp = rc->getResource( RC_MISSILE, LASER_SHOT_ID );
    unsigned int crit = ( random100() <= critical_rate ? critical_rate : 0 );

    lx::Graphics::ImgRect mpos;
    mpos.p.x = imgbox.p.x + ( imgbox.w - ( imgbox.w / 4 ) );
    mpos.p.y = imgbox.p.y + ( ( imgbox.h - LASER_HEIGHT ) / 2 );
    mpos.w = GAME_WLIM;
    mpos.h = LASER_HEIGHT;

    lx::Physics::Vector2D vel{ 0.0f, FNIL };
    EntityHandler& hdl = EntityHandler::getInstance();
    hdl.pushPlayerMissile( *( new Laser( attack_val + crit, tmp, mpos, vel ) ) );
    display->update();
}


void Player::boom() noexcept
{
    AudioHandler::AudioHDL::getInstance()->playPlayerExplosion();
}


void Player::move() noexcept
{
    const float min_xlim = Engine::getMinXlim();
    const float min_ylim = Engine::getMinYlim();

    if ( dying )
    {
        die();
        slow_mode = false;
        speed /= DEATH_VEL;
    }

    // Update the position and the circle_box on X
    phybox.p.x += speed.vx;
    circle_box.center.x += speed.vx;

    // Left or Right
    if ( ( phybox.p.x <= min_xlim ) || ( ( phybox.p.x + imgbox.w ) > GAME_WLIM ) )
    {
        phybox.p.x -= speed.vx;
        circle_box.center.x -= speed.vx;
    }

    // Do the same thing on Y
    phybox.p.y += speed.vy;
    circle_box.center.y += speed.vy;

    // Down or Up
    if ( ( phybox.p.y <= min_ylim ) || ( ( phybox.p.y + imgbox.h ) > GAME_HLIM ) )
    {
        phybox.p.y -= speed.vy;
        circle_box.center.y -= speed.vy;
    }

    // I need to store the position
    // so the enemies know where the player is
    last_position = circle_box.center;

    // The shield may be still running
    // So I need to check if I must disable it
    if ( has_shield )
    {
        if ( shtimer.getTicks() > SHIELD_TIME )
            setShield( false );
    }
}

void Player::draw() noexcept
{
    if ( !isDead() )
    {
        double angle = setAngle( isDying(), speed );
        imgbox.p = lx::Graphics::toPixelPosition( phybox.p );

        // If the player gets hit, I need to display the sprite associated to this
        // situation during a moment.
        // Otherwise, I can just display the normal sprite
        if ( hit && !dying )
        {
            if ( ptimer.getTicks() > HIT_DELAY )
            {
                hit = false;
                ptimer.lap();
            }

            hit_sprite->draw( imgbox, angle );
        }
        else
            graphic->draw( imgbox, angle );

        // In slow mode (when the player keep the button associated to this action)
        // The game must display the little hitbox.
        if ( slow_mode )
        {
            const int RAD2 = static_cast<int>( circle_box.radius ) * 2;

            lx::Graphics::ImgCoord C = lx::Graphics::toPixelPosition( circle_box.center );
            C.x -= static_cast<int>( circle_box.radius );
            C.y -= static_cast<int>( circle_box.radius );

            lx::Graphics::ImgRect rect = { C, RAD2, RAD2 };
            sprite_hitbox->draw( rect, angle );
        }
    }
}

void Player::die() noexcept
{
    if ( invtimer.getTicks() < PLAYER_INVICIBILITY_DELAY )
        return;

    // In order to be dead, the player reach two states
    // 1. dying (during PLAYER_EXPLOSION_DELAY ms)
    // 2. dead
    if ( !dying )
    {
        // 1. dying
        deaths++;
        dying = true;
        health_point = 0;
        speed /= DEATH_VEL;

        // Each death rest the combo value (rule of the game)
        Engine::getInstance()->getScore()->resetCombo();
        display->update();

        AudioHandler::AudioHDL::getInstance()->stopAlert();
        sprite_explosion->resetAnimation();
        graphic = sprite_explosion;
        extimer.lap();
        boom();
    }
    else    // 2. dead
    {
        if ( extimer.getTicks() > PLAYER_EXPLOSION_DELAY )
        {
            dying = false;
            still_alive = false;
        }
    }
}


void Player::status() noexcept
{
    static unsigned int death_start = 0;
    const unsigned int DELAY_TO_REBORN = 2000;

    if ( !isDead() )
    {
        move();
        checkLaserShot();
        death_start = lx::Time::getTicks();
    }
    else
    {
        if ( ( lx::Time::getTicks() - death_start ) > DELAY_TO_REBORN )
            reborn();
    }
}

void Player::reborn() noexcept
{
    setShield( true );
    health_point = max_health_point;
    still_alive = true;

    phybox.p.x = fbox<int>( imgbox.w * 2 );
    phybox.p.y = fbox<int>( ( GAME_HLIM - imgbox.h ) / 2 );

    slow_mode = false;
    imgbox.p = lx::Graphics::toPixelPosition( phybox.p );
    speed *= FNIL;

    const Float POINT_XOFFSET = fbox<int>( phybox.w / 2 );
    const Float POINT_YOFFSET = fbox<int>( phybox.h / 2 );
    circle_box.center.x = phybox.p.x + POINT_XOFFSET;
    circle_box.center.y = phybox.p.y + POINT_YOFFSET;

    initHitboxRadius();
    display->update();
    EntityHandler::getInstance().bulletCancel();
    invtimer.lap();
}


void Player::collision( Missile * mi ) noexcept
{
    if ( invtimer.getTicks() < PLAYER_INVICIBILITY_DELAY )
        return;

    if ( still_alive && !dying && !mi->isDead() && !mi->explosion() && mi->getX() >= imgbox.p.x )
    {
        if ( collisionCircleBox( circle_box, mi->getHitbox() ) )
        {
            if ( !has_shield )
                Engine::getInstance()->getScore()->resetCombo();

            receiveDamages( mi->hit() );
            mi->die();
        }
    }
}

void Player::collision( Item * item ) noexcept
{
    const unsigned int N = 3;
    const lx::Physics::Circle C{circle_box.center, circle_box.radius * N};

    if ( collisionCircleBox( C, item->box() ) )
    {
        takeBonus( item->getPowerUp() );
        item->die();
    }
}


void Player::takeBonus( ItemType powerUp ) noexcept
{
    switch ( powerUp )
    {
    case ItemType::SCORE:
        bonus();
        break;

    case ItemType::HEALTH:
        heal();
        break;

    case ItemType::SHIELD:
        setShield( true );
        break;

    case ItemType::ROCKET:
        rocket();
        break;

    case ItemType::BOMB:
        bomb();
        break;

    case ItemType::LASER:
        laser();
        break;

    default:
        break;
    }
}


void Player::rocket() noexcept
{
    if ( ( nb_rocket + NB_ROCKET_ADD ) <= NBMAX_ROCKET )
        nb_rocket += NB_ROCKET_ADD;

    else
    {
        // Not using a rocket can be dangerous.
        // So, taking this risk must be awarded by giving extra points.
        unsigned long score = ( nb_rocket + NB_ROCKET_ADD - NBMAX_ROCKET ) * ROCKET_SCORE;
        Engine::getInstance()->getScore()->notify( score );
        nb_rocket = NBMAX_ROCKET;
    }

    AudioHDL::getInstance()->playVoiceRocket();
    display->update();
}

void Player::bomb() noexcept
{
    if ( ( nb_bomb + NB_BOMB_ADD ) <= NBMAX_BOMB )
        nb_bomb += NB_BOMB_ADD;

    else
    {
        // Not using a bomb can be very dangerous.
        // So, taking this risk must be awarded by giving extra points.
        unsigned long score = ( nb_bomb + NB_BOMB_ADD - NBMAX_BOMB ) * BOMB_SCORE;
        Engine::getInstance()->getScore()->notify( score );
        nb_bomb = NBMAX_BOMB;
    }

    AudioHDL::getInstance()->playVoicePulse();
    display->update();
}


void Player::laser() noexcept
{
    AudioHandler::AudioHDL::getInstance()->playLaserShot();
    laser_activated = true;
    latimer.lap();
}


void Player::heal() noexcept
{
    unsigned int heal_point;
    const unsigned int HEALTH_10 = max_health_point / 10;
    const unsigned int HEALTH_25 = max_health_point / 4;
    const unsigned int HEALTH_50 = max_health_point / 2;
    const unsigned int FIVE = 5;
    const unsigned int FOUR = 4;
    const unsigned int TWO  = 2;

    // Calculate the heal_point
    if ( health_point < ( HEALTH_10 ) )
    {
        heal_point = health_point * FIVE;
    }
    else if ( health_point < ( HEALTH_25 ) )
    {
        heal_point = health_point * TWO;
    }
    else if ( health_point < ( HEALTH_50 ) )
    {
        heal_point = health_point;
    }
    else
        heal_point = health_point / FOUR;

    // Calculate the resulting health_point
    if ( health_point == max_health_point )
    {
        Engine::getInstance()->getScore()->notify( HEALTH_SCORE );
    }
    else if ( ( health_point + heal_point ) > max_health_point )
    {
        health_point = max_health_point;
    }
    else
        health_point += heal_point;

    AudioHandler::AudioHDL * audiohdl = AudioHandler::AudioHDL::getInstance();

    if ( health_point < HEALTH_25 )
    {
        audiohdl->playAlert( true );
    }
    else if ( health_point > HEALTH_25 && health_point < HEALTH_50 )
    {
        audiohdl->playAlert();
    }
    else
    {
        audiohdl->stopAlert();
    }

    display->update();
}


unsigned int Player::getBomb() const noexcept
{
    return nb_bomb;
}


unsigned int Player::getRocket() const noexcept
{
    return nb_rocket;
}

bool Player::isLaserActivated() const noexcept
{
    return laser_activated;
}

unsigned int Player::nb_death() const noexcept
{
    return deaths;
}


void Player::setShield( bool sh ) noexcept
{
    const ResourceManager * rc = ResourceManager::getInstance();

    if ( sh )
    {
        has_shield = true;
        hit_count = HITS_UNDER_SHIELD;
        graphic = rc->getPlayerResource( true );
        shtimer.lap();

        if ( still_alive )
            AudioHDL::getInstance()->playVoiceShield();
    }
    else
    {
        has_shield = false;
        graphic = rc->getPlayerResource();
    }
}

void Player::notifySlow( bool slow ) noexcept
{
    slow_mode = slow;
}
