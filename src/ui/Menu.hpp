
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017 Luxon Jean-Pierre
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
#include <LunatiX/LX_Gamepad.hpp>


namespace LX_Win
{
class LX_Window;
}

namespace LX_Mixer
{
class LX_Music;
}

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
    void gamepadEvent(LX_Event::LX_EventHandler& ev);
    void keyboardEvent(LX_Event::LX_EventHandler& ev);

protected:

    bool _done;
    GUI * gui;
    int cursor;
    bool validate;
    bool has_written;
    LX_AABB * button_rect;

    virtual void subEvent() = 0;
    virtual void hover(LX_Event::LX_EventHandler& ev) = 0;
    virtual void mouseClick(LX_Event::LX_EventHandler& ev) = 0;

public:

    Menu();
    virtual void event();
    virtual ~Menu();
};

class MainMenu: virtual public Menu
{
    LX_Win::LX_Window& win;
    LX_Mixer::LX_Music *music_menu;
    LX_Device::LX_Gamepad gamepad;

    void play();
    void option();
    void loadGamepad();

protected:

    virtual void subEvent();
    virtual void hover(LX_Event::LX_EventHandler& ev);
    virtual void mouseClick(LX_Event::LX_EventHandler& ev);

public:

    explicit MainMenu(LX_Win::LX_Window& w);
    virtual ~MainMenu();
};

class OptionMenu: virtual public Menu
{
    LX_Win::LX_Window& win;
    Option::OptionHandler * opt_handler;

    OptionGUI * getGUI();
    void call_(int cur, bool from_keyboard = false);
    void hover_(int cur);

protected:

    virtual void hover(LX_Event::LX_EventHandler& ev);
    void mouseClick(LX_Event::LX_EventHandler& ev);
    virtual void subEvent();

public:

    explicit OptionMenu(LX_Win::LX_Window& w);
    void gamepad();
    virtual ~OptionMenu();
};

class GamepadMenu: virtual public Menu
{

protected:

    virtual void subEvent() {}
    virtual void hover(LX_Event::LX_EventHandler& ev);
    virtual void mouseClick(LX_Event::LX_EventHandler& ev);

public:

    explicit GamepadMenu(LX_Win::LX_Window& w);
    virtual ~GamepadMenu() = default;
};

#endif // MENU_HPP_INCLUDED
