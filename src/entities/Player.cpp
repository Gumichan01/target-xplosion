

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2015  Luxon Jean-Pierre
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
#include <LunatiX/LX_Sound.hpp>
#include <LunatiX/LX_Chunk.hpp>
#include <LunatiX/LX_FileIO.hpp>
#include <LunatiX/LX_FileBuffer.hpp>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Physics.hpp>

#include "Player.hpp"
#include "../game/Game.hpp"

#include "Basic_missile.hpp"
#include "Bomb.hpp"
#include "Rocket.hpp"
#include "Laser.hpp"
#include "../xml/XMLReader.hpp"


using namespace LX_Random;
using namespace LX_FileIO;


static const unsigned int NBMAX_BOMB = 50;
static const unsigned int NBMAX_ROCKET = 100;
static const int LOST_POINT = 64;
static const int BONUS_SCORE = 256;


Player::Player(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic,
               SDL_Texture *image, LX_Chunk *audio,
               int x, int y, int w, int h,int dX, int dY, unsigned int w_limit, unsigned h_limit)
    : Character(hp, att, sh, image, audio, x, y, w, h, dX, dY)
{
    critical_rate = critic;
    nb_bomb = 0;
    nb_rocket = 0;
    has_shield = false;
    bomb_activated = false;
    rocket_activated = false;
    laser_activated = false;

    LIMIT_WIDTH = w_limit;
    LIMIT_HEIGHT = h_limit;

    display = new HUD(this);

    initData();
    init_hitbox(x,y,w,h);
}


Player::Player(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic,
               SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp,
               unsigned int w_limit, unsigned h_limit)
    : Character(hp, att, sh, image, audio, rect, sp)
{
    critical_rate = critic;
    nb_bomb = 5;
    nb_rocket = 10;
    has_shield = false;
    bomb_activated = true;
    rocket_activated = true;
    laser_activated = false;

    LIMIT_WIDTH = w_limit;
    LIMIT_HEIGHT = h_limit;

    display = new HUD(this);

    initData();
    init_hitbox(rect->x,rect->y,rect->w,rect->h);
}


Player::~Player()
{
    delete playerLaser;
    delete playerBomb;
    delete playerMissile;
    delete playerShoot;
    delete basic_shoot;
    delete rocket_shoot;
    delete laser_shoot;
    delete display;
    delete playerWithoutSH;
    delete playerWithSH;
}


void Player::receive_damages(unsigned int attacks)
{
    // Take less damages if the shied is activated
    if(has_shield == true)
    {
        attacks /= 2;
        nb_hits--;

        // Must we remove the shield ?
        if(nb_hits == 0)
        {
            set_shield(false);
        }
    }

    Character::receive_damages(attacks);
    display->update();                      // The player's state has changed
}



void Player::initData(void)
{
    TX_Asset *tx = TX_Asset::getInstance();
    const std::string * missilesFiles = tx->playerMissilesFiles();

    playerWithoutSH = new LX_FileBuffer(tx->playerFile());
    playerWithSH = new LX_FileBuffer(tx->playerShieldFile());
    playerShoot = new LX_FileBuffer(missilesFiles[0].c_str());
    playerMissile = new LX_FileBuffer(missilesFiles[1].c_str());
    playerBomb = new LX_FileBuffer(missilesFiles[2].c_str());
    playerLaser = new LX_FileBuffer(missilesFiles[3].c_str());

    basic_shoot = LX_Mixer::loadSample("audio/longshot.wav");
    rocket_shoot = LX_Mixer::loadSample("audio/rocket.wav");
    laser_shoot = NULL;

    basic_shoot->volume(MIX_MAX_VOLUME - (MIX_MAX_VOLUME/4));
    rocket_shoot->volume(MIX_MAX_VOLUME/4);
}



// initialize the hitbox
void Player::init_hitbox(int x, int y, int w, int h)
{
    int xCenter = x + (((x + w) - x)/2);
    int yCenter = y + (((y + h) - y)/2);

    int rad = PLAYER_RADIUS;
    int square_rad = rad*rad;

    hitbox = {xCenter, yCenter, static_cast<unsigned int>(rad), static_cast<unsigned int>(square_rad)};
}


