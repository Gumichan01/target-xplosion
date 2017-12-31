
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

#include "Laser.hpp"

namespace
{
const int LASER_MULTIPLIER = 10;
}

Laser::Laser(unsigned int pow, LX_Graphics::LX_Sprite *image, LX_AABB& rect,
             LX_Physics::LX_Vector2D& sp)
    : Missile(pow, LASER_MULTIPLIER, image, rect, sp), state(true) {}


void Laser::move()
{
    if(!state)
        die();
    else
    {
        state = false;
        Missile::move();
    }
}

void Laser::die()
{
    Entity::die();
}

Laser::~Laser() {}
