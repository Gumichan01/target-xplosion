
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

#include "Background.hpp"
#include "../resources/WinID.hpp"

#include <LunatiX/LX_Graphics.hpp>


Background::Background(std::string bg_file, LX_AABB& rect, int sp)
    : speed(sp), area(rect), background(nullptr)
{
    LX_Win::LX_Window *win = LX_Win::getWindowManager()->getWindow(WinID::getWinID());
    background = new LX_Graphics::LX_Sprite(bg_file.c_str(),*win);
}


Background::~Background()
{
    delete background;
}

// Move the background
void Background::scroll()
{
    if(area.x <= -area.w)
        area.x = 0;
    else
        area.x += speed;
}

void Background::draw()
{
    LX_AABB area2 = area;
    area2.x += area2.w;

    background->draw(&area);
    background->draw(&area2);
}
