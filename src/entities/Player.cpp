

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
*	@file Player.cpp
*	@brief The Player file
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date July 26th, 2014
*
*/

#include "Player.h"

/**
* DOC
*
*
*/
void Player::init_hitbox(int x, int y, int w, int h)
{
        int xCenter = x + ( ( (x + w) - x ) /2 );
        int yCenter = y + ( ( (y + h) - y ) /2 );

        int rad = xCenter - x;
        int square_rad = rad*rad;

        hitbox = {xCenter, yCenter, rad, square_rad};
}


/**
* DOC
*
*
*/
Missile * Player::shoot(MISSILE_TYPE m_type)
{
    //std::cout << "enum : " << m_type << "\nlist : "<< MISSILE_TYPE::BASIC_MISSILE_TYPE << std::endl;

    SDL_Rect pos_mis; //the missiles position
    Speed sp_mis; // the missiles speed

    // Setting data for the basic missile
    // for the other missile type, it will be set in each case

    pos_mis.x = position.x + (position.w/2);




    switch(m_type)
    {
        case MISSILE_TYPE::BASIC_MISSILE_TYPE :
        {
            pos_mis.y = position.y + ( (position.h - MISSILE_HEIGHT)/ 2);

            pos_mis.w = MISSIlE_WIDTH;
            pos_mis.h = MISSILE_HEIGHT;
            sp_mis = {MISSILE_SPEED,0};

            return ( new Basic_missile(attack_val, LX_graphics::load_image("image/missile.bmp"),NULL,&pos_mis,&sp_mis) );
        }
        break;


        case MISSILE_TYPE::ROCKET_TYPE : /// @todo homing missile class
        {
            std::cout << "DOUBLE" << std::endl;

            pos_mis.y = position.y + ( (position.h - ROCKET_HEIGHT)/ 2);

            pos_mis.w = ROCKET_WIDTH;
            pos_mis.h = ROCKET_HEIGHT;
            sp_mis = {ROCKET_SPEED,0};

            return ( new Rocket(attack_val, LX_graphics::load_image("image/rocket_TX.png"),NULL,&pos_mis,&sp_mis) );
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







































