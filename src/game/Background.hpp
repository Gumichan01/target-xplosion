
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

#include <LunatiX/LX_AABB.hpp>
#include <string>

namespace LX_Graphics
{
class LX_Sprite;
}

class Background
{
    int speed;                              // The scrolling speed
    LX_AABB area;                           // The dimension of the background
    LX_Graphics::LX_Sprite * background;    // The image

    Background(const Background& b);

    void scroll();
    void draw();

public:

    Background(std::string bg_file, LX_AABB& rect, int sp);
    void update();
    ~Background();
};

#endif // BACKGROUND_H_INCLUDED

