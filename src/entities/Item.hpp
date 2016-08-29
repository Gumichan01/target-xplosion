
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


#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include "Entity.hpp"
#include "PlayerVisitor.hpp"


enum POWER_UP: short {SCORE=0,NO_POWER_UP=15,HEALTH=35,
                      SHIELD=55,ROCKET=70,BOMB=90,LASER=100
                     };

class Item: public Entity, public PlayerVisitor
{
    POWER_UP bonus;
    SDL_Rect aabb;

public :

    Item();
    Item(int x_pos, int y_pos);

    static void createItemRessources();
    static void destroyItemRessources();

    const POWER_UP& getPowerUp() const;
    void move();
    const SDL_Rect& box();

    ~Item();
};

#endif // ITEM_H_INCLUDED

