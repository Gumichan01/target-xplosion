
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

#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED

#include "../entities/Entity.hpp"

#include <LunatiX/LX_AABB.hpp>
#include <string>

namespace LX_Graphics
{
class LX_Sprite;
}


class Background
{
    float speed_fgd, speed_mgd, speed_bgd;
    LX_AABB area_fgd, area_mgd, area_bgd;
    FloatPosition pos_fgd, pos_mgd, pos_bgd;
    LX_Graphics::LX_Sprite * foreground;
    LX_Graphics::LX_Sprite * middleground;
    LX_Graphics::LX_Sprite * background;
    bool inc_speed;
    bool is_parallax;
    unsigned int t;

    Background(const Background& b);
    Background& operator =(const Background& b);

    void scroll();
    void draw();

public:

    Background(unsigned int lvl, LX_AABB& rect, int sp);
    void setIncrease();
    void increaseSpeed();
    void update();
    ~Background();
};

#endif // BACKGROUND_H_INCLUDED

