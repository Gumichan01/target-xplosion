

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
*	@file Enemy.cpp
*	@brief The enemy class
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date August 8th,2014
*
*
*
*/

#include "Enemy.h"

/**
* DOC
*
*
*/
Missile * Enemy::shoot(MISSILE_TYPE m_type)
{
    //std::cout << "enum : " << m_type << "\nlist : "<< MISSILE_TYPE::BASIC_MISSILE_TYPE << std::endl;
    SDL_Rect pos_mis; //the missiles position
    Speed sp_mis; // the missiles speed

    //Setting data for the basic missile
    // for the other missile type, it will be set in each case

    pos_mis.x = position.x;
    pos_mis.y = position.y + ( (position.h - MISSILE_HEIGHT)/ 2);
    sp_mis = {-MISSILE_SPEED,0};



    switch(m_type)
    {
        case MISSILE_TYPE::BASIC_MISSILE_TYPE :
        {
            pos_mis.w = MISSIlE_WIDTH;
            pos_mis.h = MISSILE_HEIGHT;

            return ( new Basic_missile(attack_val, LX_graphics::load_image("image/missile.png"),NULL,&pos_mis,&sp_mis) );
        }
        break;



        case MISSILE_TYPE::LASER_TYPE : /// @todo laser class
            break;



        case MISSILE_TYPE::BOMB_TYPE : /// @todo bomb class
            break;

        default : return NULL;
    }

    return NULL;
}


/**
* DOC
*
*
*/
void Enemy::move()
{
    /// @todo implementation enemy
    position.x += speed.speed_X;
    position.y += speed.speed_Y;

    box.x += speed.speed_X;
    box.y += speed.speed_Y;
}






















