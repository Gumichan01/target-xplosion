#ifndef MISSILE_H_INCLUDED
#define MISSILE_H_INCLUDED



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


#define MISSILE_SPEED 32
#define MISSIlE_WIDTH 32
#define MISSILE_HEIGHT 12

#define ROCKET_SPEED 24
#define ROCKET_WIDTH 32
#define ROCKET_HEIGHT 16    /// @todo to modify -> create a sprite with GIMP DONE

#define LASER_SPEED 64
#define LASER_WIDTH 64
#define LASER_HEIGHT 32

#define BOMB_SPEED 12
#define BOMB_WIDTH 32
#define BOMB_HEIGHT 32

#define MEGA_MISSILE_SPEED 9
#define MEGA_MISSILE_WIDTH 128
#define MEGA_MISSILE_HEIGHT 64


typedef enum MISSILE_TYPE {BASIC_MISSILE_TYPE=0,ROCKET_TYPE,LASER_TYPE,BOMB_TYPE} MISSILE_TYPE;

/**
*
*   DOC
*
*/
class Missile : public Entity
{
    protected:

    unsigned int power;         /**<The power of the missile*/
    unsigned int multiplier;    /**<The multiplier of the power, it depends on the type of the missile*/

    LX_AABB missile_box;


    public :

/**
* DOC
*
*
*/
    Missile(unsigned int pow,unsigned int mul, SDL_Surface *image, Mix_Chunk *audio,
                Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
        : Entity(image,audio,x, y, w, h,dX, dY)
    {
        power = pow;
        multiplier = mul;

        missile_box = {x,y,w,h};
    }

/**
* DOC
*
*
*/
    Missile(unsigned int pow,unsigned int mul, SDL_Surface *image,
                Mix_Chunk *audio,SDL_Rect *rect,Speed *sp)
        : Entity(image, audio, rect, sp)
    {
        power = pow;
        multiplier = mul;

        missile_box = {rect->x,rect->y,rect->w,rect->h};
    }

    unsigned int put_damages();
    void move();

    LX_AABB * get_hitbox(){return &missile_box;}

    virtual ~Missile(){}

};














#endif // MISSILE_H_INCLUDED













