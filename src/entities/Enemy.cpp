

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
#include "../game/Game.h"

Missile * Enemy::shoot(MISSILE_TYPE m_type)
{
    //std::cout << "enum : " << m_type << "\nlist : "<< MISSILE_TYPE::BASIC_MISSILE_TYPE << std::endl;
    SDL_Rect pos_mis;   //the missiles position
    Speed sp_mis;       // the missiles speed

    pos_mis.x = position.x - (position.w/2);


    switch(m_type)
    {
        case MISSILE_TYPE::BASIC_MISSILE_TYPE :
        {
            pos_mis.y = position.y + ( (position.h - MISSILE_HEIGHT)/ 2);

            pos_mis.w = MISSIlE_WIDTH;
            pos_mis.h = MISSILE_HEIGHT;
            sp_mis = {-MISSILE_SPEED,0};

            return ( new Basic_missile(attack_val, LX_graphics::load_image("image/missile2.png"),NULL,&pos_mis,&sp_mis) );

        }break;


        case MISSILE_TYPE::ROCKET_TYPE : // rocket
        {
            pos_mis.y = position.y + ( (position.h - ROCKET_HEIGHT)/ 2);

            pos_mis.w = ROCKET_WIDTH;
            pos_mis.h = ROCKET_HEIGHT;
            sp_mis = {-ROCKET_SPEED,0};

            return ( new Rocket(attack_val, LX_graphics::load_image("image/rocket_TX2.png"),NULL,&pos_mis,&sp_mis) );

        }break;


        case MISSILE_TYPE::LASER_TYPE : // laser
        {
            pos_mis.y = position.y + ( (position.h - LASER_HEIGHT)/ 2);

            pos_mis.w = LASER_WIDTH;
            pos_mis.h = LASER_HEIGHT;
            sp_mis = {-LASER_SPEED,0};

            return ( new Laser(attack_val, LX_graphics::load_image("image/laser2.png"),NULL,&pos_mis,&sp_mis) );

        }break;


        case MISSILE_TYPE::BOMB_TYPE : // bomb
        {
            pos_mis.y = position.y + ( (position.h - BOMB_HEIGHT)/ 2);

            pos_mis.w = BOMB_WIDTH;
            pos_mis.h = BOMB_HEIGHT;
            sp_mis = {-BOMB_SPEED,0};

            return ( new Bomb(attack_val, LX_graphics::load_image("image/bomb2.png"),NULL,&pos_mis,&sp_mis) );

        }break;

        default : return NULL;
    }

    return NULL;
}



void Enemy::move()
{
    position.x += speed.speed_X;
    position.y += speed.speed_Y;

    box.x += speed.speed_X;
    box.y += speed.speed_Y;
}


void Enemy::strategy()
{

    if(still_alive)
    {
        cur_time = SDL_GetTicks();

        if(  cur_time - reference_time >= DELAY_ENEMY_MISSILE)
        {
            reference_time = cur_time;

            Game *tmp = Game::getInstance();
            tmp->addEnemyMissile(shoot(MISSILE_TYPE::BASIC_MISSILE_TYPE));
        }

        move();
    }

}



















