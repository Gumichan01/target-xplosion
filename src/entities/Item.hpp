

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2015  Luxon Jean-Pierre
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
*	@file Item.hpp
*	@brief The Item class
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Entity.hpp"


typedef enum POWER_UP {SCORE=0,NO_POWER_UP=15,HEALTH=35,
                       SHIELD=55,ROCKET=70,BOMB=90,LASER=100
                      } POWER_UP;

#define POWER_UP_NUMBER 7

// Item position
#define XPOS 1600   // X absolute position
#define YPOS 600    // Y relative position

// Item dimension
#define ITEM_W 47
#define ITEM_H 47

// X velocity
#define XVEL -4
#define YVEL -2


class Item: public Entity
{
    POWER_UP bonus;
    LX_AABB aabb;

public :

    Item();
    Item(int x_pos, int y_pos);

    static void createItemRessources();
    static void destroyItemRessources();

    POWER_UP getPowerUp();
    void move();
    LX_AABB * box();

    ~Item();

};

#endif // ITEM_H_INCLUDED


