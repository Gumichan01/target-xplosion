
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


#include "Menu.hpp"
#include "GUI.hpp"

#include <LunatiX/LX_AABB.hpp>
#include <LunatiX/LX_Window.hpp>

#include <SDL2/SDL_events.h>


/* Menu */

Menu::Menu() {}

Menu::~Menu() {}


/* Main menu */

MainMenu::MainMenu(LX_Win::LX_Window& w) : gui(new GUI(w)), button_rect(nullptr)
{
    button_rect = new LX_AABB[GUI::NB_BUTTONS];
    gui->getAABBs(button_rect);
}


MainMenu::~MainMenu()
{
    delete [] button_rect;
    delete gui;
}


void MainMenu::event()
{
    SDL_Event ev;
    bool done = false;

    while(!done)
    {
        while(SDL_PollEvent(&ev))
        {
            switch(ev.type)
            {
            case SDL_QUIT:
                done = true;
                break;

            case SDL_MOUSEBUTTONUP:
                mouseClick(ev);
                break;

            case SDL_MOUSEMOTION:
                hover(ev);
                break;

            default:
                break;
            }
        }
    }
}


void MainMenu::hover(SDL_Event& ev)
{

}


void MainMenu::mouseClick(SDL_Event& ev)
{

}


