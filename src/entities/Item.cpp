

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
*	@file Item.cpp
*	@brief The Item implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.2
*	@date September 21st, 2014
*
*
*/

#include "Item.h"
#include "../engine/LX_graphics.h"


Item::Item()
    : Entity(NULL,NULL,XPOS, random100()*6,ITEM_W,ITEM_H,XVEL,0)
{
    int rand_val = random100();

    if(rand_val <= POWER_UP::HEALTH_QUARTER)
    {
        bonus = POWER_UP::HEALTH_QUARTER;
        graphic = LX_graphics::load_image("image/item25.png");
        // load the health sample
    }
    else if(rand_val <= POWER_UP::HEALTH_HALF)
    {
        bonus = POWER_UP::HEALTH_HALF;
        graphic = LX_graphics::load_image("image/item50.png");
        // load the health sample
    }
    else if(rand_val <= POWER_UP::SHIELD)
    {
        bonus = POWER_UP::SHIELD;
        graphic = LX_graphics::load_image("image/itemSH.png");
    }
    else
    {
        bonus = POWER_UP::NO_POWER_UP;
    }

    aabb = {(Sint16)XPOS,(Sint16)position.y,ITEM_W,ITEM_H};
}


void Item::move()
{
    if(bonus != POWER_UP::NO_POWER_UP)
    {
        position.x += speed.speed_X;
        position.y += speed.speed_Y;

        aabb.x += speed.speed_X;
        aabb.y += speed.speed_Y;
    }
}

// Return the hitbox
LX_AABB * Item::box()
{
    return &aabb;
}

// return the power up
POWER_UP Item::getPowerUp()
{
    return bonus;
}














