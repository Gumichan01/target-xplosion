

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
*	@file Character.cpp
*	@brief The charcater file
*	@author Luxon Jean-Pierre (Gumichan01)
*	@version 0.1
*	@date July 7th, 2014
*
*
*
*/

#include "Character.h"


/**
* DOC
*
*
*/
void Character::receive_damages(unsigned int attacks)
{

    if(health_point != 0)           // if the character's health is not equal to 0, he may take damages
    {
        if(shield > attacks)        // if the character's shield is better
            health_point -= 1;      // he takes a symbolic one damage point
        else
        {
            if( (attacks - shield) < health_point )         // If the character has more health point than damage
                health_point -= (attacks - shield) + 1;     // Decrease the health point
            else
                health_point = 0;                           // One shoot
        }

        if(health_point == 0)
        {
            die();
        }

    }

}

/**
*   DOC
*
*
*/
void Character::setHP(unsigned int newHP)
{
        health_point = newHP;
}