// create a new missile according to the type of the missile
Missile * Player::shoot(MISSILE_TYPE m_type)
{
    SDL_Rect pos_mis;   // the missiles position
    LX_Vector2D sp_mis;       // the missiles speed
    unsigned int bonus_att = 0;

    SDL_Surface *tmpS = NULL;
    SDL_Texture *tmpT = NULL;

    if( xorshiftRand100() <= critical_rate)
    {
        bonus_att = critical_rate;
    }

    pos_mis.x = position.x + (position.w/2);
    pos_mis.y = position.y + ( (position.h - MISSILE_HEIGHT)/ 2);

    pos_mis.w = MISSIlE_WIDTH;
    pos_mis.h = MISSILE_HEIGHT;
    sp_mis = {MISSILE_SPEED,0};

    switch(m_type)
    {
        case ROCKET_TYPE : // rocket
        {
            pos_mis.y = position.y + ( (position.h - ROCKET_HEIGHT)/ 2);

            pos_mis.w = ROCKET_WIDTH;
            pos_mis.h = ROCKET_HEIGHT;
            sp_mis = {ROCKET_SPEED,0};

            tmpS = playerMissile->getSurfaceFromBuffer();
            tmpT = LX_Graphics::loadTextureFromSurface(tmpS,0);
            SDL_FreeSurface(tmpS);

            rocket_shoot->play();
            return (new Rocket(attack_val + bonus_att,
                               LX_Graphics::loadTextureFromFile("image/missile.png",0),
                               NULL,&pos_mis,&sp_mis));
        }
        break;


        case LASER_TYPE : // laser
        {
            pos_mis.x = position.x + position.w;
            pos_mis.y = position.y + ( (position.h - LASER_HEIGHT)/ 2);

            pos_mis.w = Game::game_Xlimit - position.x;
            pos_mis.h = LASER_HEIGHT;
            sp_mis = {0,0};

            tmpS = playerLaser->getSurfaceFromBuffer();
            tmpT = LX_Graphics::loadTextureFromSurface(tmpS,0);
            SDL_FreeSurface(tmpS);

            return (new Laser(attack_val + bonus_att,tmpT,NULL,&pos_mis,&sp_mis));
        }
        break;


        case BOMB_TYPE : // bomb
        {
            pos_mis.y = position.y + ( (position.h - BOMB_HEIGHT)/ 2);

            pos_mis.w = BOMB_WIDTH;
            pos_mis.h = BOMB_HEIGHT;
            sp_mis = {BOMB_SPEED,0};

            tmpS = playerBomb->getSurfaceFromBuffer();
            tmpT = LX_Graphics::loadTextureFromSurface(tmpS,0);
            SDL_FreeSurface(tmpS);

            return (new Bomb(attack_val + bonus_att,tmpT,
                             LX_Mixer::loadSample("audio/explosion.wav"),&pos_mis,&sp_mis));
        }
        break;

        default :
        {
            tmpS = playerShoot->getSurfaceFromBuffer();
            tmpT = LX_Graphics::loadTextureFromSurface(tmpS,0);
            SDL_FreeSurface(tmpS);

            basic_shoot->play();
            return (new Basic_missile(attack_val + bonus_att,tmpT,NULL,&pos_mis,&sp_mis));
        }
        break;
    }
}


// FIRE !!
void Player::fire(MISSILE_TYPE m_type)
{
    Game *cur_game = Game::getInstance();

    if(laser_activated)
        m_type = LASER_TYPE;

    switch(m_type)
    {
        case LASER_TYPE : // laser
        {
            if( (SDL_GetTicks() - laser_begin) < laser_delay)
            {
                cur_game->addPlayerMissile(shoot(m_type));
            }
            else
            {
                laser_activated = false;
            }
        }
        break;

        case BOMB_TYPE : // bomb
        {
            if(bomb_activated == true)
            {
                if(nb_bomb > 0)
                {
                    nb_bomb -= 1;
                    cur_game->addPlayerMissile(shoot(m_type));
                    display->update();
                }
            }
        }
        break;

        case ROCKET_TYPE :
        {
            if(rocket_activated == true)
            {
                if(nb_rocket > 0)
                {
                    nb_rocket -= 1;
                    cur_game->addPlayerMissile(shoot(m_type));
                    display->update();
                }
            }
        }
        break;

        case DOUBLE_MISSILE_TYPE :
        {
            double_shoot();
        }
        break;

        case WAVE_MISSILE_TYPE :
        {
            triple_shoot();
        }
        break;

        default :
            cur_game->addPlayerMissile(shoot(m_type));
            break;
    }
}


void Player::double_shoot(void)
{
    SDL_Rect pos1, pos2;
    LX_Vector2D sp;
    unsigned int bonus_att = 0;

    SDL_Texture *tmp1, *tmp2 = NULL;
    Game *cur_game = Game::getInstance();

    pos1 = {position.x + 41,position.y + 13,MISSIlE_WIDTH,MISSILE_HEIGHT};
    pos2 = {position.x + 41,position.y + 41,MISSIlE_WIDTH,MISSILE_HEIGHT};
    sp = {MISSILE_SPEED,0};

    if( xorshiftRand100() <= critical_rate)
    {
        bonus_att = critical_rate;
    }

    tmp1 = LX_Graphics::loadTextureFromFile("image/shoot.png",0);
    tmp2 = LX_Graphics::loadTextureFromFile("image/shoot.png",0);
    basic_shoot->play();

    cur_game->addPlayerMissile(new Basic_missile(attack_val + bonus_att,
                                                 tmp1,NULL,&pos1,&sp));

    cur_game->addPlayerMissile(new Basic_missile(attack_val + bonus_att,
                                                 tmp2,NULL,&pos2,&sp));
}


