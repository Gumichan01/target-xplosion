
/*
*	Target_Xplosion - A classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
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
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/


/**
*	@file Item.cpp
*	@brief The Item implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <iostream>

#include <SDL2/SDL_render.h>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Random.hpp>

#include "Item.hpp"
#include "../asset/TX_Asset.hpp"
#include "../entities/Player.hpp"
#include "../game/Rank.hpp"
#include "../game/Framerate.hpp"
#include "../pattern/BulletPattern.hpp"

using namespace LX_Random;
using namespace LX_Physics;

static LX_Graphics::LX_Sprite *item_texture[NB_ITEMS];
const float ITEM_XLIMIT = 1600.0f;
const float ITEM_YLIMIT = 768.0f;


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
        graphic = item_texture[0];
    }
    else if(rand_val <= POWER_UP::SHIELD)
    {
        bonus = POWER_UP::SHIELD;
        graphic = item_texture[1];
    }
    else if(rand_val <= POWER_UP::ROCKET && r == S_RANK)
    {
        bonus = POWER_UP::ROCKET;
        graphic = item_texture[2];
    }
    else if(rand_val <= POWER_UP::BOMB && r == S_RANK)
    {
        bonus = POWER_UP::BOMB;
        graphic = item_texture[3];
    }
    else if(rand_val <= POWER_UP::LASER && r == S_RANK)
    {
        bonus = POWER_UP::LASER;
        graphic = item_texture[4];
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
    graphic = item_texture[5];
    aabb = {position.x,position.y,ITEM_W,ITEM_H};
}


Item::~Item()
{
    // Empty
}


void Item::createItemRessources()
{
    TX_Asset *asset = TX_Asset::getInstance();
    LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(0);

    for(unsigned int i = 0; i < NB_ITEMS; i++)
    {
        item_texture[i] = new LX_Graphics::LX_Sprite(asset->getItemFile(i),*w);
    }
}

void Item::destroyItemRessources()
{
    for(int i = 0; i < NB_ITEMS; i++)
    {
        delete item_texture[i];
        item_texture[i] = nullptr;
    }
}


void Item::move()
{
    const int xpos = position.x;
    const int ypos = position.y;

    if(bonus != POWER_UP::NO_POWER_UP)
    {
        if(bonus == POWER_UP::SCORE)
        {
            Player::accept(this);
            const int pos_to_get = static_cast<int>(ITEM_XLIMIT/2.5f);

            if(last_player_x > pos_to_get && xpos < (last_player_x - (position.w*2)))
                BulletPattern::shotOnTarget(position.x,position.y,
                                            last_player_x, last_player_y,
                                            VEL_SCORE_ITEM,speed);
            else
            {
                speed.vx = VEL_SCORE_ITEM/6;
                speed.vy = 0;
            }
        }
        else
        {
            if(ypos > (ITEM_YLIMIT - position.h)|| ypos < 0)
                speed.vy = -speed.vy;
        }

        moveRect(position,speed);
        moveRect(aabb,speed);
    }
}


const SDL_Rect& Item::box()
{
    return aabb;
}


POWER_UP Item::getPowerUp() const
{
    return bonus;
}

