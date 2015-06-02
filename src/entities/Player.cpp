

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2014  Luxon Jean-Pierre
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


#include "Player.hpp"
#include "../game/Game.hpp"
#include "../engine/LX_Random.hpp"

#include "Basic_missile.hpp"
#include "Bomb.hpp"
#include "Rocket.hpp"
#include "Laser.hpp"

#include "../engine/LX_Sound.hpp"
#include "../engine/LX_Chunk.hpp"


using namespace LX_Random;


Player::Player(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic,
               SDL_Texture *image, LX_Chunk *audio,
               int x, int y, int w, int h,int dX, int dY, unsigned int w_limit, unsigned h_limit)
    : Character(hp, att, sh, image, audio, x, y, w, h, dX, dY)
{
    critical_rate = critic;
    nb_bomb = 0;
    nb_rocket = 0;
    shield = false;
    bomb_activated = false;
    rocket_activated = false;
    laser_activated = false;

    LIMIT_WIDTH = w_limit;
    LIMIT_HEIGHT = h_limit;

    display = new HUD(this);

    init_hitbox(x,y,w,h);
}


Player::Player(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic,
               SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp,
               unsigned int w_limit, unsigned h_limit)
    : Character(hp, att, sh, image, audio, rect, sp)
{
    critical_rate = critic;
    nb_bomb = 10;
    nb_rocket = 10;
    shield = false;
    bomb_activated = false;
    rocket_activated = false;
    laser_activated = false;

    LIMIT_WIDTH = w_limit;
    LIMIT_HEIGHT = h_limit;

    display = new HUD(this);

    init_hitbox(rect->x,rect->y,rect->w,rect->h);
}


Player::~Player()
{
    delete display;
}


void Player::receive_damages(unsigned int attacks)
{
    // Take less damages if the shied is activated
    if(shield == true)
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


// initialize the hitbox
void Player::init_hitbox(int x, int y, int w, int h)
{
    int xCenter = x + ( ( (x + w) - x ) /2 );
    int yCenter = y + ( ( (y + h) - y ) /2 );

    int rad = xCenter - x;
    int square_rad = rad*rad;

    hitbox = {xCenter, yCenter, rad, square_rad};
}


// create a new missile according to the type of the missile
Missile * Player::shoot(MISSILE_TYPE m_type)
{
    SDL_Rect pos_mis;   // the missiles position
    LX_Vector2D sp_mis;       // the missiles speed
    unsigned int bonus_att = 0;

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

            return (new Rocket(attack_val + bonus_att,
                               LX_Graphics::loadTextureFromFile("image/rocket_TX.png",0),
                               NULL,&pos_mis,&sp_mis));
        }
        break;


        case LASER_TYPE : // laser
        {
            pos_mis.y = position.y + ( (position.h - LASER_HEIGHT)/ 2);

            pos_mis.w = LASER_WIDTH;
            pos_mis.h = LASER_HEIGHT;
            sp_mis = {LASER_SPEED,0};

            return (new Laser(attack_val + bonus_att,
                              LX_Graphics::loadTextureFromFile("image/laser.png",0),
                              NULL,&pos_mis,&sp_mis));
        }
        break;


        case BOMB_TYPE : // bomb
        {
            pos_mis.y = position.y + ( (position.h - BOMB_HEIGHT)/ 2);

            pos_mis.w = BOMB_WIDTH;
            pos_mis.h = BOMB_HEIGHT;
            sp_mis = {BOMB_SPEED,0};

            return (new Bomb(attack_val + bonus_att,
                             LX_Graphics::loadTextureFromFile("image/bomb.png",0),
                             LX_Mixer::loadSample("sound/explosion.wav"),&pos_mis,&sp_mis));
        }
        break;

        default :
        {
            sound->play();
            return (new Basic_missile(attack_val + bonus_att,
                                      LX_Graphics::loadTextureFromFile("image/missile.png",0),
                                      NULL,&pos_mis,&sp_mis));
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
            if( laser_delay > 0 )
            {
                laser_delay -= 1;
                cur_game->addPlayerMissile(shoot(m_type));
            }
            else
            {
                laser_activated = false;
                laser_delay = LASER_LIFETIME;
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

        case ROCKET_TYPE : // bomb
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

        default :
            cur_game->addPlayerMissile(shoot(m_type));
            break;
    }
}


// manage the player's action (movement and shield)
void Player::move()
{
    position.x += speed.vx;
    hitbox.xCenter += speed.vx;

    // left or right
    if( (position.x <= 0) || ( (position.x + position.w) > Game::getGameW() ) )
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

    if(SDL_GetTicks() - shield_time > SHIELD_TIME)
    {
        set_shield(false);
    }

}

void Player::die()
{
    health_point = 0;
    Entity::die();
    display->update();
}


void Player::collision(Missile *mi)
{
    if(LX_Physics::collisionCircleRect(&hitbox,mi->get_hitbox()))
    {
        receive_damages(mi->put_damages());
        mi->die();
    }
}


void Player::takeBonus(POWER_UP powerUp)
{
    switch(powerUp)
    {
        case POWER_UP::HEALTH_QUARTER :
            healQuarter();
            break;

        case POWER_UP::HEALTH_HALF :
            healHalf();
            break;

        case POWER_UP::SHIELD :
            set_shield(true);
            break;

        default :
            break;
    }
}


void Player::healQuarter(void)
{
    if( (health_point + (health_point/QUARTER)) > max_health_point )
    {
        health_point = max_health_point;
    }
    else
    {
        health_point += health_point/QUARTER;
    }

    display->update();
}


void Player::healHalf(void)
{
    if( (health_point + (health_point/HALF)) > max_health_point )
    {
        health_point = max_health_point;
    }
    else
    {
        health_point += health_point/HALF;
    }

    display->update();
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
    if(sh == true)
    {
        shield = true;
        shield_time = SDL_GetTicks();
        nb_hits = HITS_UNDER_SHIELD;

        /// @todo set the ship with shield image
    }
    else
    {
        shield = false;
        /// @todo set the ship without shield image
    }

}


