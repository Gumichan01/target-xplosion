
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
#include "Item.hpp"
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
#include <LunatiX/LX_Chunk.hpp>
#include <LunatiX/LX_Mixer.hpp>
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
const unsigned int NBMIN_BOMB = 4;
const unsigned int NBMAX_BOMB = 16;
const unsigned int NBMAX_ROCKET = 50;

const unsigned int BASIC_SHOT_ID = 0;
const unsigned int ROCKET_SHOT_ID = 1;
const unsigned int BOMB_SHOT_ID = 2;
const unsigned int LASER_SHOT_ID = 3;
const unsigned int BULLET_SHOT_ID = 5;

// Noise ID for the bomb
const unsigned int LASER_NOISE_ID = 2;
const unsigned int EXPLOSION_NOISE_ID = 3;

const int BONUS_SCORE = 32;
const int PLAYER_BULLET_W = 24;
const int PLAYER_BULLET_H = 24;

const unsigned int SHIELD_TIME = 10000;
const unsigned int HITS_UNDER_SHIELD = 16;

void bonus()
{
    Score *sc = Engine::getInstance()->getScore();
    int n = static_cast<int>(sc->getKilledEnemies());

    if(n > 0)
        sc->notify(BONUS_SCORE*n);
}

};


Player::Player(unsigned int hp, unsigned int att, unsigned int sh,
               unsigned int critic, LX_Graphics::LX_Sprite *image,
               LX_Mixer::LX_Chunk *audio, LX_AABB& rect,
               LX_Vector2D& sp, int w_limit, int h_limit)
    : Character(hp, att, sh, image, audio, rect, sp), critical_rate(critic),
      nb_bomb(0), nb_rocket(0), bomb_activated(true),
      laser_activated(false), has_shield(false), shield_time(0),
      nb_hits(HITS_UNDER_SHIELD), nb_died(0), laser_begin(0),
      laser_delay(LASER_LIFETIME), LIMIT_WIDTH(w_limit), LIMIT_HEIGHT(h_limit),
      basic_shot(nullptr), rocket_shot(nullptr), laser_shot(nullptr),
      display(nullptr)
{
    initData();
    initHitboxRadius();
    display = new PlayerHUD(*this);
    Engine::getInstance()->acceptHUD(display);
}


Player::~Player()
{
    delete display;
    display = nullptr;
}


void Player::initData()
{
    const ResourceManager * rc = ResourceManager::getInstance();
    basic_shot  = rc->getSound(BASIC_SHOT_ID);
    rocket_shot = rc->getSound(ROCKET_SHOT_ID);
    laser_shot  = rc->getSound(LASER_NOISE_ID);
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
        nb_hits--;

        // Must we remove the shield ?
        if(nb_hits == 0)
            setShield(false);
    }

    Character::receiveDamages(attacks);
    display->update();
}

