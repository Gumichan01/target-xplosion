

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2014  Luxon Jean-Pierre
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Luxon Jean-Pierre (Gumichan01)
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
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

#include "Character.hpp"


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
            kill();
        }

    }

}


void Character::setHP(unsigned int newHP)
{
        health_point = newHP;
}


void Character::kill(void)
{
    wasKilled = true;
    die();
}










