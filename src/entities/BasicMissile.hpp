
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
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#ifndef BASICMISSILE_H_INCLUDED
#define BASICMISSILE_H_INCLUDED

#include "Missile.hpp"


class BasicMissile final: public Missile
{
    static const int BASIC_MULTIPLIER = 3;

public:

    BasicMissile( unsigned int pow, lx::Graphics::Sprite * image, lx::Graphics::ImgRect& rect,
                  lx::Physics::Vector2D& sp )
        : Missile( pow, BASIC_MULTIPLIER, image, rect, sp ) {}

    virtual ~BasicMissile() = default;
};

#endif // BASICMISSILE_H_INCLUDED
