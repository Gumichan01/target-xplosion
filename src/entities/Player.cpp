
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017  Luxon Jean-Pierre
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
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

#include "../game/engine/AudioHandler.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/engine/Hud.hpp"
#include "../game/Scoring.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>

using namespace AudioHandler;
using namespace LX_Random;
using namespace LX_FileIO;
using namespace LX_Mixer;
using namespace LX_Physics;

LX_Point Player::last_position(0,0);

namespace
{
const unsigned int PLAYER_RADIUS = 8;
const unsigned int NB_ROCKET_ADD = 10;
const unsigned int NB_BOMB_ADD = 5;
const unsigned int NBMAX_BOMB = 15;
const unsigned int NBMAX_ROCKET = 50;

const unsigned long HEALTH_SCORE = 10000000;
const unsigned long ROCKET_SCORE = 1000000;
const unsigned long BOMB_SCORE = 5000000;

const unsigned int BULLET_SHOT_ID = 0;
const unsigned int ROCKET_SHOT_ID = 1;
const unsigned int BOMB_SHOT_ID = 2;
const unsigned int LASER_SHOT_ID = 3;

const unsigned int PLAYER_EXPLOSION_ID = 7;
const unsigned int PLAYER_EXPLOSION_DELAY = 620;

const unsigned long BONUS_SCORE = 50;
const int PLAYER_BULLET_W = 24;
const int PLAYER_BULLET_H = 24;

const unsigned int SHIELD_TIME = 10000;
const unsigned int HITS_UNDER_SHIELD = 16;

const uint32_t PLAYER_INVICIBILITY_DELAY = 2000;

void bonus()
{
    Score *sc = Engine::getInstance()->getScore();
    unsigned int n = static_cast<int>(sc->getCombo());
    sc->bonusScore(BONUS_SCORE*n);
}

LX_Graphics::LX_Sprite * getExplosionSprite()
{
    using namespace LX_Graphics;
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Sprite * tmp = rc->getResource(RC_XPLOSION, PLAYER_EXPLOSION_ID);
    LX_AnimatedSprite * anim_tmp = dynamic_cast<LX_AnimatedSprite*>(tmp);

    if(anim_tmp != nullptr)
        anim_tmp->resetAnimation();

    return anim_tmp;
}

}


Player::Player(unsigned int hp, unsigned int att, unsigned int sh,
               unsigned int critic, LX_Graphics::LX_Sprite *image,
               LX_AABB& rect, LX_Vector2D& sp, int w_limit, int h_limit)
    : Character(hp, att, sh, image, rect, sp), GAME_WLIM(w_limit),
      GAME_HLIM(h_limit), critical_rate(critic), nb_bomb(0), nb_rocket(0),
      has_shield(false), shield_t(0), hit_count(HITS_UNDER_SHIELD), deaths(0),
      laser_activated(false), laser_begin(0), laser_delay(LASER_LIFETIME),
      invincibility_t(LX_Timer::getTicks()),display(nullptr)
{
    initHitboxRadius();
    display = new PlayerHUD(*this);
    Engine::getInstance()->acceptHUD(display);
}


Player::~Player()
{
    delete display;
    display = nullptr;
}


// A missile can get the last position of the player
void Player::accept(PlayerVisitor *pv)
{
    LX_Point p(last_position);
    pv->visit(p);
}


// initialize the hitbox
void Player::initHitboxRadius()
{
    unsigned int rad = PLAYER_RADIUS;
    unsigned int square_rad = rad*rad;

    hitbox.radius = rad;
    hitbox.square_radius = square_rad;
}


void Player::receiveDamages(unsigned int attacks)
{
    // Take less damages if the shied is activated
    if(has_shield == true)
    {
        attacks /= 4;
        hit_count--;

        // Must we remove the shield?
        if(hit_count == 0)
            setShield(false);
    }

    Character::receiveDamages(attacks);
    display->update();

    if(health_point == 0)
        die();
}

