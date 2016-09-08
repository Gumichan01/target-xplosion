
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
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_AABB.hpp>
#include <LunatiX/LX_Image.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_TrueTypeFont.hpp>
#include <LunatiX/utils/utf8_string.hpp>

#include <SDL2/SDL_pixels.h>


namespace
{
LX_AABB button1_box = {0,300,427,100};
LX_AABB button2_box = {0,450,427,100};
LX_AABB button3_box = {0,600,427,100};

LX_AABB aux1_box = {100,300,427,100};
LX_AABB aux2_box = {250,450,427,100};
LX_AABB aux3_box = {250,600,427,100};
LX_AABB aux4_box = {400,600,427,100};
};


GUI::GUI(LX_Win::LX_Window& w)
    : win(w),f(nullptr),state(MAIN_GUI),button_play(nullptr),button_option(nullptr),
      button_quit(nullptr), play_text(nullptr),option_text(nullptr),quit_text(nullptr)
{
    const SDL_Color c = {0,0,0,0};
    ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *s = rc->getMenuResource(1);

    f = new LX_TrueTypeFont::LX_Font(c);
    button_play = s;
    button_option = s;
    button_quit = s;
    //play_text = new LX_Graphics::LX_BlendedTextImage("Play",32,*f,win);
    //option_text = new LX_Graphics::LX_BlendedTextImage("Option",32,*f,win);
    //quit_text = new LX_Graphics::LX_BlendedTextImage("Quit",32,*f,win);
}

GUI::~GUI()
{
    delete quit_text;
    delete option_text;
    delete play_text;
    delete f;
}


void GUI::draw()
{
    win.clearWindow();
    button_play->draw(&button1_box);
    button_play->draw(&aux1_box);
    button_option->draw(&button2_box);
    button_option->draw(&aux2_box);
    button_quit->draw(&button3_box);
    button_quit->draw(&aux3_box);
    button_quit->draw(&aux4_box);
    win.update();
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
