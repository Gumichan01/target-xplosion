
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
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
*   website : https://gumichan01.github.io/
*   mail : luxon.jean.pierre@gmail.com
*/

#ifndef HEAVISIDE_HPP_INCLUDED
#define HEAVISIDE_HPP_INCLUDED

#include "Enemy.hpp"


class Heaviside: public Enemy
{

public:

    Heaviside(unsigned int hp, unsigned int att, unsigned int sh,
              LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
              int x, int y, int w, int h,float vx, float vy);

    virtual ~Heaviside() = default;
};

#endif // HEAVISIDE_HPP_INCLUDED