void Player::triple_shoot(void)
{
    SDL_Rect pos1, pos2;
    LX_Vector2D sp1, sp2;
    unsigned int bonus_att = 0;

    SDL_Texture *tmp1, *tmp2 = NULL;
    Game *cur_game = Game::getInstance();

    pos1 = {position.x + 41,position.y + 13,MISSIlE_WIDTH,MISSILE_HEIGHT};
    pos2 = {position.x + 41,position.y + 41,MISSIlE_WIDTH,MISSILE_HEIGHT};
    sp1 = {MISSILE_SPEED,-6};
    sp2 = {MISSILE_SPEED,6};

    if( xorshiftRand100() <= critical_rate)
    {
        bonus_att = critical_rate;
    }

    tmp1 = LX_Graphics::loadTextureFromFile("image/shoot_6.png",0);
    tmp2 = LX_Graphics::loadTextureFromFile("image/shoot_-6.png",0);
    basic_shoot->play();

    cur_game->addPlayerMissile(new Basic_missile(attack_val + bonus_att,
                                                 tmp1,NULL,&pos1,&sp1));

    cur_game->addPlayerMissile(new Basic_missile(attack_val + bonus_att,
                                                 tmp2,NULL,&pos2,&sp2));
}


// manage the player's action (movement and shield)
void Player::move()
{
    position.x += speed.vx;
    hitbox.xCenter += speed.vx;

    // left or right
    if( (position.x <= 0) || ( (position.x + position.w) > Game::game_Xlimit ) )
    {
        position.x -= speed.vx;
        hitbox.xCenter -= speed.vx;
    }


    position.y += speed.vy;
    hitbox.yCenter += speed.vy;

    //down or up
    if( (position.y  <= 0) || ((position.y + position.h) > LIMIT_HEIGHT )  )
    {
        position.y -= speed.vy;
        hitbox.yCenter -= speed.vy;
    }

    if(shield == true)
    {
        if(SDL_GetTicks() - shield_time > SHIELD_TIME)
        {
            set_shield(false);
        }
    }
}

void Player::die()
{
    const int killed = Game::score->nb_killed_enemies();

    health_point = 0;
    Entity::die();
    display->update();
    Game::score->notify(-(LOST_POINT*(killed + 1)));
}


void Player::reborn()
{
    health_point = max_health_point;
    still_alive = true;
    set_shield(true);
    position.x = 0;
    position.y = Game::game_Ylimit - position.h;
    speed = {0,0};

    init_hitbox(position.x,position.y,position.w,position.h);
    display->update();
}


void Player::updateHUD()
{
    display->display_HUD();
}


void Player::collision(Missile *mi)
{
    if(mi->getX() >= position.x)
    {
        if(LX_Physics::collisionCircleRect(&hitbox,mi->get_hitbox()))
        {
            receive_damages(mi->put_damages());
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
            set_shield(true);
            break;

        case POWER_UP::ROCKET :
            rocket();
            break;

        case POWER_UP::BOMB :
            bomb();
            break;

        case POWER_UP::LASER :
            laser_activated = true;
            laser_begin = SDL_GetTicks();
            break;

        default :
            break;
    }
}


void Player::rocket(void)
{
    rocket_activated = true;

    if((nb_rocket + 20) < NBMAX_ROCKET)
        nb_rocket += 20;
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
    const unsigned int n = Game::score->nb_killed_enemies();

    if(n > 0)
        Game::score->notify(BONUS_SCORE*n);
}


unsigned int Player::getBomb()
{
    return nb_bomb;
}


unsigned int Player::getRocket()
{
    return nb_rocket;
}


LX_Circle * Player::get_hitbox()
{
    return &hitbox;
}


bool Player::isLaser_activated()
{
    return laser_activated;
}


void Player::set_shield(bool sh)
{
    SDL_Surface *tmp = NULL;

    SDL_DestroyTexture(graphic);

    if(sh == true)
    {
        has_shield = true;
        shield_time = SDL_GetTicks();
        nb_hits = HITS_UNDER_SHIELD;
        tmp = playerWithSH->getSurfaceFromBuffer();
        graphic = LX_Graphics::loadTextureFromSurface(tmp,0);
    }
    else
    {
        has_shield = false;
        tmp = playerWithoutSH->getSurfaceFromBuffer();
        graphic = LX_Graphics::loadTextureFromSurface(tmp,0);
    }

    SDL_FreeSurface(tmp);
}


