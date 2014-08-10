#ifndef POLY_HITBOX_H_INCLUDED
#define POLY_HITBOX_H_INCLUDED



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
*	@file TX_Polygon.h
*	@brief The polygon hitbox Library
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 1.0
*	@date July 14th, 2014
*
*/

#include<cstdlib>
#include<new>

#include "TX_Point.h"

/**
*
*   @class TX_Polygon
*   @brief The TX_Polygon hitbox
*
*   This class describes the polygon hitbox
*/
class TX_Polygon{

        TX_Point *list_points;     /**<The list of all the points of the polygon*/
        unsigned int size;      /**<The size of the list*/
        unsigned int cursor;    /**<The curssor which keep the information of the position of the first available position*/

    public :

/**
*
*	@fn TX_Polygon(int nbTX_Points)
*
*	The TX_Polygon constructor
*
*   @param nbTX_Points : the points number
*
*/
        TX_Polygon(unsigned int nbTX_Points)
        {
            list_points = new (std::nothrow) TX_Point[nbTX_Points];

            if(list_points == NULL)
            {
                perror("Error occured in the Poly_Hit_box constructor while creating the points list ");
                delete this;
            }

            size = nbTX_Points;
            cursor = 0;
        }

        //TX_Point * getTX_Points();
        TX_Point * getTX_PointAt(unsigned int i);
        unsigned int getSize();
        bool putTX_Point(TX_Point *p);
        bool putTX_Point(int a, int b);


        ~TX_Polygon()
        {
            delete [] list_points;
        }

};

#endif // POLY_HITBOX_H_INCLUDED


