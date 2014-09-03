

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
*	@file Entity.cpp
*	@brief The Entity file
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date July 23th, 2014
*
*
*/

#include "Entity.h"



void Entity::setX(int newX)
{
    position.x = newX;
}



void Entity::setY(int newY)
{
    position.y = newY;
}



void Entity::set_Xvel(int Xvel)
{
    speed.speed_X = Xvel;
}



void Entity::set_Yvel(int Yvel)
{
    speed.speed_Y = Yvel;
}



SDL_Surface * Entity::getSurface()
{
    return graphic;
}



SDL_Rect * Entity::getPos()
{
    return &position;
}



void Entity::die()
{
    still_alive = false;
}



bool Entity::isDead()
{
    return !still_alive ;
}












