

/*
*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*   TX_physics is a library defined originaly for Target Xplosion.
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*/


/**
*	@file TX_Polygon.cpp
*	@brief The TX_Polygon file
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 1.0
*	@date July 14th, 2014
*/


#include "TX_Polygon.h"

/*
*
*	@fn TX_Point * TX_Polygon::getTX_Points()
*
*	Returns the points of the polygon
*
*
*	@return the points array
*

TX_Point * TX_Polygon::getTX_Points()
{
    return list_points;
}*/


/**
*
*	@fn TX_Point * TX_Polygon::getTX_PointAt(unsigned int i)
*
*	Returns the point at the position specified
*
*   @param i : the position of the point in the array
*
*	@return the center
*/

TX_Point * TX_Polygon::getTX_PointAt(unsigned int i)
{
    if (i <= cursor)
    {
        return &list_points[i];
    }

    return NULL;

}

/**
*
*	@fn unsigned int TX_Polygon::getSize()
*
*	returns the size of the array
*
*	@return the size
*/

unsigned int TX_Polygon::getSize()
{
    return cursor;
}

/**
*
*	@fn bool TX_Polygon::putTX_Point(TX_Point *p)
*
*	Puts the point into the array
*
*   @param p : the point
*
*	@return TRUE if it is OK, FALSE otherwise
*/

bool TX_Polygon::putTX_Point(TX_Point *p)
{
    return putTX_Point(p->x, p->y);
}


/**
*
*	@fn bool TX_Polygon::putTX_Point(int x, int y)
*
*	Puts the point into the array
*
*   @param x : the x position
*   @param y : the y position
*
*	@return TRUE if it is OK, FALSE otherwise
*/

bool TX_Polygon::putTX_Point(int x, int y)
{

    if(cursor < size)
    {
        list_points[cursor].x = x;
        list_points[cursor].y = y;
        cursor++;
        return true;
    }

    return false;

}





