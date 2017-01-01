
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

#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

#include <LunatiX/LX_AABB.hpp>

namespace LX_Win
{
class LX_Window;
};

namespace Option
{
class OptionHandler;
}

namespace LX_Event
{
class LX_EventHandler;
}

class GUI;
class MainGUI;
class OptionGUI;

class Menu
{
protected:

    GUI * gui;
    virtual void hover(LX_Event::LX_EventHandler& ev) = 0;
    virtual void mouseClick(LX_Event::LX_EventHandler& ev, bool& done) = 0;

public:

    Menu();
    virtual void event();
    virtual ~Menu();
};

class MainMenu: virtual public Menu
{
    LX_AABB * button_rect;
    LX_Win::LX_Window& win;

    virtual void hover(LX_Event::LX_EventHandler& ev);
    virtual void mouseClick(LX_Event::LX_EventHandler& ev, bool& done);

    void play();
    void option();

public:

    explicit MainMenu(LX_Win::LX_Window& w);
    virtual ~MainMenu();
};

class OptionMenu: virtual public Menu
{
    LX_AABB * button_rect;
    Option::OptionHandler * opt_handler;

    virtual void hover(LX_Event::LX_EventHandler& ev);
    virtual void mouseClick(LX_Event::LX_EventHandler& ev, bool& done);

public:

    explicit OptionMenu(LX_Win::LX_Window& w);
    virtual void event();
    virtual ~OptionMenu();
};

#endif // MENU_HPP_INCLUDED
