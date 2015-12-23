
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

#include "Item.hpp"
#include "../xml/XMLReader.hpp"
#include "../game/Game.hpp"
#include "../game/Rank.hpp"
#include "../pattern/BulletPattern.hpp"

#include <LunatiX/LX_Physics.hpp>

using namespace LX_Random;
using namespace LX_Physics;

static SDL_Surface *itemSurface[NB_ITEMS];


Item::Item()
    : Entity(nullptr,nullptr,XPOS,
             static_cast<int>(xorshiftRand100()*6),
             ITEM_W,ITEM_H,XVEL,YVEL)
{
    const unsigned int r = Rank::getRank();
    int rand_val = static_cast<int>(xorshiftRand100());

    if(rand_val <= POWER_UP::NO_POWER_UP)
    {
        bonus = POWER_UP::NO_POWER_UP;
    }
    else if(rand_val <= POWER_UP::HEALTH)
    {
        bonus = POWER_UP::HEALTH;
        graphic = LX_Graphics::loadTextureFromSurface(itemSurface[0]);
    }
    else if(rand_val <= POWER_UP::SHIELD)
    {
        bonus = POWER_UP::SHIELD;
        graphic = LX_Graphics::loadTextureFromSurface(itemSurface[1]);
    }
    else if(rand_val <= POWER_UP::ROCKET && r == S_RANK)
    {
        bonus = POWER_UP::ROCKET;
        graphic = LX_Graphics::loadTextureFromSurface(itemSurface[2]);
    }
    else if(rand_val <= POWER_UP::BOMB && r == S_RANK)
    {
        bonus = POWER_UP::BOMB;
        graphic = LX_Graphics::loadTextureFromSurface(itemSurface[3]);
    }
    else if(rand_val <= POWER_UP::LASER && r == S_RANK)
    {
        bonus = POWER_UP::LASER;
        graphic = LX_Graphics::loadTextureFromSurface(itemSurface[4]);
    }
    else
    {
        bonus = POWER_UP::NO_POWER_UP;
    }

    aabb = {(Sint16)XPOS,(Sint16)position.y,ITEM_W,ITEM_H};
}


Item::Item(int x_pos, int y_pos)
    : Entity(nullptr,nullptr,x_pos,y_pos,ITEM_W-(ITEM_W/3),ITEM_H-(ITEM_W/3),
             XVEL_SCORE,0),
    bonus(POWER_UP::SCORE)
{
    graphic = LX_Graphics::loadTextureFromSurface(itemSurface[5]);
    aabb = {position.x,position.y,ITEM_W,ITEM_H};
}


Item::~Item()
{
    // Empty
}


void Item::createItemRessources()
{
    //const string *ITEMS = TX_Asset::getInstance()->getItemFiles();
    TX_Asset *asset = TX_Asset::getInstance();

    for(unsigned int i = 0; i< NB_ITEMS; i++)
    {
        itemSurface[i] = LX_Graphics::loadSurface(asset->getItemFile(i));
    }
}

void Item::destroyItemRessources()
{
    for(int i = 0; i< NB_ITEMS; i++)
    {
        SDL_FreeSurface(itemSurface[i]);
        itemSurface[i] = nullptr;
    }
}


void Item::move()
{
    const int xpos = position.x;
    const int ypos = position.y;
    const int game_xlim = Game::game_Xlimit;

    if(bonus != POWER_UP::NO_POWER_UP)
    {
        if(bonus == POWER_UP::SCORE)
        {
            const int lastx = Player::last_position.x;
            const float xlim = static_cast<float>(game_xlim);
            const int pos_to_get = static_cast<int>(xlim/2.5f);

            if(lastx > pos_to_get && xpos < (lastx - (position.w*2)))
                BulletPattern::shotOnPlayer(position.x,position.y,
                                            VEL_SCORE_ITEM,speed);
            else
            {
                speed.vx = VEL_SCORE_ITEM/6;
                speed.vy = 0;
            }
        }
        else
        {
            if(ypos > (Game::game_Ylimit - position.h)|| ypos < 0)
                speed.vy = -speed.vy;
        }

        moveRect(position,speed);
        moveRect(aabb,speed);
    }
}


SDL_Rect * Item::box()
{
    return &aabb;
}


POWER_UP Item::getPowerUp() const
{
    return bonus;
}