// FIRE !!
void Player::fire(MissileType m_type)
{
    MissileType ty;

    if(dying)
        return;

    if(laser_activated)
        ty = MissileType::LASER_TYPE;
    else
        ty = m_type;

    switch(ty)
    {
    case MissileType::LASER_TYPE:
    {
        if((LX_Timer::getTicks() - laser_begin) < laser_delay)
            laserShot();
        else
            laser_activated = false;
    }
    break;

    case MissileType::BOMB_TYPE:
    {
        if(nb_bomb > 0)
        {
            nb_bomb--;
            bombShot();
        }
    }
    break;

    case MissileType::ROCKET_TYPE:
    {
        if(nb_rocket > 0)
        {
            nb_rocket--;
            rocketShot();
        }
    }
    break;

    case MissileType::DOUBLE_MISSILE:
    case MissileType::WAVE_MISSILE:
        specialShot(ty);
        break;

    default:
        specialShot(ty);
        break;
    }

    display->update();
}


void Player::rocketShot()
{
    LX_AABB mpos;
    LX_Vector2D vel = LX_Vector2D(ROCKET_SPEED, 0.0f);
    unsigned int bonus_att = 0;

    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *tmp = rc->getResource(RC_MISSILE, ROCKET_SHOT_ID);

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    mpos.x = position.x + (position.w/2);
    mpos.y = position.y + ( (position.h - ROCKET_HEIGHT)/ 2);
    mpos.w = ROCKET_WIDTH;
    mpos.h = ROCKET_HEIGHT;

    AudioHandler::AudioHDL::getInstance()->playRocketShot();

    g->acceptPlayerMissile(new PlayerRocket(attack_val + bonus_att, tmp, mpos, vel));
}


void Player::bombShot()
{
    LX_AABB mpos;
    unsigned int bonus_att = 0;
    LX_Vector2D vel = LX_Vector2D(BOMB_SPEED, 0.0f);
    Engine *g = Engine::getInstance();

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *tmp = rc->getResource(RC_MISSILE, BOMB_SHOT_ID);

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    mpos.x = position.x + (position.w/2);
    mpos.y = position.y + ( (position.h - BOMB_HEIGHT)/ 2);
    mpos.w = BOMB_WIDTH;
    mpos.h = BOMB_HEIGHT;

    g->acceptPlayerMissile(new Bomb(attack_val + bonus_att, tmp, mpos, vel));
    display->update();
}


void Player::laserShot()
{
    LX_AABB mpos;
    LX_Vector2D vel;
    unsigned int bonus_att = 0;
    Engine *g = Engine::getInstance();

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *tmp = rc->getResource(RC_MISSILE, LASER_SHOT_ID);   /// @note ... ????

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    mpos.x = position.x + (position.w - (position.w / 4));
    mpos.y = position.y + ( (position.h - LASER_HEIGHT)/ 2);
    mpos.w = GAME_WLIM;
    mpos.h = LASER_HEIGHT;

    g->acceptPlayerMissile(new Laser(attack_val + bonus_att, tmp, mpos, vel));
}

