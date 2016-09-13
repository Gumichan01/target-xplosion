
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
#include "../option/VolumeHandler.hpp"

#include <LunatiX/LX_AABB.hpp>
#include <LunatiX/LX_Image.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_TrueTypeFont.hpp>
#include <LunatiX/utils/utf8_string.hpp>

#include <SDL2/SDL_pixels.h>


namespace
{
/// ID of resources
const unsigned int bg_id = 0;
const unsigned int button_id = 1;
const unsigned int button_hover_id = 2;
const unsigned int arrow_id = 3;
const unsigned int arrow_hover_id = 4;

/// MainGUI
const std::string TITLE("Target Xplosion");
const unsigned int TITLE_SZ = 128;
const int X_TITLE = 192;
const int Y_TITLE = 32;

const unsigned int SELECT_SZ = 64;
const int X_PLAY = 160;
const int Y_PLAY = 304;

const int Y_OPT = 454;
const int Y_QUIT = 604;

// Box of main menu buttons
LX_AABB play_box = {0,300,527,100};
LX_AABB opt_box = {0,450,677,100};
LX_AABB quit_box = {0,600,827,100};

LX_AABB main_play_box = {0,300,427,100};
LX_AABB main_opt_box = {0,450,427,100};
LX_AABB main_quit_box = {0,600,427,100};

LX_AABB aux1_box = {100,300,427,100};
LX_AABB aux2_box = {250,450,427,100};
LX_AABB aux3_box = {250,600,427,100};
LX_AABB aux4_box = {400,600,427,100};

/// OptionGUI
const std::string OPTION("Option");
const unsigned int VOL_SZ = 64;
const int X_OPT = 64;
const int Y_OV = 192;
const int Y_MUSIC = 272;
const int Y_FX = Y_MUSIC + 80;

const int Y_ARROW_OV = 200;
const int Y_ARROW_MU = 280;
const int Y_ARROW_FX = Y_FX + 8;

const unsigned int OPT_SZ = 64;
const int X_OPTION = 64;
const int Y_GP = 452;
const int Y_BACK = 604;

const int OFFSET_Y = 4;

LX_AABB gp_box = {0,448,427,100};
LX_AABB back_box = {0,600,427,100};
LX_AABB aux_gp_box = {64,448,427,100};
LX_AABB aux_back_box = {224,600,427,100};

LX_AABB option_gp_box = {0,448,448,100};
LX_AABB option_back_box = {0,600,600,100};

LX_AABB option_ovd_box = {512,Y_ARROW_OV,90,64};
LX_AABB option_ovu_box = {698,Y_ARROW_OV,90,64};
LX_AABB option_mud_box = {512,Y_ARROW_MU,90,64};
LX_AABB option_muu_box = {698,Y_ARROW_MU,90,64};
LX_AABB option_fxd_box = {512,Y_ARROW_FX,90,64};
LX_AABB option_fxu_box = {698,Y_ARROW_FX,90,64};
};

