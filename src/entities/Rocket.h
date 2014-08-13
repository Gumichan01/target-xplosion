#ifndef ROCKET_H_INCLUDED
#define ROCKET_H_INCLUDED



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
*	@file Rocket.h
*	@brief Rhe Rocket class
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date August 13th, 2014
*
*
*
*/

#include "Missile.h"

/**
* DOC
*
*
*/
class Rocket : public Missile
{

    public:


/**
* DOC
*
*
*/
    Rocket(unsigned int pow, SDL_Surface *image, Mix_Chunk *audio,int x, int y, int w, int h,int dX, int dY)
        : Missile(pow, 2, image, audio, x, y, w, h, dX, dY)
    {
        //Nothing to do
    }


/**
* DOC
*
*
*/
    Rocket(unsigned int pow, SDL_Surface *image, Mix_Chunk *audio,SDL_Rect *rect,Speed *sp)
        : Missile(pow, 2, image, audio, rect, sp)
    {
        //Nothing to do
    }


    ~Rocket(){}
};



















#endif // ROCKET_H_INCLUDED


















