

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
*	@file Bomb.cpp
*	@brief The bomb class definition
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date August 28th, 2014
*
*/

#include "Bomb.h"
#include <iostream>


void Bomb::move()
{
    //if the bomb has not life time anymore and have not been exploded
    if(lifetime == 0 && !explosion)
    {
        die();
    }
    else
    {
        if(lifetime > 0)
            lifetime -= 1;
        else
            Missile::die();

        if(!explosion)
        {
            Missile::move();
        }
    }
}


void Bomb::die()
{
    std::cout << "DIE BOMB" << std::endl;
    still_alive = false;

    /// @todo set the explosion sprite
    explosion = true;
    lifetime =  BOMB_LIFETIME;
}
































