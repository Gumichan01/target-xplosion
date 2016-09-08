
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


#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

enum GUI_State: short {MAIN_GUI,PLAY_GUI};

namespace LX_Win
{
class LX_Window;
};

struct LX_AABB;

class GUI
{
    LX_Win::LX_Window& win;
    GUI_State state;

public:

    static const int NB_BUTTONS = 3;

    GUI(LX_Win::LX_Window& w);

    void draw();
    void setState(GUI_State st);

    void getAABBs(LX_AABB * aabb);

    ~GUI() = default;
};

#endif // GUI_HPP_INCLUDED
