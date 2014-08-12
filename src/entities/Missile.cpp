

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
*	@file Missile.cpp
*	@brief The Missile functions
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date August 1st, 2014
*
*
*/

#include "Missile.h"

/**
* DOC
*
*
*/
unsigned int Missile::put_damages()
{
    return(power*multiplier);
}


/**
* DOC
*
*
*/
void Missile::move()
{
    /// @todo missile implementation
    position.x += speed.speed_X;
    position.y += speed.speed_Y;

    missile_box.x += speed.speed_X;
    missile_box.y += speed.speed_Y;
}































