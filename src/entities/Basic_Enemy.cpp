

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
*	@file
*	@brief
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version
*	@date
*
*
*
*/

#include "Basic_Enemy.h"



Missile * Basic_Enemy::shoot(MISSILE_TYPE m_type)
{

    SDL_Rect pos_mis;   // the missiles position
    Speed sp_mis;       // the missiles speed


    switch(m_type)
    {
        case MISSILE_TYPE::BASIC_MISSILE_TYPE :
        {
            pos_mis.x = position.x + ( (position.w - MISSILE_HEIGHT)/ 2);
            pos_mis.y = position.y + ( (position.h - MISSILE_HEIGHT)/ 2);

            pos_mis.w = MISSIlE_WIDTH;
            pos_mis.h = MISSILE_HEIGHT;
            sp_mis = {-MISSILE_SPEED,0};

            return ( new Basic_missile(attack_val, LX_graphics::load_image("image/missile2.png"),NULL,&pos_mis,&sp_mis) );

        }break;


        case MISSILE_TYPE::ROCKET_TYPE : // rocket
        {
            pos_mis.x = position.x + ( (position.w - ROCKET_HEIGHT)/ 2);
            pos_mis.y = position.y + ( (position.h - ROCKET_HEIGHT)/ 2);

            pos_mis.w = ROCKET_WIDTH;
            pos_mis.h = ROCKET_HEIGHT;
            sp_mis = {-ROCKET_SPEED,0};

            return ( new Rocket(attack_val, LX_graphics::load_image("image/rocket_TX2.png"),NULL,&pos_mis,&sp_mis) );

        }break;


        case MISSILE_TYPE::LASER_TYPE : // laser
        {
            pos_mis.x = position.x + ( (position.w - LASER_HEIGHT)/ 2);
            pos_mis.y = position.y + ( (position.h - LASER_HEIGHT)/ 2);

            pos_mis.w = LASER_WIDTH;
            pos_mis.h = LASER_HEIGHT;
            sp_mis = {-LASER_SPEED,0};

            return ( new Laser(attack_val, LX_graphics::load_image("image/laser2.png"),NULL,&pos_mis,&sp_mis) );

        }break;


        case MISSILE_TYPE::BOMB_TYPE : // bomb
        {
            pos_mis.x = position.x + ( (position.w - BOMB_HEIGHT)/ 2);
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
