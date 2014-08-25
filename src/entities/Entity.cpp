

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

/**
* DOC
*
*
* @todo move doit être virtuelle (abstraction)
*
*/
void Entity::move()
{



    //hitbox->move(speed.speed_X, speed.speed_Y);

    /*Point *p = hitbox->getPoint();

    hitbox->setPoint( p->x + speed.speed_X , p->y + speed.speed_Y);*/

}


/**
* DOC
*
*
*/
void Entity::setX(int newX)
{
    position.x = newX;
}


/**
* DOC
*
*
*/
void Entity::setY(int newY)
{
    position.y = newY;
}


/**
* DOC
*
*
*/
void Entity::set_Xvel(int Xvel)
{
    speed.speed_X = Xvel;
}

/**
* DOC
*
*
*/
void Entity::set_Yvel(int Yvel)
{
    speed.speed_Y = Yvel;
}

/**
* DOC
*
*
*/
SDL_Surface * Entity::getSurface()
{
    return graphic;
}

/**
* DOC
*
*
*/
SDL_Rect * Entity::getPos()
{
    return &position;
}

/**
* DOC
*
*
*/
void Entity::die()
{
    still_alive = false;
}

/**
* DOC
*
*
*/
bool Entity::isDead()
{
    return !still_alive ;
}












