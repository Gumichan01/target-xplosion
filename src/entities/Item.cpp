
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
#include "Player.hpp"
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
// Random
const int RAND_MULT = 5;
const int RAND_OFFSET = 70;

// Item position
const int XPOS = 1600;                  // X absolute position

// Item dimension
const int ITEM_W = 48;
const int ITEM_H = 48;

// Velocity
const float XVEL = -4.0f;
const float YVEL = -3.0f;

const float XVEL_SCORE = -5.0f;         // Default X velocity
const float VEL_SCORE_ITEM = -20.0f;    // Global velocity of the score item
const int VELF = static_cast<int>(VEL_SCORE_ITEM);
}

using namespace POWER_UP;

Item::Item(): bonus(NOPOW), aabb(), toplayer(false)
{
    int rand_val = static_cast<int>(xorshiftRand100());
    unsigned int lid = Level::getLevelNum();

    if(rand_val <= HEALTH)
    {
        bonus = HEALTH;
        graphic = item_texture[0];
    }
    else if(rand_val <= SHIELD)
    {
        bonus = SHIELD;
        graphic = item_texture[1];
    }
    else if(rand_val <= ROCKET && lid >= Level::ROCKET_LEVEL_MIN)
    {
        bonus = ROCKET;
        graphic = item_texture[2];
    }
    else if(rand_val <= BOMB && lid >= Level::BOMB_LEVEL_MIN)
    {
        bonus = BOMB;
        graphic = item_texture[3];
    }
    else if(rand_val <= LASER && lid >= Level::LASER_LEVEL_MIN)
    {
        bonus = LASER;
        graphic = item_texture[4];
    }
    else
        bonus = NOPOW;

    position = {XPOS, static_cast<int>(xorshiftRand100()*RAND_MULT + RAND_OFFSET), ITEM_W, ITEM_H};
    aabb = position;
    speed = LX_Vector2D(XVEL, YVEL);
}

// Create score items
Item::Item(int x_pos, int y_pos): Item(x_pos, y_pos, SCORE) {}

// General Item creation
Item::Item(int x_pos, int y_pos, ItemType pup): bonus(pup), toplayer(false)
{
    position = {x_pos, y_pos, ITEM_W, ITEM_H};

    switch(bonus)
    {
    case HEALTH:
        graphic = item_texture[0];
        break;

    case SHIELD:
        graphic = item_texture[1];
        break;

    case ROCKET:
        graphic = item_texture[2];
        break;

    case BOMB:
        graphic = item_texture[3];
        break;

    case LASER:
        graphic = item_texture[4];
        break;

    case SCORE:
        graphic = item_texture[5];
        position = {x_pos, y_pos, ITEM_W/2, ITEM_H/2};
        break;

    default:
        break;
    }

    aabb = position;
    speed = LX_Vector2D(XVEL_SCORE, 0.0f);
}


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

    if(bonus != NOPOW)
    {
        if(bonus == SCORE)
        {
            if(inPlayerField())
            {
                toplayer = true;
                BulletPattern::shotOnTarget(xpos, ypos, last_player_x, last_player_y,
                                            VELF, speed);
            }
        }
        else
        {
            if(ypos > (Engine::getMaxYlim() - position.w) || ypos < Engine::getMinYlim())
                speed.vy = -speed.vy;
        }

        moveRect(position, speed);
        moveRect(aabb, speed);
    }
}


bool Item::inPlayerField()
{
    Player::accept(this);

    const int FIELD_COEF = 3;
    int fxpos = last_player_x - Player::PLAYER_WIDTH;
    int fypos = last_player_y - Player::PLAYER_WIDTH;
    int fwidth  = Player::PLAYER_WIDTH * FIELD_COEF;
    int fheight = Player::PLAYER_WIDTH * FIELD_COEF;

    // Area
    LX_Circle field(LX_Point(fxpos + fwidth/2, fypos + fheight/2), fwidth/2);
    int pos_to_get = Engine::getMaxXlim()/2;

    return last_player_x > pos_to_get || toplayer
           || LX_Physics::collisionCircleRect(field, aabb);
}

const LX_AABB& Item::box() const
{
    return aabb;
}


ItemType Item::getPowerUp() const
{
    return bonus;
}
