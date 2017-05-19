
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017  Luxon Jean-Pierre
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "Item.hpp"
#include "../level/Level.hpp"
#include "../asset/TX_Asset.hpp"
#include "../entities/Player.hpp"
#include "../game/engine/Engine.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../resources/WinID.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Random.hpp>

using namespace LX_Random;
using namespace LX_Physics;

static LX_Graphics::LX_Sprite *item_texture[NB_ITEMS];


namespace
{
const int ITEM_XLIMIT = 1600;
const int ITEM_YLIMIT = 720;

// Random
const int RAND_MULT = 5;
const int RAND_OFFSET = 70;

// Item position
const int XPOS = 1600;              // X absolute position

// Item dimension
const int ITEM_W = 48;
const int ITEM_H = 48;

// Velocity
const float XVEL = -4.0f;
const float YVEL = -3.0f;

const float XVEL_SCORE = -2.0f;         // Default X velocity
const float VEL_SCORE_ITEM = -32.0f;    // Global velocity of the score item
};


Item::Item(): bonus(POWER_UP::NO_POWER_UP), aabb()
{
    int rand_val = static_cast<int>(xorshiftRand100());
    unsigned int lid = Level::getLevelNum();

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
    else if(rand_val <= POWER_UP::ROCKET && lid >= Level::ROCKET_LEVEL_MIN)
    {
        bonus = POWER_UP::ROCKET;
        graphic = item_texture[2];
    }
    else if(rand_val <= POWER_UP::BOMB && lid >= Level::BOMB_LEVEL_MIN)
    {
        bonus = POWER_UP::BOMB;
        graphic = item_texture[3];
    }
    else if(rand_val <= POWER_UP::LASER && lid >= Level::LASER_LEVEL_MIN)
    {
        bonus = POWER_UP::LASER;
        graphic = item_texture[4];
    }
    else
        bonus = POWER_UP::NO_POWER_UP;

    position = {XPOS, static_cast<int>(xorshiftRand100()*RAND_MULT + RAND_OFFSET), ITEM_W, ITEM_H};
    aabb = position;
    speed = LX_Vector2D(XVEL, YVEL);
}

// Create score items
Item::Item(int x_pos, int y_pos): Item(x_pos, y_pos, POWER_UP::SCORE) {}

// General Item creation
Item::Item(int x_pos, int y_pos, POWER_UP pup): bonus(pup)
{
    position = {x_pos, y_pos, ITEM_W, ITEM_H};

    switch(bonus)
    {
    case POWER_UP::HEALTH:
        graphic = item_texture[0];
        break;

    case POWER_UP::SHIELD:
        graphic = item_texture[1];
        break;

    case POWER_UP::ROCKET:
        graphic = item_texture[2];
        break;

    case POWER_UP::BOMB:
        graphic = item_texture[3];
        break;

    case POWER_UP::LASER:
        graphic = item_texture[4];
        break;

    case POWER_UP::SCORE:
        graphic = item_texture[5];
        position = {x_pos, y_pos, ITEM_W/2, ITEM_H/2};
        break;

    default:
        break;
    }

    aabb = position;
    speed = LX_Vector2D(XVEL_SCORE, 0.0f);
}


Item::~Item() {}


void Item::createItemRessources()
{
    const TX_Asset *asset = TX_Asset::getInstance();
    LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(WinID::getWinID());

    for(unsigned int i = 0; i < NB_ITEMS; i++)
    {
        item_texture[i] = new LX_Graphics::LX_Sprite(asset->getItemFile(i), *w);
    }
}

void Item::destroyItemRessources()
{
    for(unsigned long i = 0; i < NB_ITEMS; i++)
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
                BulletPattern::shotOnTarget(position.x, position.y,
                                            last_player_x, last_player_y,
                                            VEL_SCORE_ITEM, speed);
            else
            {
                speed.vx = VEL_SCORE_ITEM/6;
                speed.vy = 0;
            }
        }
        else
        {
            if(ypos > (ITEM_YLIMIT - position.w) || ypos < Engine::getMinYlim())
                speed.vy = -speed.vy;
        }

        moveRect(position, speed);
        moveRect(aabb, speed);
    }
}


const LX_AABB& Item::box() const
{
    return aabb;
}


const POWER_UP& Item::getPowerUp() const
{
    return bonus;
}
