

#ifndef HITBOX_H_INCLUDED
#define HITBOX_H_INCLUDED


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
*	@file TX_Hitbox.h
*	@brief The hitbox library
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 1.0
*	@date July 28th, 2014
*
*
*/

#include <SDL/SDL.h>

/*
typedef struct SDL_Rect{

    short x,y,w,h;

}TX_AABB;*/

typedef SDL_Rect TX_AABB;


/**
*   @struct TX_Circle
*
*   @brief The circle structure
*
*/
typedef struct TX_Circle{

    int xCenter;                /**<The x position of circle center*/
    int yCenter;                /**<The y position of circle center*/
    unsigned int radius;        /**<The circle radius*/
    unsigned int square_radius; /**<The square radius (for a collision optimization)*/



}TX_Circle;



#endif // HITBOX_H_INCLUDED






























