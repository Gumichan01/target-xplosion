

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


#ifndef MISSILE_H_INCLUDED
#define MISSILE_H_INCLUDED


/**
*	@file Missile.h
*	@brief The missile class
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date August 1st, 2014
*
*
*
*/


#include "Entity.h"


#define MISSIlE_WIDTH 32
#define MISSILE_HEIGHT 12
#define MISSILE_SPEED 32

#define ROCKET_WIDTH 32
#define ROCKET_HEIGHT 16
#define ROCKET_SPEED 24

#define LASER_WIDTH 128
#define LASER_HEIGHT 47
#define LASER_SPEED 64

#define BOMB_WIDTH 47
#define BOMB_HEIGHT 47
#define BOMB_SPEED 12

#define MEGA_MISSILE_WIDTH 128
#define MEGA_MISSILE_HEIGHT 64
#define MEGA_MISSILE_SPEED 9



typedef enum MISSILE_TYPE{BASIC_MISSILE_TYPE=0,ROCKET_TYPE,LASER_TYPE,BOMB_TYPE,MEGA_MISSILE_TYPE} MISSILE_TYPE;


class Missile : public Entity
{
    protected:

    unsigned int power;         /**<The power of the missile*/
    unsigned int multiplier;    /**<The multiplier of the power, it depends on the type of the missile*/

    LX_AABB missile_box;


    public :


    Missile(unsigned int pow,unsigned int mul, SDL_Texture *image, LX_Chunk *audio,
                Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
        : Entity(image,audio,x, y, w, h,dX, dY)
    {
        power = pow;
        multiplier = mul;

        missile_box = {x,y,w,h};
    }


    Missile(unsigned int pow,unsigned int mul, SDL_Texture *image,
                LX_Chunk *audio, SDL_Rect *rect,Speed *sp)
        : Entity(image, audio, rect, sp)
    {
        power = pow;
        multiplier = mul;

        missile_box = {rect->x,rect->y,rect->w,rect->h};
    }

    unsigned int put_damages();
    virtual void move();

    LX_AABB * get_hitbox(){return &missile_box;}

    virtual ~Missile(){}

};














#endif // MISSILE_H_INCLUDED













