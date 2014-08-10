

/*
*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*   TX_physics is a library defined originaly for Target Xplosion.
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*
*/


/**
*	@file TX_Point.h
*	@brief The TX_Point file
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 1.0
*	@date July 14th, 2014
*
*/

#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include<cstdio>
#include<new>

/**
*   @struct TX_Point
*   @brief The point used by these followwing class : Circle, Rect_Hitbox, Polygon
*
*   This point is described by the x and y field
*/
typedef struct TX_Point{

    int x;  /**<The x position of the point*/
    int y;  /**<The y position of the point*/

}TX_Point; /**< */

#endif










