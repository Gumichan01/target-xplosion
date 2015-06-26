

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


#ifndef MISSILE_H_INCLUDED
#define MISSILE_H_INCLUDED


/**
*	@file Missile.hpp
*	@brief The missile class
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/


#include "Entity.hpp"


#define MISSIlE_WIDTH 32
#define MISSILE_HEIGHT 12
#define MISSILE_SPEED 32
#define MISSILE_Y_SPEED 8

#define ROCKET_WIDTH 32
#define ROCKET_HEIGHT 16
#define ROCKET_SPEED 24

#define LASER_WIDTH 128
#define LASER_HEIGHT 47

#define BOMB_WIDTH 47
#define BOMB_HEIGHT 47
#define BOMB_SPEED 12


typedef enum MISSILE_TYPE {BASIC_MISSILE_TYPE=0,DOUBLE_MISSILE_TYPE,
                           ROCKET_TYPE,LASER_TYPE,BOMB_TYPE,
                          } MISSILE_TYPE;


class Missile : public Entity
{

protected:

    unsigned int power;         /* The power of the missile*/
    unsigned int multiplier;    /* The multiplier of the power, it depends on the type of the missile */

    LX_AABB missile_box;


public :

    Missile(unsigned int pow,unsigned int mul, SDL_Texture *image, LX_Chunk *audio,
            Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY);

    Missile(unsigned int pow,unsigned int mul, SDL_Texture *image,
            LX_Chunk *audio, SDL_Rect *rect,LX_Vector2D *sp);

    unsigned int put_damages();
    virtual void move();
    virtual void displayAdditionnalData() = 0;

    LX_AABB * get_hitbox();

    virtual ~Missile() {}

};


#endif // MISSILE_H_INCLUDED


