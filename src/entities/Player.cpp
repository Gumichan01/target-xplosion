
/*
*	Target_Xplosion - A classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Luxon Jean-Pierre (Gumichan01)
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/


#include "Player.hpp"
#include "PlayerVisitor.hpp"
#include "Item.hpp"
#include "BasicMissile.hpp"
#include "Bomb.hpp"
#include "Rocket.hpp"
#include "Laser.hpp"

#include "../game/Game.hpp"
#include "../game/Hud.hpp"
#include "../game/Scoring.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Sound.hpp>
#include <LunatiX/LX_Physics.hpp>

using namespace LX_Random;
using namespace LX_FileIO;
using namespace LX_Physics;

LX_Point Player::last_position(0,0);// = LX_Point(0,0);

namespace
{
const unsigned int PLAYER_RADIUS = 24;
const unsigned int NBMAX_BOMB = 25;
const unsigned int NBMAX_ROCKET = 50;

const int BONUS_SCORE = 16;
const int PLAYER_BULLET_W = 24;
const int PLAYER_BULLET_H = 24;

const unsigned int SHIELD_TIME = 10000;
const unsigned int HITS_UNDER_SHIELD = 16;
};


Player::Player(unsigned int hp, unsigned int att, unsigned int sh,
               unsigned int critic, LX_Graphics::LX_Sprite *image,
               LX_Mixer::LX_Sound *audio,SDL_Rect& rect,
               LX_Vector2D& sp,
               int w_limit, int h_limit)
    : Character(hp, att, sh, image, audio, rect, sp), critical_rate(critic),
      nb_bomb(0),nb_rocket(0), bomb_activated(true),
      laser_activated(false), has_shield(false),shield_time(0),
      nb_hits(HITS_UNDER_SHIELD), nb_died(0),
      LIMIT_WIDTH(w_limit), LIMIT_HEIGHT(h_limit),
      basic_shoot(nullptr), rocket_shoot(nullptr), laser_shoot(nullptr),
      display(nullptr)
{
    initData();
    initHitboxRadius();
}


Player::~Player()
{
    display = nullptr;
}


void Player::initData(void)
{
    ResourceManager * rc = ResourceManager::getInstance();
    basic_shoot = rc->getSound(0);
    rocket_shoot = rc->getSound(1);
    laser_shoot = rc->getSound(2);
}


// A missile can get the last position of the player
void Player::accept(PlayerVisitor *pv)
{
    LX_Point p(last_position);
    pv->visit(p);
}


// initialize the hitbox
void Player::initHitboxRadius(void)
{
    unsigned int rad = PLAYER_RADIUS;
    unsigned int square_rad = rad*rad;

    hitbox.radius = rad;
    hitbox.square_radius = square_rad;
}


void Player::setHUD(HUD *h)
{
    display = h;
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
    case LASER_TYPE : // laser
    {
        if((SDL_GetTicks() - laser_begin) < laser_delay)
            laserShot();
        else
            laser_activated = false;
    }
    break;

    case BOMB_TYPE : // bomb
    {
        if(nb_bomb > 0)
        {
            nb_bomb--;
            bombShot();
        }
    }
    break;

    case ROCKET_TYPE :
    {
        if(nb_rocket > 0)
        {
            nb_rocket--;
            rocketShot();
        }
    }
    break;

    case DOUBLE_MISSILE_TYPE :
    {
        doubleShot();
    }
    break;

    case WAVE_MISSILE_TYPE :
    {
        largeShot();
    }
    break;

    default :
    {
        basicShot();
    }
    break;

    }

    display->update();
}

// Basic shot of the player
void Player::basicShot()
{
    SDL_Rect pos_mis;
    LX_Vector2D vel = LX_Vector2D(PLAYER_MISSILE_SPEED,0);
    unsigned int bonus_att = 0;

    LX_Graphics::LX_Sprite *tmp = nullptr;
    Game *g = Game::getInstance();
    ResourceManager *rc = ResourceManager::getInstance();

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    pos_mis.x = position.x + (position.w/2);
    pos_mis.y = position.y + ( (position.h - MISSILE_HEIGHT)/ 2);
    pos_mis.w = MISSILE_WIDTH;
    pos_mis.h = MISSILE_HEIGHT;

    tmp = rc->getResource(RC_MISSILE,0);

    basic_shoot->play();
    g->acceptPlayerMissile(new BasicMissile(attack_val + bonus_att,tmp,
                                            nullptr,pos_mis,vel));
}


void Player::rocketShot()
{
    SDL_Rect pos_mis;
    LX_Vector2D vel = LX_Vector2D(ROCKET_SPEED,0);
    unsigned int bonus_att = 0;

    LX_Graphics::LX_Sprite *tmp = nullptr;
    Game *g = Game::getInstance();
    ResourceManager *rc = ResourceManager::getInstance();

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    pos_mis.x = position.x + (position.w/2);
    pos_mis.y = position.y + ( (position.h - ROCKET_HEIGHT)/ 2);
    pos_mis.w = ROCKET_WIDTH;
    pos_mis.h = ROCKET_HEIGHT;

    tmp = rc->getResource(RC_MISSILE,1);
    rocket_shoot->play();
    g->acceptPlayerMissile(new Rocket(attack_val + bonus_att,tmp,
                                      nullptr,pos_mis,vel));
}


void Player::bombShot()
{
    SDL_Rect pos_mis;
    LX_Vector2D vel = LX_Vector2D(BOMB_SPEED,0);
    unsigned int bonus_att = 0;

    LX_Graphics::LX_Sprite *tmp = nullptr;
    Game *g = Game::getInstance();
    Score *sc = g->getScore();
    ResourceManager *rc = ResourceManager::getInstance();

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    pos_mis.x = position.x + (position.w/2);
    pos_mis.y = position.y + ( (position.h - BOMB_HEIGHT)/ 2);
    pos_mis.w = BOMB_WIDTH;
    pos_mis.h = BOMB_HEIGHT;

    tmp = rc->getResource(RC_MISSILE,2);

    g->acceptPlayerMissile(new Bomb(attack_val + bonus_att,tmp,
                                    rc->getSound(3),pos_mis,vel));

    display->update();
    sc->notify(-(BONUS_SCORE*sc->getKilledEnemies()));
}


void Player::laserShot()
{
    SDL_Rect pos_mis;
    LX_Vector2D vel;
    unsigned int bonus_att = 0;

    LX_Graphics::LX_Sprite *tmp = nullptr;
    Game *g = Game::getInstance();
    ResourceManager *rc = ResourceManager::getInstance();

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    pos_mis.x = position.x + (position.w - (position.w/4));
    pos_mis.y = position.y + ( (position.h - LASER_HEIGHT)/ 2);
    pos_mis.w = Game::getXlim();
    pos_mis.h = LASER_HEIGHT;

    tmp = rc->getResource(RC_MISSILE,3);

    g->acceptPlayerMissile(new Laser(attack_val + bonus_att,tmp,nullptr,
                                     pos_mis,vel));
}


void Player::doubleShot(void)
{
    specialShot(DOUBLE_MISSILE_TYPE);
}


void Player::largeShot(void)
{
    specialShot(WAVE_MISSILE_TYPE);
}

// It only concerns the double shots and the large shot
void Player::specialShot(const MISSILE_TYPE& type)
{
    const int offsetY1 = 8;
    const int offsetY2 = 36 ;
    const int offsetX = position.w - PLAYER_BULLET_W;
    const int offsetY3[] = {-5,5};
    const int SHOTS = 2;

    SDL_Rect pos[2];
    LX_Vector2D projectile_speed[2];
    unsigned int bonus_att = 0;

    LX_Graphics::LX_Sprite *tmp = nullptr;
    Game *cur_game = Game::getInstance();
    ResourceManager *rc = ResourceManager::getInstance();

    if(type == DOUBLE_MISSILE_TYPE)
    {
        pos[0] = {position.x + offsetX,position.y + offsetY1,
                  MISSILE_WIDTH,MISSILE_HEIGHT
                 };
        pos[1] = {position.x + offsetX,position.y + offsetY2,
                  MISSILE_WIDTH,MISSILE_HEIGHT
                 };

        projectile_speed[0] = LX_Vector2D(PLAYER_MISSILE_SPEED,0.0f);
        projectile_speed[1] = LX_Vector2D(PLAYER_MISSILE_SPEED,0.0f);
    }
    else
    {
        pos[0] = {position.x + offsetX,position.y + offsetY1,
                  PLAYER_BULLET_W,PLAYER_BULLET_H
                 };
        pos[1] = {position.x + offsetX,position.y + offsetY2,
                  PLAYER_BULLET_W,PLAYER_BULLET_H
                 };

        projectile_speed[0] = LX_Vector2D(PLAYER_MISSILE_SPEED,offsetY3[0]);
        projectile_speed[1] = LX_Vector2D(PLAYER_MISSILE_SPEED,offsetY3[1]);
    }

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    // The basic shoot sound
    basic_shoot->play();

    for(int i = 0; i < SHOTS; i++)
    {
        if(type == DOUBLE_MISSILE_TYPE)
            tmp = rc->getResource(RC_MISSILE,0);
        else
            tmp = rc->getResource(RC_MISSILE,4);

        cur_game->acceptPlayerMissile(new BasicMissile(attack_val + bonus_att,
                                      tmp,nullptr,pos[i],projectile_speed[i]));
    }
}


// manage the action of the player (movement and shield)
void Player::move()
{
    // Update the position and the hitbox on X
    position.x += speed.vx;
    hitbox.center.x += speed.vx;

    // Left or Right
    if((position.x <= 0) || ((position.x + position.w) > LIMIT_WIDTH))
    {
        position.x -= speed.vx;
        hitbox.center.x -= speed.vx;
    }

    // Do the same thing on Y
    position.y += speed.vy;
    hitbox.center.y += speed.vy;

    // Down or Up
    if((position.y <= 0) || ((position.y + position.h) > LIMIT_HEIGHT))
    {
        position.y -= speed.vy;
        hitbox.center.y -= speed.vy;
    }

    // Store the updated position of the player
    last_position = hitbox.center;

    // Check the shield
    if(has_shield == true)
    {
        if(SDL_GetTicks() - shield_time > SHIELD_TIME)
            setShield(false);
    }
}

void Player::die()
{
    Score *sc = Game::getInstance()->getScore();
    unsigned int old_sc = sc->getCurrentScore();
    int score = -(static_cast<int>(old_sc)/2);

    nb_died++;
    health_point = 0;
    Entity::die();
    display->update();
    sc->notify(score);
}


void Player::reborn()
{
    health_point = max_health_point;
    still_alive = true;
    setShield(true);
    position.x = 0;
    position.y = (Game::getYlim() - position.h)/2;
    speed = {0,0};

    hitbox.center = LX_Point(position.x +
                             (((position.x + position.w) - position.x)/2),
                             position.y +
                             (((position.y + position.h) - position.y)/2));
    initHitboxRadius();
    display->update();
    Game::getInstance()->screenCancel();
}


void Player::updateHUD()
{
    display->displayHUD();
}


void Player::collision(Missile *mi)
{
    if(mi->getX() >= position.x)
    {
        if(collisionCircleRect(hitbox,*mi->getHitbox()))
        {
            receiveDamages(mi->hit());
            mi->die();
        }
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


void Player::rocket(void)
{
    rocket_activated = true;

    if((nb_rocket + 10) < NBMAX_ROCKET)
        nb_rocket += 10;
    else
        nb_rocket = NBMAX_ROCKET;

    display->update();
}


void Player::bomb(void)
{
    bomb_activated = true;

    if((nb_bomb + 5) < NBMAX_BOMB)
        nb_bomb += 5;
    else
        nb_bomb = NBMAX_BOMB;

    display->update();
}


void Player::laser(void)
{
    laser_activated = true;
    laser_begin = SDL_GetTicks();

    if(laser_shoot != nullptr)
        laser_shoot->play();
}


void Player::heal(void)
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


void Player::bonus(void)
{
    Score *sc = Game::getInstance()->getScore();
    int n = static_cast<int>(sc->getKilledEnemies());

    if(n > 0)
        sc->notify(BONUS_SCORE*n);
}


unsigned int Player::getBomb()
{
    return nb_bomb;
}


unsigned int Player::getRocket()
{
    return nb_rocket;
}


const LX_Physics::LX_Circle * Player::getHitbox()
{
    return &hitbox;
}


bool Player::isLaserActivated()
{
    return laser_activated;
}


unsigned int Player::nb_death()
{
    return nb_died;
}


void Player::setShield(bool sh)
{
    ResourceManager *rc = ResourceManager::getInstance();

    if(sh == true)
    {
        has_shield = true;
        shield_time = SDL_GetTicks();
        nb_hits = HITS_UNDER_SHIELD;
        graphic = rc->getPlayerResource(true);
    }
    else
    {
        has_shield = false;
        graphic = rc->getPlayerResource();
    }
}
