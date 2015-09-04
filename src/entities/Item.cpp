
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


/**
*	@file Item.cpp
*	@brief The Item implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <iostream>

#include <SDL2/SDL_surface.h>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Physics.hpp>

#include "Item.hpp"
#include "../xml/XMLReader.hpp"
#include "../game/Game.hpp"
#include "../level/Level.hpp"


using namespace LX_Random;
using namespace LX_Physics;
using namespace std;

static SDL_Surface *itemSurface[NB_ITEMS];

static const unsigned int LVL_MAX_NO_SCORE = 11;


Item::Item()
    : Entity(NULL,NULL,XPOS, xorshiftRand100()*6,ITEM_W,ITEM_H,XVEL,YVEL)
{
    int rand_val = xorshiftRand100();
    const unsigned int NUM_LVL = Level::getLevelNum();

    if(rand_val <= POWER_UP::NO_POWER_UP)
    {
        bonus = POWER_UP::NO_POWER_UP;
    }
    else if(rand_val <= POWER_UP::SCORE && NUM_LVL > LVL_MAX_NO_SCORE)
    {
        bonus = POWER_UP::SCORE;
        graphic = LX_Graphics::loadTextureFromSurface(itemSurface[5],0);
    }
    else if(rand_val <= POWER_UP::HEALTH)
    {
        bonus = POWER_UP::HEALTH;
        graphic = LX_Graphics::loadTextureFromSurface(itemSurface[0],0);
    }
    else if(rand_val <= POWER_UP::SHIELD)
    {
        bonus = POWER_UP::SHIELD;
        graphic = LX_Graphics::loadTextureFromSurface(itemSurface[1],0);
    }
    else if(rand_val <= POWER_UP::ROCKET && (NUM_LVL == 0
            || NUM_LVL >= ROCKET_LEVEL_MIN))
    {
        bonus = POWER_UP::ROCKET;
        graphic = LX_Graphics::loadTextureFromSurface(itemSurface[2],0);
    }
    else if(rand_val <= POWER_UP::BOMB && (NUM_LVL == 0
                                           || NUM_LVL >= BOMB_LEVEL_MIN))
    {
        bonus = POWER_UP::BOMB;
        graphic = LX_Graphics::loadTextureFromSurface(itemSurface[3],0);
    }
    else if(rand_val <= POWER_UP::LASER && (NUM_LVL == 0
                                            || NUM_LVL >= LASER_LEVEL_MIN))
    {
        bonus = POWER_UP::LASER;
        graphic = LX_Graphics::loadTextureFromSurface(itemSurface[4],0);
    }
    else
    {
        bonus = POWER_UP::NO_POWER_UP;
    }

    aabb = {(Sint16)XPOS,(Sint16)position.y,ITEM_W,ITEM_H};
}


Item::~Item()
{
    // Empty
}


void Item::createItemRessources()
{
    const string *ITEMS = TX_Asset::getInstance()->itemsFiles();

    memset(itemSurface,0,NB_ITEMS);

    for(int i = 0; i< NB_ITEMS; i++)
    {
        itemSurface[i] = LX_Graphics::loadSurface(ITEMS[i]);
    }
}

void Item::destroyItemRessources()
{
    for(int i = 0; i< NB_ITEMS; i++)
    {
        SDL_FreeSurface(itemSurface[i]);
    }
}


void Item::move()
{
    if(bonus != POWER_UP::NO_POWER_UP)
    {
        if(position.y > (Game::game_Ylimit - position.h - 1) || position.y < 0)
        {
            speed.vy = -speed.vy;
        }

        moveRect(&position,&speed);
        moveRect(&aabb,&speed);
    }
}


LX_AABB * Item::box()
{
    return &aabb;
}


POWER_UP Item::getPowerUp()
{
    return bonus;
}

