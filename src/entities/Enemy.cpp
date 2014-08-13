

/*
*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*
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






