// It only concerns the double shots and the large shot
void Player::specialShot(MissileType type)
{
    const int offset_y1 = position.w/4;
    const int offset_y2 = position.h - offset_y1;
    const int offset_x  = position.w - PLAYER_BULLET_W;
    const float vy[] = {-3.0f, 3.0f};
    const int SHOTS = 2;

    LX_AABB pos[SHOTS];
    LX_Vector2D projectile_speed[SHOTS];
    unsigned int bonus_att = 0;

    Engine *cur_game = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();

    if(type == MissileType::DOUBLE_MISSILE)
    {
        pos[0] = {position.x + offset_x, position.y + offset_y1,
                  MISSILE_WIDTH, MISSILE_HEIGHT
                 };
        pos[1] = {position.x + offset_x, position.y + offset_y2,
                  MISSILE_WIDTH, MISSILE_HEIGHT
                 };

        projectile_speed[0] = LX_Vector2D(PLAYER_MISSILE_SPEED, 0.0f);
        projectile_speed[1] = LX_Vector2D(PLAYER_MISSILE_SPEED, 0.0f);
    }
    else
    {
        pos[0] = {position.x + PLAYER_BULLET_W,
                  position.y + (position.w - PLAYER_BULLET_H)/2 -1,
                  PLAYER_BULLET_W, PLAYER_BULLET_H
                 };

        pos[1] = pos[0];
        projectile_speed[0] = LX_Vector2D(PLAYER_MISSILE_SPEED, vy[0]);
        projectile_speed[1] = LX_Vector2D(PLAYER_MISSILE_SPEED, vy[1]);
    }

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    // The basic shot sound
    AudioHandler::AudioHDL::getInstance()->playShot();
    LX_Graphics::LX_Sprite *tmp = rc->getResource(RC_MISSILE, BULLET_SHOT_ID);

    for(int i = 0; i < SHOTS; i++)
    {
        cur_game->acceptPlayerMissile(new BasicMissile(attack_val + bonus_att,
                                      tmp, pos[i], projectile_speed[i]));
    }
}


void Player::boom()
{
    AudioHandler::AudioHDL::getInstance()->playPlayerExplosion();
}


// manage the action of the player (movement and shield)
void Player::move()
{
    const int min_xlim = Engine::getMinXlim();
    const int min_ylim = Engine::getMinYlim();

    if(dying)
    {
        // No movement. Die!
        die();
        return;
    }

    // Update the position and the hitbox on X
    position.x += speed.vx;
    hitbox.center.x += speed.vx;

    // Left or Right
    if((position.x <= min_xlim) || ((position.x + position.w) > GAME_WLIM))
    {
        position.x -= speed.vx;
        hitbox.center.x -= speed.vx;
    }

    // Do the same thing on Y
    position.y += speed.vy;
    hitbox.center.y += speed.vy;

    // Down or Up
    if((position.y <= min_ylim) || ((position.y + position.h) > GAME_HLIM))
    {
        position.y -= speed.vy;
        hitbox.center.y -= speed.vy;
    }

    // Store the updated position of the player
    last_position = hitbox.center;

    // Check the shield
    if(has_shield == true)
    {
        if(LX_Timer::getTicks() - shield_t > SHIELD_TIME)
            setShield(false);
    }
}

void Player::draw()
{
    if(!isDead())
        Character::draw();
}

void Player::die()
{
    static uint32_t t = 0;

    if((LX_Timer::getTicks() - invincibility_t) < PLAYER_INVICIBILITY_DELAY)
        return;

    if(!dying)
    {
        deaths++;
        dying = true;
        health_point = 0;
        speed = LX_Vector2D(0.0f, 0.0f);
        t = LX_Timer::getTicks();

        // Update the HUD
        Engine::getInstance()->getScore()->resetCombo();
        display->update();

        graphic = getExplosionSprite();
        boom();
    }
    else
    {
        if((LX_Timer::getTicks() - t) > PLAYER_EXPLOSION_DELAY)
        {
            dying = false;
            Character::die();
        }
    }
}


void Player::reborn()
{
    setShield(true);
    health_point = max_health_point;
    still_alive = true;
    position.x = position.w * 2;
    position.y = (GAME_HLIM - position.h) / 2;
    speed = {0.0f,0.0f};

    hitbox.center = LX_Point(position.x + (((position.x + position.w) - position.x)/2),
                             position.y + (((position.y + position.h) - position.y)/2));
    initHitboxRadius();
    display->update();
    Engine::getInstance()->bulletCancel();
    invincibility_t = LX_Timer::getTicks();
}