using namespace LX_Graphics;


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
    LX_Sprite *bgs = rc->getMenuResource(bg_id);
    LX_Sprite *s = rc->getMenuResource(button_id);

    f = new LX_TrueTypeFont::LX_Font(c);
    bg = bgs;
    button_play = s;
    button_option = s;
    button_quit = s;

    // Background
    f->setColor(white_color);
    title_text = new LX_BlendedTextImage(TITLE,TITLE_SZ,*f,win);
    f->setColor(c);
    title_text->setPosition(X_TITLE,Y_TITLE);

    // Text
    play_text = new LX_BlendedTextImage("Play",SELECT_SZ,*f,win);
    play_text->setPosition(X_PLAY,Y_PLAY);
    option_text = new LX_BlendedTextImage(OPTION,SELECT_SZ,*f,win);
    option_text->setPosition(X_PLAY,Y_OPT);
    quit_text = new LX_BlendedTextImage("Quit",SELECT_SZ,*f,win);
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
    button_play->draw(&main_play_box);
    button_play->draw(&aux1_box);
    button_option->draw(&main_opt_box);
    button_option->draw(&aux2_box);
    button_quit->draw(&main_quit_box);
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
    LX_Sprite *b = rc->getMenuResource(button_id);
    LX_Sprite *bhover = rc->getMenuResource(button_hover_id);

    switch(bstate)
    {
    case PLAY_BUTTON_HOVER:
        button_play = bhover;
        button_option = b;
        button_quit = b;
        break;
    case OPT_BUTTON_HOVER:
        button_option = bhover;
        button_play = b;
        button_quit = b;
        break;
    case QUIT_BUTTON_HOVER:
        button_play = b;
        button_option = b;
        button_quit = bhover;
        break;
    default:
        button_play = b;
        button_option = b;
        button_quit = b;
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

OptionGUI::OptionGUI(LX_Win::LX_Window& w, const Option::VolumeHandler& v)
    : GUI(w),ov_volume_text(nullptr),ov_volume_vtext(nullptr),
      button_ov_down(nullptr),button_ov_up(nullptr),
      music_volume_text(nullptr),music_volume_vtext(nullptr),
      button_music_down(nullptr),button_music_up(nullptr),
      fx_volume_text(nullptr),fx_volume_vtext(nullptr),
      button_fx_down(nullptr),button_fx_up(nullptr),
      gp_text(nullptr),button_gp(nullptr),return_text(nullptr),button_back(nullptr)
{
    state = MAIN_GUI;
    const SDL_Color c = {0,0,0,0};
    const SDL_Color white_color = {255,255,255,0};
    const SDL_Color blue_color = {0,192,255,0};

    ResourceManager *rc = ResourceManager::getInstance();
    LX_Sprite *s = rc->getMenuResource(button_id);
    LX_Sprite *ars = rc->getMenuResource(arrow_id);

    bg = rc->getMenuResource(bg_id);
    f = new LX_TrueTypeFont::LX_Font(blue_color);
    title_text = new LX_BlendedTextImage(OPTION,TITLE_SZ,*f,win);
    title_text->setPosition(X_TITLE,Y_TITLE);

    f->setColor(white_color);
    ov_volume_text = new LX_BlendedTextImage("Overall volume",VOL_SZ,*f,win);
    ov_volume_text->setPosition(X_OPT,Y_OV);
    music_volume_text = new LX_BlendedTextImage("Music volume",VOL_SZ,*f,win);
    music_volume_text->setPosition(X_OPT,Y_MUSIC);

    fx_volume_text = new LX_BlendedTextImage("FX volume",VOL_SZ,*f,win);
    fx_volume_text->setPosition(X_OPT,Y_FX);
    f->setColor(c);

    // Buttons
    button_gp = s;
    button_back = s;
    button_ov_down = ars;
    button_ov_up = ars;
    button_music_down = ars;
    button_music_up = ars;
    button_fx_down = ars;
    button_fx_up = ars;

    // Volume value
    f->setColor(white_color);
    ov_volume_vtext = new LX_ShadedTextImage(*f,w);
    ov_volume_vtext->setText(v.stringOfOverallVolume(),c,VOL_SZ);
    ov_volume_vtext->setPosition(option_ovd_box.x + option_ovd_box.w,option_ovd_box.y - OFFSET_Y);

    music_volume_vtext = new LX_ShadedTextImage(*f,w);
    music_volume_vtext->setText(v.stringOfMusicVolume(),c,VOL_SZ);
    music_volume_vtext->setPosition(option_mud_box.x + option_mud_box.w,option_mud_box.y - OFFSET_Y);

    fx_volume_vtext = new LX_ShadedTextImage(*f,w);
    fx_volume_vtext->setText(v.stringOfFXVolume(),c,VOL_SZ);
    fx_volume_vtext->setPosition(option_fxd_box.x + option_fxd_box.w,option_fxd_box.y - OFFSET_Y);
    f->setColor(c);

    gp_text = new LX_BlendedTextImage("Gamepad",OPT_SZ,*f,win);
    gp_text->setPosition(X_OPTION,Y_GP);

    return_text = new LX_BlendedTextImage("Back",OPT_SZ,*f,win);
    return_text->setPosition(X_OPTION,Y_BACK);
}

void OptionGUI::draw()
{
    win.clearWindow();

    bg->draw();
    title_text->draw();
    ov_volume_text->draw();
    fx_volume_text->draw();
    music_volume_text->draw();

    button_ov_down->draw(&option_ovd_box,0.0,LX_Graphics::LX_MIRROR_HORIZONTAL);
    ov_volume_vtext->draw();
    button_ov_up->draw(&option_ovu_box);

    button_music_down->draw(&option_mud_box,0.0,LX_Graphics::LX_MIRROR_HORIZONTAL);
    music_volume_vtext->draw();
    button_music_up->draw(&option_muu_box);

    button_fx_down->draw(&option_fxd_box,0.0,LX_Graphics::LX_MIRROR_HORIZONTAL);
    fx_volume_vtext->draw();
    button_fx_up->draw(&option_fxu_box);

    button_gp->draw(&gp_box);
    button_gp->draw(&aux_gp_box);
    button_back->draw(&back_box);
    button_back->draw(&aux_back_box);

    gp_text->draw();
    return_text->draw();

    win.update();
}


void OptionGUI::setButtonState(GUI_Button_State st)
{
    bstate = st;
    ResourceManager *rc = ResourceManager::getInstance();
    LX_Sprite *opt = rc->getMenuResource(button_id);
    LX_Sprite *a = rc->getMenuResource(arrow_id);
    LX_Sprite *a_hover = rc->getMenuResource(arrow_hover_id);
    LX_Sprite *opt_hover = rc->getMenuResource(button_hover_id);

    switch(bstate)
    {
    case GP_BUTTON_HOVER:
        button_gp = opt_hover;
        button_back = opt;
        button_ov_down = a;
        button_ov_up = a;
        button_music_down = a;
        button_music_up = a;
        button_fx_down = a;
        button_fx_up = a;
        break;

    case BACK_BUTTON_HOVER:
        button_gp = opt;
        button_back = opt_hover;
        button_ov_down = a;
        button_ov_up = a;
        button_music_down = a;
        button_music_up = a;
        button_fx_down = a;
        button_fx_up = a;
        break;

    case OVD_BUTTON_HOVER:
        button_gp = opt;
        button_back = opt;
        button_ov_down = a_hover;
        button_ov_up = a;
        button_music_down = a;
        button_music_up = a;
        button_fx_down = a;
        button_fx_up = a;
        break;

    case OVU_BUTTON_HOVER:
        button_gp = opt;
        button_back = opt;
        button_ov_down = a;
        button_ov_up = a_hover;
        button_music_down = a;
        button_music_up = a;
        button_fx_down = a;
        button_fx_up = a;
        break;

    case MUD_BUTTON_HOVER:
        button_gp = opt;
        button_back = opt;
        button_ov_down = a;
        button_ov_up = a;
        button_music_down = a_hover;
        button_music_up = a;
        button_fx_down = a;
        button_fx_up = a;
        break;

    case MUU_BUTTON_HOVER:
        button_gp = opt;
        button_back = opt;
        button_ov_down = a;
        button_ov_up = a;
        button_music_down = a;
        button_music_up = a_hover;
        button_fx_down = a;
        button_fx_up = a;
        break;

    case FXD_BUTTON_HOVER:
        button_gp = opt;
        button_back = opt;
        button_ov_down = a;
        button_ov_up = a;
        button_music_down = a;
        button_music_up = a;
        button_fx_down = a_hover;
        button_fx_up = a;
        break;

    case FXU_BUTTON_HOVER:
        button_gp = opt;
        button_back = opt;
        button_ov_down = a;
        button_ov_up = a;
        button_music_down = a;
        button_music_up = a;
        button_fx_down = a;
        button_fx_up = a_hover;
        break;

    default:
        button_gp = opt;
        button_back = opt;
        button_ov_down = a;
        button_ov_up = a;
        button_music_down = a;
        button_music_up = a;
        button_fx_down = a;
        button_fx_up = a;
        break;
    }
}

void OptionGUI::updateVolume(GUI_Button_State st, Option::VolumeHandler& v)
{
    bstate = st;
    const SDL_Color BLACK = {0,0,0,0};

    f->setColor({255,255,255,0});

    switch(bstate)
    {
    case OVD_BUTTON_CLICK:
        if(v.getOverallVolume() > 0)
            v.setOverallVolume(v.getOverallVolume() - 1);

        ov_volume_vtext->setText(v.stringOfOverallVolume(),BLACK,VOL_SZ);
        break;

    case OVU_BUTTON_CLICK:
        if(v.getOverallVolume() < Option::MAX_VOLUME)
            v.setOverallVolume(v.getOverallVolume() + 1);

        ov_volume_vtext->setText(v.stringOfOverallVolume(),BLACK,VOL_SZ);
        break;

    case MUD_BUTTON_CLICK:
        if(v.getMusicVolume() > 0)
            v.setMusicVolume(v.getMusicVolume() - 1);
        music_volume_vtext->setText(v.stringOfMusicVolume(),BLACK,VOL_SZ);
        break;

    case MUU_BUTTON_CLICK:
        if(v.getMusicVolume() < Option::MAX_VOLUME)
            v.setMusicVolume(v.getMusicVolume() + 1);

        music_volume_vtext->setText(v.stringOfMusicVolume(),BLACK,VOL_SZ);
        break;

    case FXD_BUTTON_CLICK:
        if(v.getFXVolume() > 0)
            v.setFXVolume(v.getFXVolume() - 1);

        fx_volume_vtext->setText(v.stringOfFXVolume(),BLACK,VOL_SZ);
        break;

    case FXU_BUTTON_CLICK:
        if(v.getFXVolume() < Option::MAX_VOLUME)
            v.setFXVolume(v.getFXVolume() + 1);

        fx_volume_vtext->setText(v.stringOfFXVolume(),BLACK,VOL_SZ);
        break;

    default:
        break;
    }

    f->setColor(BLACK);
}


void OptionGUI::getAABBs(LX_AABB * aabb)
{
    aabb[0] = option_gp_box;
    aabb[1] = option_back_box;
    aabb[2] = option_ovd_box;
    aabb[3] = option_ovu_box;
    aabb[4] = option_mud_box;
    aabb[5] = option_muu_box;
    aabb[6] = option_fxd_box;
    aabb[7] = option_fxu_box;
}


OptionGUI::~OptionGUI()
{
    delete return_text;
    delete gp_text;
    delete fx_volume_text;
    delete music_volume_text;
    delete ov_volume_text;
}
