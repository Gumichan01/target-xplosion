
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
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/

#include "GUI.hpp"

#include <LunatiX/LX_AABB.hpp>


namespace
{
const LX_AABB button1_box = {0,100,427,100};
const LX_AABB button2_box = {200,100,427,100};
const LX_AABB button3_box = {300,100,427,100};
};


GUI::GUI(LX_Win::LX_Window& w) : win(w), state(MAIN_GUI) {}


void GUI::draw()
{

}


void GUI::setState(GUI_State st)
{
    state = st;
}

void GUI::getAABBs(LX_AABB * aabb)
{
    if(aabb != nullptr)
    {
        aabb[0] = button1_box;
        aabb[1] = button2_box;
        aabb[2] = button3_box;
    }
}
