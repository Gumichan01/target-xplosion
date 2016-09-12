
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

#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED


namespace LX_Win
{
class LX_Window;
};

namespace Option
{
class VolumeHandler;
}

class GUI;
class MainGUI;
class OptionGUI;
struct LX_AABB;
union SDL_Event;


class Menu
{
protected:

    GUI * gui;
    virtual void hover(SDL_Event& ev) = 0;
    virtual void mouseClick(SDL_Event& ev, bool& done) = 0;

public:

    Menu();
    virtual void event();
    virtual ~Menu();
};

class MainMenu: virtual public Menu
{
    LX_AABB * button_rect;
    LX_Win::LX_Window& win;

    virtual void hover(SDL_Event& ev);
    virtual void mouseClick(SDL_Event& ev, bool& done);

    void play();
    void option();

public:

    explicit MainMenu(LX_Win::LX_Window& w);
    virtual ~MainMenu();
};

class OptionMenu: virtual public Menu
{
    LX_AABB * button_rect;
    Option::VolumeHandler * vhandler;

    virtual void hover(SDL_Event& ev);
    virtual void mouseClick(SDL_Event& ev, bool& done);

public:

    explicit OptionMenu(LX_Win::LX_Window& w);
    virtual ~OptionMenu();
};

#endif // MENU_HPP_INCLUDED
