

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


#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED


/**
*	@file Item.h
*	@brief The Item class
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Entity.hpp"



typedef enum POWER_UP {NO_POWER_UP=20,HEALTH=40,
                       SHIELD=50,ROCKET_BONUS=60,BOMB_BONUS=90,LASER_BONUS=100
                      } POWER_UP;

#define POWER_UP_NUMBER 7

// Item position
#define XPOS 1600   // X absolute position
#define YPOS 600    // Y relative position

// Item dimension
#define ITEM_W 47
#define ITEM_H 47

// X velocity
#define XVEL -1


class Item: public Entity
{

    POWER_UP bonus;
    LX_AABB aabb;

public :

    Item();

    POWER_UP getPowerUp();
    void move();
    LX_AABB * box();

    ~Item() {}

};

#endif // ITEM_H_INCLUDED


