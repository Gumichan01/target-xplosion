
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
*	@file Basic_Enemy.cpp
*	@brief The basic enemy implementaion
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Basic_Enemy.hpp"

#include "Basic_missile.hpp"
#include "Bomb.hpp"
#include "Rocket.hpp"
#include "Laser.hpp"

#include <LunatiX/LX_Graphics.hpp>


Basic_Enemy::Basic_Enemy(unsigned int hp, unsigned int att, unsigned int sh,
                         SDL_Texture *image, LX_Chunk *audio,
                         Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,dX,dY)
{
    strat = new Basic_strategy(this);
}


Basic_Enemy::Basic_Enemy(unsigned int hp, unsigned int att, unsigned int sh,
                         SDL_Texture *image, LX_Chunk *audio, SDL_Rect *rect,LX_Vector2D *sp)
    : Enemy(hp,att,sh,image,audio,rect,sp)
{
    strat = new Basic_strategy(this);
}


Missile * Basic_Enemy::shoot(MISSILE_TYPE m_type)
{

    SDL_Rect pos_mis;   // the missiles position
    LX_Vector2D sp_mis;       // the missiles speed


    switch(m_type)
    {
        case BASIC_MISSILE_TYPE :
        {
            pos_mis.x = position.x;
            pos_mis.y = position.y + ( (position.h - MISSILE_HEIGHT)/ 2);

            pos_mis.w = MISSIlE_WIDTH;
            pos_mis.h = MISSILE_HEIGHT;
            sp_mis = {-MISSILE_SPEED,0};

            return ( new Basic_missile(attack_val, LX_Graphics::loadTextureFromFile("image/shoot2.png",0),NULL,&pos_mis,&sp_mis) );

        }
        break;


        case ROCKET_TYPE : // rocket
        {
            pos_mis.x = position.x + ( (position.w - ROCKET_WIDTH)/ 2);
            pos_mis.y = position.y + ( (position.h - ROCKET_HEIGHT)/ 2);

            pos_mis.w = ROCKET_WIDTH;
            pos_mis.h = ROCKET_HEIGHT;
            sp_mis = {-ROCKET_SPEED,0};

            return ( new Rocket(attack_val, LX_Graphics::loadTextureFromFile("image/rocket_TX2.png",0),NULL,&pos_mis,&sp_mis) );

        }
        break;


        case LASER_TYPE : // laser
        {
            pos_mis.x = 0;
            pos_mis.y = position.y + ( (position.h - LASER_HEIGHT)/ 2);

            pos_mis.w = position.x;
            pos_mis.h = LASER_HEIGHT;
            sp_mis = {0,0};

            return ( new Laser(attack_val, LX_Graphics::loadTextureFromFile("image/laser2.png",0),NULL,&pos_mis,&sp_mis) );

        }
        break;


        case BOMB_TYPE : // bomb
        {
            pos_mis.x = position.x + ( (position.w - BOMB_HEIGHT)/ 2);
            pos_mis.y = position.y + ( (position.h - BOMB_HEIGHT)/ 2);

            pos_mis.w = BOMB_WIDTH;
            pos_mis.h = BOMB_HEIGHT;
            sp_mis = {-BOMB_SPEED,0};

            return ( new Bomb(attack_val, LX_Graphics::loadTextureFromFile("image/bomb2.png",0),NULL,&pos_mis,&sp_mis) );

        }
        break;

        default :
            return NULL;
    }

    return NULL;
}
