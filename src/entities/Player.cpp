

/*
*	Target_Xplosion - The classic shoot'em up video game
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
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file Player.cpp
*	@brief The Player file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Chunk.hpp>
#include <LunatiX/LX_Mixer.hpp>
#include <LunatiX/LX_FileBuffer.hpp>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Hitbox.hpp>

#include "Player.hpp"
#include "../game/Game.hpp"
#include "../game/hud.hpp"

#include "BasicMissile.hpp"
#include "Bomb.hpp"
#include "Rocket.hpp"
#include "Laser.hpp"
#include "../xml/XMLReader.hpp"
#include "../resources/ResourceManager.hpp"

using namespace LX_Random;
using namespace LX_FileIO;
using namespace LX_Physics;

LX_Point Player::last_position(0,0);// = LX_Point(0,0);

static const unsigned int NBMAX_BOMB = 25;
static const unsigned int NBMAX_ROCKET = 50;
static const int LOST_POINT = 128;
static const int BONUS_SCORE = 16;


Player::Player(unsigned int hp, unsigned int att, unsigned int sh,
               unsigned int critic, SDL_Texture *image,
               LX_Mixer::LX_Chunk *audio,SDL_Rect& rect,
               LX_Vector2D& sp,
               int w_limit, int h_limit)
    : Character(hp, att, sh, image, audio, rect, sp), critical_rate(critic),
      nb_bomb(0),nb_rocket(0), bomb_activated(true),
      laser_activated(false), has_shield(false),shield_time(0),
      nb_hits(HITS_UNDER_SHIELD), nb_died(0),
      LIMIT_WIDTH(w_limit), LIMIT_HEIGHT(h_limit),
      basic_shoot(nullptr), rocket_shoot(nullptr), laser_shoot(nullptr),
      display(nullptr), playerShoot(nullptr), playerMissile(nullptr),
      playerBomb(nullptr), playerBullet(nullptr)
{
    initData();
    initHitboxRadius();
}


Player::~Player()
{
    delete laser_shoot;
    delete rocket_shoot;
    delete basic_shoot;
    delete playerBullet;
    delete playerLaser;
    delete playerBomb;
    delete playerMissile;
    delete playerShoot;
    display = nullptr;
}


void Player::initData(void)
{
    TX_Asset *tx = TX_Asset::getInstance();
    const unsigned int NB_PLAYER_MISSILES = 5;
    std::string missilesFiles[NB_PLAYER_MISSILES];

    for(unsigned int i = 0; i < NB_PLAYER_MISSILES; i++)
    {
        missilesFiles[i] = tx->getPlayerMissilesFile(i);
    }

    // Additionnal information
    playerShoot = new LX_FileBuffer(missilesFiles[0].c_str());
    playerMissile = new LX_FileBuffer(missilesFiles[1].c_str());
    playerBomb = new LX_FileBuffer(missilesFiles[2].c_str());
    playerLaser = new LX_FileBuffer(missilesFiles[3].c_str());
    playerBullet = new LX_FileBuffer(missilesFiles[4].c_str());

    basic_shoot = LX_Mixer::loadSample("audio/longshot.wav");
    rocket_shoot = LX_Mixer::loadSample("audio/rocket.wav");
    laser_shoot = LX_Mixer::loadSample("audio/playerlaser.wav");

    basic_shoot->volume(MIX_MAX_VOLUME - (MIX_MAX_VOLUME/4));
    rocket_shoot->volume(MIX_MAX_VOLUME/4);
    rocket_shoot->volume(MIX_MAX_VOLUME - (MIX_MAX_VOLUME/4));
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
void Player::fire(MISSILE_TYPE m_type)
{
    if(laser_activated)
        m_type = LASER_TYPE;

    switch(m_type)
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

    SDL_Surface *tmpS = nullptr;
    SDL_Texture *tmpT = nullptr;
    Game *g = Game::getInstance();

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    pos_mis.x = position.x + (position.w/2);
    pos_mis.y = position.y + ( (position.h - MISSILE_HEIGHT)/ 2);
    pos_mis.w = MISSILE_WIDTH;
    pos_mis.h = MISSILE_HEIGHT;

    tmpS = playerShoot->getSurfaceFromBuffer();
    tmpT = LX_Graphics::loadTextureFromSurface(tmpS);

    basic_shoot->play();
    g->addPlayerMissile(new BasicMissile(attack_val + bonus_att,tmpT,
                                         nullptr,pos_mis,vel));
    SDL_FreeSurface(tmpS);
}


void Player::rocketShot()
{
    SDL_Rect pos_mis;
    LX_Vector2D vel = LX_Vector2D(ROCKET_SPEED,0);
    unsigned int bonus_att = 0;

    SDL_Surface *tmpS = nullptr;
    SDL_Texture *tmpT = nullptr;
    Game *g = Game::getInstance();

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    pos_mis.x = position.x + (position.w/2);
    pos_mis.y = position.y + ( (position.h - ROCKET_HEIGHT)/ 2);
    pos_mis.w = ROCKET_WIDTH;
    pos_mis.h = ROCKET_HEIGHT;

    tmpS = playerMissile->getSurfaceFromBuffer();
    tmpT = LX_Graphics::loadTextureFromSurface(tmpS);
    SDL_FreeSurface(tmpS);

    rocket_shoot->play();
    g->addPlayerMissile(new Rocket(attack_val + bonus_att,tmpT,nullptr,pos_mis,
                                   vel));
}


void Player::bombShot()
{
    SDL_Rect pos_mis;
    LX_Vector2D vel = LX_Vector2D(BOMB_SPEED,0);
    unsigned int bonus_att = 0;

    SDL_Surface *tmpS = nullptr;
    SDL_Texture *tmpT = nullptr;
    Game *g = Game::getInstance();
    Score *sc = g->getScore();

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    pos_mis.x = position.x + (position.w/2);
    pos_mis.y = position.y + ( (position.h - BOMB_HEIGHT)/ 2);
    pos_mis.w = BOMB_WIDTH;
    pos_mis.h = BOMB_HEIGHT;

    tmpS = playerBomb->getSurfaceFromBuffer();
    tmpT = LX_Graphics::loadTextureFromSurface(tmpS);
    SDL_FreeSurface(tmpS);

    g->addPlayerMissile(new Bomb(attack_val + bonus_att,tmpT,
                                 LX_Mixer::loadSample("audio/explosion.wav"),
                                 pos_mis,vel));

    display->update();
    sc->notify(-(BONUS_SCORE*sc->getKilledEnemies()));
}


void Player::laserShot()
{
    SDL_Rect pos_mis;
    LX_Vector2D vel;
    unsigned int bonus_att = 0;

    SDL_Surface *tmpS = nullptr;
    SDL_Texture *tmpT = nullptr;
    Game *g = Game::getInstance();

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    pos_mis.x = position.x + (position.w - (position.w/4));
    pos_mis.y = position.y + ( (position.h - LASER_HEIGHT)/ 2);
    pos_mis.w = Game::game_Xlimit;
    pos_mis.h = LASER_HEIGHT;

    tmpS = playerLaser->getSurfaceFromBuffer();
    tmpT = LX_Graphics::loadTextureFromSurface(tmpS);
    SDL_FreeSurface(tmpS);

    g->addPlayerMissile(new Laser(attack_val + bonus_att,tmpT,nullptr,
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
void Player::specialShot(MISSILE_TYPE type)
{
    const int offsetY1 = 8;
    const int offsetY2 = 36 ;
    const int offsetX = position.w - PLAYER_BULLET_W;
    const int offsetY3[] = {-5,5};
    const int SHOTS = 2;

    SDL_Rect pos[2];
    LX_Vector2D projectile_speed[2];
    unsigned int bonus_att = 0;

    SDL_Surface *tmpS = nullptr;
    Game *cur_game = Game::getInstance();

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
        tmpS = playerShoot->getSurfaceFromBuffer();
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
        tmpS = playerBullet->getSurfaceFromBuffer();
    }

    if(xorshiftRand100() <= critical_rate)
        bonus_att = critical_rate;

    // The basic shoot sound
    basic_shoot->play();

    for(int i = 0; i < SHOTS; i++)
    {
        cur_game->addPlayerMissile(new BasicMissile(attack_val + bonus_att,
                                   LX_Graphics::loadTextureFromSurface(tmpS),
                                   nullptr,pos[i],projectile_speed[i]));
    }
    SDL_FreeSurface(tmpS);
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
    unsigned int nb_killed = sc->getKilledEnemies();
    int score = -((LOST_POINT* static_cast<int>(nb_killed)));

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
    position.y = (Game::game_Ylimit - position.h)/2;
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


void Player::takeBonus(POWER_UP powerUp)
{
    switch(powerUp)
    {
    case POWER_UP::SCORE :
        bonus();
        break;

    case POWER_UP::HEALTH :
        heal();
        break;

    case POWER_UP::SHIELD :
        setShield(true);
        break;

    case POWER_UP::ROCKET :
        rocket();
        break;

    case POWER_UP::BOMB :
        bomb();
        break;

    case POWER_UP::LASER :
        laser();
        break;

    default :
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
    SDL_DestroyTexture(graphic);

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

