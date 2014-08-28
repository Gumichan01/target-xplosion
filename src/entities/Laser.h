#ifndef LASER_H_INCLUDED
#define LASER_H_INCLUDED



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
*	@file Laser.h
*	@brief The Laser class
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date August 27th, 2014
*
*
*
*/

#include "Missile.h"


class Laser: public Missile
{

    public:

    Laser(unsigned int pow, SDL_Surface *image, Mix_Chunk *audio,int x, int y, int w, int h,int dX, int dY)
        : Missile(pow, 3, image, audio, x, y, w, h, dX, dY)
    {

    }

    Laser(unsigned int pow, SDL_Surface *image, Mix_Chunk *audio,SDL_Rect *rect,Speed *sp)
        : Missile(pow, 3, image, audio, rect, sp)
    {

    }


    ~Laser(){}

};





















#endif // LASER_H_INCLUDED















