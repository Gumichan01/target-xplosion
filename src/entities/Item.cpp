
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
*
*/

#include "Item.hpp"
#include "../engine/LX_Graphics.hpp"
#include "../engine/LX_Random.hpp"
#include "../engine/LX_Physics.hpp"

using namespace LX_Random;
using namespace LX_Physics;


Item::Item()
    : Entity(NULL,NULL,XPOS, xorshiftRand100()*6,ITEM_W,ITEM_H,XVEL,0)
{
    int rand_val = xorshiftRand100();

    if(rand_val <= POWER_UP::NO_POWER_UP)
    {
        bonus = POWER_UP::NO_POWER_UP;
    }
    else if(rand_val <= POWER_UP::HEALTH)
    {
        bonus = POWER_UP::HEALTH;
        graphic = LX_Graphics::loadTextureFromFile("image/itemH.png",0);
        // load the health sample
    }
    else if(rand_val <= POWER_UP::SHIELD)
    {
        bonus = POWER_UP::SHIELD;
        graphic = LX_Graphics::loadTextureFromFile("image/itemSH.png",0);
    }
    else if(rand_val <= POWER_UP::BOMB)
    {
        bonus = POWER_UP::BOMB;
        graphic = LX_Graphics::loadTextureFromFile("image/itemB.png",0);
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
        moveRect(&position,&speed);
        moveRect(&aabb,&speed);
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














