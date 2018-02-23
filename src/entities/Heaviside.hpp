
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

#ifndef HEAVISIDE_HPP_INCLUDED
#define HEAVISIDE_HPP_INCLUDED

#include "TargetShooter.hpp"


class Heaviside: public TargetShooter
{

public:

    Heaviside(unsigned int hp, unsigned int att, unsigned int sh,
              LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
              float vx, float vy);

    virtual void fire() noexcept;
    virtual ~Heaviside() = default;
};

// Heaviside with the reverse pattern
class RHeaviside: public Heaviside
{

public:

    RHeaviside(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy);

    virtual ~RHeaviside() = default;
};

class HeavisidePurple: public Heaviside
{

public:

    HeavisidePurple(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                    float vx, float vy);

    virtual void fire() noexcept;
    virtual ~HeavisidePurple() = default;
};

class RHeavisidePurple: public HeavisidePurple
{

public:

    RHeavisidePurple(unsigned int hp, unsigned int att, unsigned int sh,
                     LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                     float vx, float vy);

    virtual ~RHeavisidePurple() = default;
};

#endif // HEAVISIDE_HPP_INCLUDED
