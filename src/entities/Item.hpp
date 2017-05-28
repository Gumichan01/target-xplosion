
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


#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include "Entity.hpp"
#include "PlayerVisitor.hpp"


using ItemType = short;

namespace POWER_UP
{
const short SCORE  = 0;
const short NOPOW  = 15;
const short HEALTH = 35;
const short SHIELD = 55;
const short ROCKET = 75;
const short BOMB   = 90;
const short LASER  = 100;
}


class Item: public Entity, public PlayerVisitor
{
    ItemType bonus;
    LX_AABB aabb;

public:

    Item();
    Item(int x_pos, int y_pos);
    Item(int x_pos, int y_pos, ItemType pup);

    static void createItemRessources();
    static void destroyItemRessources();

    ItemType getPowerUp() const;
    virtual void move();
    const LX_AABB& box() const;

    ~Item();
};

#endif // ITEM_H_INCLUDED

