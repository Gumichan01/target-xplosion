
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

#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED

#include "../entities/Entity.hpp"

#include <Lunatix/ImgRect.hpp>

namespace lx
{

namespace Graphics
{
class Sprite;
}

namespace Physics
{
struct FloatPosition;
}

}


class Background final
{
    Float speed_fgd, speed_mgd, speed_bgd;

    lx::Graphics::ImgRect area_fgd, area_mgd, area_bgd;
    lx::Physics::FloatPosition pos_fgd, pos_mgd, pos_bgd;
    lx::Graphics::Sprite * foreground;
    lx::Graphics::Sprite * middleground;
    lx::Graphics::Sprite * background;

    bool inc_speed;
    bool is_parallax;
    unsigned int t;

    Background( const Background& b ) = delete;
    Background& operator =( const Background& b ) = delete;

    void scroll();
    void draw();
    void increaseSpeed();

public:

    Background( unsigned int lvl, lx::Graphics::ImgRect& rect, int sp );
    void setIncrease();
    void update();
    ~Background();
};

#endif // BACKGROUND_H_INCLUDED
