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


#define MISSILE_SPEED 16
#define MISSIlE_WIDTH 32
#define MISSILE_HEIGHT 12

#define HOMING_MISSILE_SPEED
#define HOMING_MISSILE_WIDTH
#define HOMING_MISSILE_HEIGHT

#define LASER_SPEED
#define LASER_WIDTH
#define LASER_HEIGHT

#define BOMB_SPEED
#define BOMB_WIDTH
#define BOMB_HEIGHT


typedef enum MISSILE_TYPE {BASIC_MISSILE_TYPE=0,HOMING_MISSILE_TYPE,LASER_TYPE,BOMB_TYPE} MISSILE_TYPE;

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

    TX_AABB missile_box;


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

    TX_AABB * get_hitbox(){return &missile_box;}

    virtual ~Missile(){}

};














#endif // MISSILE_H_INCLUDED