// FIRE !!
void Player::fire(const MISSILE_TYPE& m_type)
{
    MISSILE_TYPE ty;

    if(laser_activated)
        ty = LASER_TYPE;
    else
        ty = m_type;

    switch(ty)
    {
    case LASER_TYPE:
    {
        if((LX_Timer::getTicks() - laser_begin) < laser_delay)
            laserShot();
        else
            laser_activated = false;
    }
    break;

    case BOMB_TYPE:
    {
        if(nb_bomb > 0)
        {
            nb_bomb--;
            bombShot();
        }
    }
    break;

    case ROCKET_TYPE:
    {
        if(nb_rocket > 0)
        {
            nb_rocket--;
            rocketShot();
        }
    }
    break;

    case DOUBLE_MISSILE_TYPE:
    case WAVE_MISSILE_TYPE:
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
    LX_AABB pos_mis;
    LX_Vector2D vel = LX_Vector2D(ROCKET_SPEED, 0);
    unsigned int bonus_att = 0;

    LX_Graphics::LX_Sprite *tmp = nullptr;
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    pos_mis.x = position.x + (position.w/2);
    pos_mis.y = position.y + ( (position.h - ROCKET_HEIGHT)/ 2);
    pos_mis.w = ROCKET_WIDTH;
    pos_mis.h = ROCKET_HEIGHT;

    tmp = rc->getResource(RC_MISSILE, ROCKET_SHOT_ID);
    LX_Mixer::groupPlayChunk(*rocket_shot, AudioHandler::AUDIOHANDLER_PLAYER_TAG);
    g->acceptPlayerMissile(new PlayerRocket(attack_val + bonus_att, tmp,
                                            nullptr, pos_mis, vel));
}


void Player::bombShot()
{
    LX_AABB pos_mis;
    LX_Vector2D vel = LX_Vector2D(BOMB_SPEED, 0);
    unsigned int bonus_att = 0;

    LX_Graphics::LX_Sprite *tmp = nullptr;
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    pos_mis.x = position.x + (position.w/2);
    pos_mis.y = position.y + ( (position.h - BOMB_HEIGHT)/ 2);
    pos_mis.w = BOMB_WIDTH;
    pos_mis.h = BOMB_HEIGHT;

    tmp = rc->getResource(RC_MISSILE, BOMB_SHOT_ID);

    g->acceptPlayerMissile(new Bomb(attack_val + bonus_att, tmp,
                                    rc->getSound(EXPLOSION_NOISE_ID), pos_mis,
                                    vel));

    display->update();
}


void Player::laserShot()
{
    LX_AABB pos_mis;
    LX_Vector2D vel;
    unsigned int bonus_att = 0;

    LX_Graphics::LX_Sprite *tmp = nullptr;
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    pos_mis.x = position.x + (position.w - (position.w/4));
    pos_mis.y = position.y + ( (position.h - LASER_HEIGHT)/ 2);
    pos_mis.w = Engine::getMaxXlim();
    pos_mis.h = LASER_HEIGHT;

    tmp = rc->getResource(RC_MISSILE, LASER_SHOT_ID);

    g->acceptPlayerMissile(new Laser(attack_val + bonus_att, tmp, nullptr,
                                     pos_mis, vel));
}

// It only concerns the double shots and the large shot
void Player::specialShot(const MISSILE_TYPE& type)
{
    const int offsetY1 = 8;
    const int offsetY2 = 36 ;
    const int offsetX = position.w - PLAYER_BULLET_W;
    const int offsetY3[] = {-2,2};
    const int SHOTS = 2;

    LX_AABB pos[2];
    LX_Vector2D projectile_speed[2];
    unsigned int bonus_att = 0;

    Engine *cur_game = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();

    if(type == DOUBLE_MISSILE_TYPE)
    {
        pos[0] = {position.x + offsetX, position.y + offsetY1,
                  MISSILE_WIDTH, MISSILE_HEIGHT
                 };
        pos[1] = {position.x + offsetX, position.y + offsetY2,
                  MISSILE_WIDTH, MISSILE_HEIGHT
                 };

        projectile_speed[0] = LX_Vector2D(PLAYER_MISSILE_SPEED, 0.0f);
        projectile_speed[1] = LX_Vector2D(PLAYER_MISSILE_SPEED, 0.0f);
    }
    else
    {
        pos[0] = {position.x + offsetX, position.y + offsetY1,
                  PLAYER_BULLET_W, PLAYER_BULLET_H
                 };
        pos[1] = {position.x + offsetX, position.y + offsetY2,
                  PLAYER_BULLET_W, PLAYER_BULLET_H
                 };

        projectile_speed[0] = LX_Vector2D(PLAYER_MISSILE_SPEED, offsetY3[0]);
        projectile_speed[1] = LX_Vector2D(PLAYER_MISSILE_SPEED, offsetY3[1]);
    }

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    // The basic shot sound
    LX_Mixer::groupPlayChunk(*basic_shot, AudioHandler::AUDIOHANDLER_PLAYER_TAG);
    LX_Graphics::LX_Sprite *tmp = rc->getResource(RC_MISSILE, BULLET_SHOT_ID);

    for(int i = 0; i < SHOTS; i++)
    {
        cur_game->acceptPlayerMissile(new BasicMissile(attack_val + bonus_att,
                                      tmp, nullptr, pos[i], projectile_speed[i]));
    }
}


// manage the action of the player (movement and shield)
void Player::move()
{
    const int min_xlim = Engine::getMinXlim();
    const int min_ylim = Engine::getMinYlim();
    // Update the position and the hitbox on X
    position.x += speed.vx;
    hitbox.center.x += speed.vx;

    // Left or Right
    if((position.x <= min_xlim) || ((position.x + position.w) > LIMIT_WIDTH))
    {
        position.x -= speed.vx;
        hitbox.center.x -= speed.vx;
    }

    // Do the same thing on Y
    position.y += speed.vy;
    hitbox.center.y += speed.vy;

    // Down or Up
    if((position.y <= min_ylim) || ((position.y + position.h) > LIMIT_HEIGHT))
    {
        position.y -= speed.vy;
        hitbox.center.y -= speed.vy;
    }

    // Store the updated position of the player
    last_position = hitbox.center;

    // Check the shield
    if(has_shield == true)
    {
        if(LX_Timer::getTicks() - shield_time > SHIELD_TIME)
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
    nb_died++;
    health_point = 0;
    Entity::die();
    display->update();
    Engine::getInstance()->getScore()->resetCombo();
}


void Player::reborn()
{
    setShield(true);
    health_point = max_health_point;
    still_alive = true;
    position.x = 0;
    position.y = (Engine::getMaxYlim() - position.h)/2;
    speed = {0,0};

    hitbox.center = LX_Point(position.x + (((position.x + position.w) - position.x)/2),
                             position.y + (((position.y + position.h) - position.y)/2));
    initHitboxRadius();
    display->update();
    Engine::getInstance()->screenCancel();
}


void Player::collision(Missile *mi)
{
    if(mi->getX() >= position.x)
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


void Player::takeBonus(const POWER_UP& powerUp)
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
    rocket_activated = true;

    if((nb_rocket + 10) < NBMAX_ROCKET)
        nb_rocket += 10;
    else
        nb_rocket = NBMAX_ROCKET;

    AudioHDL::getInstance()->playVoiceRocket();
    display->update();
}

void Player::bomb()
{
    bomb_activated = true;

    if((nb_bomb + NBMIN_BOMB) < NBMAX_BOMB)
        nb_bomb += NBMIN_BOMB;
    else
        nb_bomb = NBMAX_BOMB;

    AudioHDL::getInstance()->playVoicePulse();
    display->update();
}


void Player::laser()
{
    laser_activated = true;
    laser_begin = LX_Timer::getTicks();

    if(laser_shot != nullptr)
        laser_shot->play();
}


void Player::heal()
{
    unsigned int heal_point;

    if(health_point < (max_health_point/10))
    {
        heal_point = health_point*5;
    }
    else if(health_point < (max_health_point/4))
    {
        heal_point = health_point*2;
    }
    else if(health_point < (max_health_point/2))
    {
        heal_point = health_point;
    }
    else
        heal_point = health_point/4;

    if((health_point + heal_point) > max_health_point)
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
    return nb_died;
}


void Player::setShield(bool sh)
{
    const ResourceManager *rc = ResourceManager::getInstance();

    if(sh == true)
    {
        has_shield = true;
        shield_time = LX_Timer::getTicks();
        nb_hits = HITS_UNDER_SHIELD;
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
