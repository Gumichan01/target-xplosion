
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


#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include "Entity.hpp"
#include "PlayerVisitor.hpp"


// Score
enum class ItemType : short
{
    SCORE  = 0,
    NOPOW  = 1,
    HEALTH = 27,
    SHIELD = 57,
    ROCKET = 77,
    BOMB   = 92,
    LASER  = 102
};

class Item: public Entity, public PlayerVisitor
{
    ItemType bonus;
    bool toplayer;

    bool inPlayerField() noexcept;

    Item(const Item&) = delete;
    Item(const Item&&) = delete;
    Item& operator =(const Item&) = delete;
    Item& operator =(const Item&&) = delete;

public:

    Item();
    Item(int x_pos, int y_pos);
    Item(int x_pos, int y_pos, ItemType pup);

    static void createItemRessources();
    static void destroyItemRessources() noexcept;

    ItemType getPowerUp() const noexcept;
    virtual void move() noexcept;
    const LX_Physics::LX_FloatingBox& box() const noexcept;

    ~Item() = default;
};

#endif // ITEM_H_INCLUDED
