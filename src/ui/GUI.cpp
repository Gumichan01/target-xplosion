
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
// ID of resources
const unsigned int bg_id = 0;
const unsigned int button_id = 1;
const unsigned int button_hover_id = 2;

// Box of main menu buttons
LX_AABB play_box = {0,300,527,100};
LX_AABB opt_box = {0,450,677,100};
LX_AABB quit_box = {0,600,827,100};

LX_AABB button1_box = {0,300,427,100};
LX_AABB button2_box = {0,450,427,100};
LX_AABB button3_box = {0,600,427,100};

LX_AABB aux1_box = {100,300,427,100};
LX_AABB aux2_box = {250,450,427,100};
LX_AABB aux3_box = {250,600,427,100};
LX_AABB aux4_box = {400,600,427,100};

// MainGUI
const std::string TITLE("Target Xplosion");
const unsigned int TITLE_SZ = 128;
const int X_TITLE = 192;
const int Y_TITLE = 32;

const unsigned int SELECT_SZ = 64;
const int X_PLAY = 160;
const int Y_PLAY = 304;

const int Y_OPT = 454;
const int Y_QUIT = 604;

// OptionGUI
const std::string OPTION("Option");
const unsigned int OV_VOLUME_SZ = 48;
const int X_OV = 64;
const int Y_OV = 192;
};


/** GUI */

GUI::GUI(LX_Win::LX_Window& w)
    : win(w),f(nullptr),title_text(nullptr),bg(nullptr),
      state(UNDEF_GUI),bstate(NORMAL) {}

GUI::~GUI()
{
    delete title_text;
    delete f;
}


/** Main GUI */

MainGUI::MainGUI(LX_Win::LX_Window& w)
    : GUI(w),button_play(nullptr),button_option(nullptr),
      button_quit(nullptr), play_text(nullptr),option_text(nullptr),
      quit_text(nullptr)
{
    state = MAIN_GUI;
    const SDL_Color c = {0,0,0,0};
    const SDL_Color white_color = {255,255,255,0};
    ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *bgs = rc->getMenuResource(bg_id);
    LX_Graphics::LX_Sprite *s = rc->getMenuResource(button_id);

    f = new LX_TrueTypeFont::LX_Font(c);
    bg = bgs;
    button_play = s;
    button_option = s;
    button_quit = s;

    // Background
    f->setColor(white_color);
    title_text = new LX_Graphics::LX_BlendedTextImage(TITLE,TITLE_SZ,*f,win);
    f->setColor(c);
    title_text->setPosition(X_TITLE,Y_TITLE);

    // Text
    play_text = new LX_Graphics::LX_BlendedTextImage("Play",SELECT_SZ,*f,win);
    play_text->setPosition(X_PLAY,Y_PLAY);
    option_text = new LX_Graphics::LX_BlendedTextImage(OPTION,SELECT_SZ,*f,win);
    option_text->setPosition(X_PLAY,Y_OPT);
    quit_text = new LX_Graphics::LX_BlendedTextImage("Quit",SELECT_SZ,*f,win);
    quit_text->setPosition(X_PLAY,Y_QUIT);
}

MainGUI::~MainGUI()
{
    delete quit_text;
    delete option_text;
    delete play_text;
}


void MainGUI::draw()
{
    win.clearWindow();

    bg->draw();
    title_text->draw();

    // Button
    button_play->draw(&button1_box);
    button_play->draw(&aux1_box);
    button_option->draw(&button2_box);
    button_option->draw(&aux2_box);
    button_quit->draw(&button3_box);
    button_quit->draw(&aux3_box);
    button_quit->draw(&aux4_box);

    // Text
    play_text->draw();
    option_text->draw();
    quit_text->draw();

    win.update();
}


void MainGUI::setState(GUI_State st)
{
    state = st;
}

void MainGUI::setButtonState(GUI_Button_State st)
{
    bstate = st;
    ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *b = rc->getMenuResource(button_id);
    LX_Graphics::LX_Sprite *bhover = rc->getMenuResource(button_hover_id);

    switch(bstate)
    {
    case PLAY_BUTTON_HOVER:
        button_play = bhover;
        break;
    case OPT_BUTTON_HOVER:
        button_option = bhover;
        break;
    case QUIT_BUTTON_HOVER:
        button_quit = bhover;
        break;
    default:
    {
        button_play = b;
        button_option = b;
        button_quit = b;
    }
    break;
    }
}

void MainGUI::getAABBs(LX_AABB * aabb)
{
    if(aabb != nullptr)
    {
        aabb[0] = play_box;
        aabb[1] = opt_box;
        aabb[2] = quit_box;
    }
}


/** OptionGUI */

OptionGUI::OptionGUI(LX_Win::LX_Window& w)
    : GUI(w),ov_volume_text(nullptr),button_ov_down(nullptr),button_ov_up(nullptr),
      music_volume_text(nullptr),button_music_down(nullptr),button_music_up(nullptr),
      fx_volume_text(nullptr),button_fx_down(nullptr),button_fx_up(nullptr),
      gp_text(nullptr),return_text(nullptr),button_back(nullptr)
{
    state = MAIN_GUI;
    const SDL_Color c = {0,0,0,0};
    const SDL_Color white_color = {255,255,255,0};
    const SDL_Color blue_color = {0,192,255,0};
    ResourceManager *rc = ResourceManager::getInstance();
    bg = rc->getMenuResource(bg_id);
    //LX_Graphics::LX_Sprite *s = rc->getMenuResource(button_id);
    f = new LX_TrueTypeFont::LX_Font(blue_color);
    title_text = new LX_Graphics::LX_BlendedTextImage(OPTION,TITLE_SZ,*f,win);
    title_text->setPosition(X_TITLE,Y_TITLE);

    f->setColor(white_color);
    ov_volume_text = new LX_Graphics::LX_BlendedTextImage("Overall volume",
                                                          OV_VOLUME_SZ,*f,win);
    ov_volume_text->setPosition(X_OV,Y_OV);
    /// @todo other texts
    f->setColor(c);
}

void OptionGUI::draw()
{
    win.clearWindow();

    bg->draw();
    title_text->draw();
    ov_volume_text->draw();
    win.update();
}

void OptionGUI::setButtonState(GUI_Button_State st)
{

}

void OptionGUI::getAABBs(LX_AABB * aabb)
{
    //aabb[0] = ;
}


OptionGUI::~OptionGUI()
{
    delete return_text;
    delete gp_text;
    delete fx_volume_text;
    delete music_volume_text;
    delete ov_volume_text;
}