void Player::collision(Missile *mi)
{
    if(still_alive && !dying && mi->getX() >= position.x)
    {
        if(collisionCircleRect(hitbox, *mi->getHitbox()))
        {
            if(!has_shield) Engine::getInstance()->getScore()->resetCombo();
            receiveDamages(mi->hit());
            mi->die();
        }
    }
}

void Player::collision(Item *item)
{
    const unsigned M = 3;
    LX_Circle c(hitbox);
    c.radius *= M;
    c.square_radius = c.radius * c.radius;

    if(collisionCircleRect(c, item->box()))
    {
        takeBonus(item->getPowerUp());
        item->die();
    }
}


void Player::takeBonus(ItemType powerUp)
{
    switch(powerUp)
    {
    case POWER_UP::SCORE:
        bonus();
        break;

    case POWER_UP::HEALTH:
        heal();
        break;

    case POWER_UP::SHIELD:
        setShield(true);
        break;

    case POWER_UP::ROCKET:
        rocket();
        break;

    case POWER_UP::BOMB:
        bomb();
        break;

    case POWER_UP::LASER:
        laser();
        break;

    default:
        break;
    }
}


void Player::rocket()
{
    if((nb_rocket + NB_ROCKET_ADD) <= NBMAX_ROCKET)
        nb_rocket += NB_ROCKET_ADD;
    else
    {
        unsigned long score = (nb_rocket + NB_ROCKET_ADD - NBMAX_ROCKET) * ROCKET_SCORE;
        Engine::getInstance()->getScore()->notify(score);
        nb_rocket = NBMAX_ROCKET;
    }


    AudioHDL::getInstance()->playVoiceRocket();
    display->update();
}

void Player::bomb()
{
    if((nb_bomb + NB_BOMB_ADD) <= NBMAX_BOMB)
        nb_bomb += NB_BOMB_ADD;
    else
    {
        unsigned long score = (nb_bomb + NB_BOMB_ADD - NBMAX_BOMB) * BOMB_SCORE;
        Engine::getInstance()->getScore()->notify(score);
        nb_bomb = NBMAX_BOMB;
    }

    AudioHDL::getInstance()->playVoicePulse();
    display->update();
}


void Player::laser()
{
    laser_activated = true;
    laser_begin = LX_Timer::getTicks();
    AudioHandler::AudioHDL::getInstance()->playLaserShot();
}


void Player::heal()
{
    unsigned int heal_point;
    const uint32_t TEN = 10;
    const uint32_t FIVE = 5;
    const uint32_t FOUR = 4;
    const uint32_t TWO = 2;

    // Calculate the heal_point
    if(health_point < (max_health_point / TEN))
        heal_point = health_point * FIVE;

    else if(health_point < (max_health_point / FOUR))
        heal_point = health_point * TWO;

    else if(health_point < (max_health_point / TWO))
        heal_point = health_point;
    else
        heal_point = health_point / FOUR;

    // Calculate the resulting health_point
    if(health_point == max_health_point)
        Engine::getInstance()->getScore()->notify(HEALTH_SCORE);

    else if((health_point + heal_point) > max_health_point)
        health_point = max_health_point;

    else
        health_point += heal_point;

    display->update();
}


unsigned int Player::getBomb() const
{
    return nb_bomb;
}


unsigned int Player::getRocket() const
{
    return nb_rocket;
}


const LX_Physics::LX_Circle * Player::getHitbox()
{
    return &hitbox;
}


bool Player::isLaserActivated() const
{
    return laser_activated;
}


unsigned int Player::nb_death() const
{
    return deaths;
}


void Player::setShield(bool sh)
{
    const ResourceManager *rc = ResourceManager::getInstance();

    if(sh == true)
    {
        has_shield = true;
        shield_t = LX_Timer::getTicks();
        hit_count = HITS_UNDER_SHIELD;
        graphic = rc->getPlayerResource(true);

        if(still_alive)
            AudioHDL::getInstance()->playVoiceShield();
    }
    else
    {
        has_shield = false;
        graphic = rc->getPlayerResource();
    }
}
