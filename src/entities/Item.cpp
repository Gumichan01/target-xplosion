
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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
#include "../game/engine/Engine.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../resources/WinID.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Random.hpp>

using namespace LX_Random;
using namespace LX_Physics;


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
const Float VELF = Float{VEL_SCORE_ITEM};

static LX_Graphics::LX_Sprite *item_texture[Asset::NB_ITEMS];

constexpr short SCORE  = static_cast<short>(ItemType::SCORE);
constexpr short NOPOW  = static_cast<short>(ItemType::NOPOW);
constexpr short HEALTH = static_cast<short>(ItemType::HEALTH);
constexpr short SHIELD = static_cast<short>(ItemType::SHIELD);
constexpr short ROCKET = static_cast<short>(ItemType::ROCKET);
constexpr short BOMB   = static_cast<short>(ItemType::BOMB);
constexpr short LASER  = static_cast<short>(ItemType::LASER);

constexpr Float PLAYER_W = FloatBox::fbox(Player::PLAYER_WIDTH);

}

using namespace FloatBox;

Item::Item(): bonus(ItemType::NOPOW), /*aabb(),*/ toplayer(false)
{
    unsigned short rand_val = xrand<unsigned short>(1, LASER);
    unsigned int lid = Level::getLevelNum();

    if(rand_val <= HEALTH)
    {
        bonus   = ItemType::HEALTH;
        graphic = item_texture[0];
    }
    else if(rand_val <= SHIELD)
    {
        bonus   = ItemType::SHIELD;
        graphic = item_texture[1];
    }
    else if(rand_val <= ROCKET && lid >= Level::ROCKET_LEVEL_MIN)
    {
        bonus   = ItemType::ROCKET;
        graphic = item_texture[2];
    }
    else if(rand_val <= BOMB && lid >= Level::BOMB_LEVEL_MIN)
    {
        bonus   = ItemType::BOMB;
        graphic = item_texture[3];
    }
    else if(rand_val <= LASER && lid >= Level::LASER_LEVEL_MIN)
    {
        bonus   = ItemType::LASER;
        graphic = item_texture[4];
    }
    else
        bonus = ItemType::NOPOW;

    position = {XPOS, static_cast<int>(xorshiftRand100()*RAND_MULT + RAND_OFFSET), ITEM_W, ITEM_H};
    phybox = {{static_cast<float>(position.p.x), static_cast<float>(position.p.y)}, position.w, position.h};
    speed = LX_Vector2D{XVEL, YVEL};
}

// Create score items
Item::Item(int x_pos, int y_pos): Item(x_pos, y_pos, ItemType::SCORE) {}

// General Item creation
Item::Item(int x_pos, int y_pos, ItemType pup): bonus(pup), toplayer(false)
{
    position = {x_pos, y_pos, ITEM_W, ITEM_H};

    switch(bonus)
    {
    case ItemType::HEALTH:
        graphic = item_texture[0];
        break;

    case ItemType::SHIELD:
        graphic = item_texture[1];
        break;

    case ItemType::ROCKET:
        graphic = item_texture[2];
        break;

    case ItemType::BOMB:
        graphic = item_texture[3];
        break;

    case ItemType::LASER:
        graphic = item_texture[4];
        break;

    case ItemType::SCORE:
        graphic = item_texture[5];
        position = {x_pos, y_pos, ITEM_W/2, ITEM_H/2};
        break;

    default:
        break;
    }

    phybox = {{static_cast<float>(position.p.x), static_cast<float>(position.p.y)}, position.w, position.h};
    speed = LX_Vector2D{XVEL_SCORE, 0.0f};
}


void Item::createItemRessources()
{
    const TX_Asset * const asset = TX_Asset::getInstance();
    LX_Win::LX_Window& w = LX_Win::getWindowManager().getWindow(WinID::getWinID());

    for(unsigned int i = 0; i < Asset::NB_ITEMS; i++)
    {
        item_texture[i] = new LX_Graphics::LX_Sprite(asset->getItemFile(i), w);
    }
}

void Item::destroyItemRessources() noexcept
{
    for(unsigned long i = 0; i < Asset::NB_ITEMS; i++)
    {
        delete item_texture[i];
        item_texture[i] = nullptr;
    }
}


void Item::move() noexcept
{
    const Float xpos = phybox.p.x;
    const Float ypos = phybox.p.y;
    const int y = position.p.y;

    if(bonus != ItemType::NOPOW)
    {
        if(bonus == ItemType::SCORE)
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
            if(y > (Engine::getMaxYlim() - position.w) || y < Engine::getMinYlim())
                speed.vy = -speed.vy;
        }

        moveBox(phybox, speed);
        //position = LX_Graphics::toImgRect(phybox);
    }
}


bool Item::inPlayerField() noexcept
{
    Player::accept(this);

    const int FIELD_COEF = 3;
    constexpr Float TWO  = {2.0f};
    const Float& fxpos    = last_player_x - PLAYER_W;
    const Float& fypos    = last_player_y - PLAYER_W;
    const Float& fwidth   = PLAYER_W * fbox(FIELD_COEF);
    const Float& fheight  = PLAYER_W * fbox(FIELD_COEF);

    // Area
    LX_Circle field
    {
        {fxpos + fwidth / TWO, fypos + fheight / TWO},
        (Player::PLAYER_WIDTH * FIELD_COEF) / 2
    };

    int pos_to_get = Engine::getMaxXlim()/2;

    return last_player_x > pos_to_get || toplayer
           || LX_Physics::collisionCircleBox(field, phybox);
}

const LX_Physics::LX_FloatingBox& Item::box() const noexcept
{
    return phybox;
}


ItemType Item::getPowerUp() const noexcept
{
    return bonus;
}
