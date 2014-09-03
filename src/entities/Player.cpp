

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
*	@version 0.1
*	@date July 26th, 2014
*
*/

#include "Player.h"


void Player::init_hitbox(int x, int y, int w, int h)
{
        int xCenter = x + ( ( (x + w) - x ) /2 );
        int yCenter = y + ( ( (y + h) - y ) /2 );

        int rad = xCenter - x;
        int square_rad = rad*rad;

        hitbox = {xCenter, yCenter, rad, square_rad};
}



Missile * Player::shoot(MISSILE_TYPE m_type)
{

    SDL_Rect pos_mis;   //the missiles position
    Speed sp_mis;       // the missiles speed

    pos_mis.x = position.x + (position.w/2);


    switch(m_type)
    {
        case MISSILE_TYPE::BASIC_MISSILE_TYPE :
        {
            pos_mis.y = position.y + ( (position.h - MISSILE_HEIGHT)/ 2);

            pos_mis.w = MISSIlE_WIDTH;
            pos_mis.h = MISSILE_HEIGHT;
            sp_mis = {MISSILE_SPEED,0};

            return ( new Basic_missile(attack_val, LX_graphics::load_image("image/missile.png"),NULL,&pos_mis,&sp_mis) );

        }break;


        case MISSILE_TYPE::ROCKET_TYPE : // rocket
        {
            pos_mis.y = position.y + ( (position.h - ROCKET_HEIGHT)/ 2);

            pos_mis.w = ROCKET_WIDTH;
            pos_mis.h = ROCKET_HEIGHT;
            sp_mis = {ROCKET_SPEED,0};

            return ( new Rocket(attack_val, LX_graphics::load_image("image/rocket_TX.png"),NULL,&pos_mis,&sp_mis) );

        }break;


        case MISSILE_TYPE::LASER_TYPE : // laser
        {
            pos_mis.y = position.y + ( (position.h - LASER_HEIGHT)/ 2);

            pos_mis.w = LASER_WIDTH;
            pos_mis.h = LASER_HEIGHT;
            sp_mis = {LASER_SPEED,0};


            if( laser_delay > 0 )
            {
                laser_delay -= 1;
                return ( new Laser(attack_val, LX_graphics::load_image("image/laser.png"),NULL,&pos_mis,&sp_mis) );
            }
            else
            {
                laser_activated = false;
                laser_delay = LASER_LIFETIME;
                return NULL;
            }

        }break;


        case MISSILE_TYPE::BOMB_TYPE : // bomb
        {
            pos_mis.y = position.y + ( (position.h - BOMB_HEIGHT)/ 2);

            pos_mis.w = BOMB_WIDTH;
            pos_mis.h = BOMB_HEIGHT;
            sp_mis = {BOMB_SPEED,0};

            if( nb_bomb > 0 )
            {
                nb_bomb -= 1;
                return ( new Bomb(attack_val, LX_graphics::load_image("image/bomb.png"),NULL,&pos_mis,&sp_mis) );
            }
            else
            {
                return NULL;
            }

        }break;

        default : return NULL;
    }

    return NULL;
}



void Player::move()
{

    position.x += speed.speed_X;
    hitbox.xCenter += speed.speed_X;

    // left or right
    if( (position.x <= 0) || ( (position.x + position.w) > 1280 ) )
    {
        position.x -= speed.speed_X;
        hitbox.xCenter -= speed.speed_X;
    }



    position.y += speed.speed_Y;
    hitbox.yCenter += speed.speed_Y;

    //down or up
    if( (position.y  <= 0) || ((position.y + position.h) > LIMIT_HEIGHT )  )
    {
        position.y -= speed.speed_Y;
        hitbox.yCenter -= speed.speed_Y;
    }

}














