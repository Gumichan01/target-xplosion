#ifndef BASIC_MISSILE_H_INCLUDED
#define BASIC_MISSILE_H_INCLUDED



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
*	@file Basic_missile.h
*	@brief The Basic_missile class
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date August 1st, 2014
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
class Basic_missile: public Missile
{

    public:


/**
* DOC
*
*
*/
    Basic_missile(unsigned int pow, SDL_Surface *image, Mix_Chunk *audio,int x, int y, int w, int h,int dX, int dY)
        : Missile(pow, 1, image, audio, x, y, w, h, dX, dY)
    {
        //Nothing to do
    }


/**
* DOC
*
*
*/
    Basic_missile(unsigned int pow, SDL_Surface *image, Mix_Chunk *audio,SDL_Rect *rect,Speed *sp)
        : Missile(pow, 1, image, audio, rect, sp)
    {
        //Nothing to do
    }


    ~Basic_missile(){}

};

#endif // BASIC_MISSILE_H_INCLUDED











