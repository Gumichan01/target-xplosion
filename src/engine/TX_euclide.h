#ifndef TX_EUCLIDE_H_INCLUDED
#define TX_EUCLIDE_H_INCLUDED



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
*	@file TX_euclide.h
*	@brief the library which include different non-standard mathemetic operation
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 1.0
*	@date July 28th, 2014
*
*
*
*/

#include "Point.h"
#include <cmath>

/**
*
*	@fn int euclide_square_distance( int x1, int y1, int x2, int y2)
*
*	This function calculates the Euclide square distance
*
*	@param	x1 The x location of object no 1
*   @param  y1 The y location of object no 1
*	@param	x2 The x location of object no 2
*   @param	y2 The y location of object no 2
*
*	@return an integer value
*
*/
int euclide_square_distance( int x1, int y1, int x2, int y2)
{
    return( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}


/**
*
*	@fn int euclide_distance( int x1, int y1, int x2, int y2)
*
*	This function calculates the Euclide distance
*
*	@param	x1 The x location of object no 1
*   @param  y1 The y location of object no 1
*	@param	x2 The x location of object no 2
*   @param	y2 The y location of object no 2
*
*	@return an integer value
*
*/
int euclide_distance( int x1, int y1, int x2, int y2)
{
    return sqrt(euclide_square_distance(x1,y1,x2,y2));
}


/**
*
*	@fn int euclide_square_distance( Point *p1, Point *p2)
*
*	This function calculates the Euclide square distance
*
*	@param	p1 The point no 1
*   @param	p2 The point no 2
*
*	@return an integer value
*
*/
int euclide_square_distance( Point *p1, Point *p2)
{
    return euclide_square_distance(p1->x,p1->y,p2->x,p2->y);
}


/**
*
*	@fn int euclide_distance( Point *p1, Point *p2)
*
*	This function calculates the Euclide square distance
*
*	@param	p1 The point no 1
*   @param	p2 The point no 2
*
*	@return an integer value
*
*/
int euclide_distance( Point *p1, Point *p2)
{
    return sqrt(euclide_square_distance(p1,p2));
}


#endif // TX_EUCLIDE_H_INCLUDED











