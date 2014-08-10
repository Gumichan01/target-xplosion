

#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED



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
*
*	@file TX_physics.h
*	@brief The TX_physics Library, it will be linked with the SDL library
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 1.0
*	@date July 14th, 2014
*
*/

#include "Hitbox.h"
#include "TX_Polygon.h"

#define RECT_SIDES 4    /**<The number of the AABB sides*/

// todo : gravity, wind, strengh, friction

/**
*
*   @class TX_physics
*   @brief The Target_Xplosion physic engine
*
*   This class describes the TX_physics, a physics engine used in Target Xplosion. It mainly calculates collisions so far.
*
*/
class TX_physics{

    public :

        TX_physics(){}

        int euclide_square_distance( int x1, int y1, int x2, int y2);
        int euclide_distance( int x1, int y1, int x2, int y2);

        bool collision(int x_pos, int y_pos, const TX_AABB *rect);                          // collision point/AABB
        bool collision(int x_pos, int y_pos, const TX_Circle *circle);                      // collision point/circle
        bool collision(const TX_AABB *rect1, const TX_AABB *rect2);                         // collision AABB/AABB
        bool collision(const TX_Circle *circle1, const TX_Circle *circle2);                 // collision circle/circle
        bool collision(const TX_Circle *circle, const TX_Point *A, const TX_Point *B);      // collision circle/segment
        bool collision(const TX_Circle *circle, const TX_AABB *rect);                       // collision circle/AABB
        bool collision(const TX_Circle *circle, TX_Polygon *polygon);                       // collision circle/polygon

        ~TX_physics(){}
};





#endif // PHYSICS_H_INCLUDED































