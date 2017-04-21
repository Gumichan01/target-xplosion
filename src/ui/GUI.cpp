
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

#include "GUI.hpp"
#include "../resources/ResourceManager.hpp"
#include "../option/OptionHandler.hpp"
#include "../asset/TX_Asset.hpp"

#include <LunatiX/LX_AABB.hpp>
#include <LunatiX/LX_Chunk.hpp>
#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_TrueTypeFont.hpp>
#include <LunatiX/LX_Text.hpp>

#include <sstream>
#include <cstdlib>

using namespace LX_Graphics;
using namespace LX_Random;

namespace
{
/// ID of resources
unsigned int bg_id = 0;
const unsigned int button_id = 0;
const unsigned int button_hover_id = 1;
const unsigned int arrow_id = 2;
const unsigned int arrow_hover_id = 3;

/// Colour
const LX_Colour BLACK_COLOUR = {0,0,0,0};
const LX_Colour WHITE_COLOUR = {255,255,255,0};

/// Strings
const std::string TITLE("Target Xplosion");
const std::string PLAY("Play");
const std::string QUIT("Quit");
const std::string OVERALL_VOLUME("Overall volume");
const std::string MUSIC_VOLUME("Music volume");
const std::string FX_VOLUME("FX volume");
const std::string FULLSCREEN("Fullscreen");
const std::string GAMEPAD("Gamepad (soon)");
const std::string BACK("Back");

/// MainGUI
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
const int Y_FULLSCREEN = Y_FX + 90;

const int Y_ARROW_OV = 200;
const int Y_ARROW_MU = 280;
const int Y_ARROW_FX = Y_FX + 8;

const unsigned int OPT_SZ = 64;
const int X_OPTION = 64;
const int Y_GP = 516;
const int Y_BACK = 666;

const int OFFSET_Y = 4;

LX_AABB gp_box = {0,512,427,100};
LX_AABB back_box = {0,662,427,100};
LX_AABB aux_gp_box = {64,512,427,100};
LX_AABB aux_back_box = {224,662,427,100};

LX_AABB option_gp_box = {0,512,448,100};
LX_AABB option_back_box = {0,662,600,100};

LX_AABB option_ovd_box = {512, Y_ARROW_OV, 90, 64};
LX_AABB option_ovu_box = {698, Y_ARROW_OV, 90, 64};
LX_AABB option_mud_box = {512, Y_ARROW_MU, 90, 64};
LX_AABB option_muu_box = {698, Y_ARROW_MU, 90, 64};
LX_AABB option_fxd_box = {512, Y_ARROW_FX, 90, 64};
LX_AABB option_fxu_box = {698, Y_ARROW_FX, 90, 64};
LX_AABB option_fullscreen_box = {516, Y_FULLSCREEN, 256, 64};

// text box
LX_AABB option_oval_box;
LX_AABB option_mval_box;
LX_AABB option_fxval_box;

// Sound played for option
LX_Mixer::LX_Chunk ch;

/* OptionMenuCallback */

const int BASE = 10;
const unsigned short MAX_VOLUME = 100;
const unsigned int EXPLOSION_ID = 3;

// Convert a string to a number
inline unsigned short toNumber(const UTF8string& u8str)
{
    return static_cast<unsigned short>(strtol(u8str.utf8_str(), nullptr, BASE));
}

// Check if the string is a number
inline bool isNumber(const std::string& str)
{
    if(str.empty() || ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+')))
        return false;

    char *p;
    strtol(str.c_str(), &p, BASE);
    return *p == 0;
}

inline UTF8string transformString(const UTF8string& u8str)
{
    std::ostringstream ss;
    unsigned short i = toNumber(u8str);
    ss << (i > MAX_VOLUME ? MAX_VOLUME: i);
    return UTF8string(ss.str());
}



class OptionMenuCallback: public LX_Text::LX_RedrawCallback
{
    LX_Win::LX_Window& _w;
    LX_TextTexture& _t;
    OptionGUI& gui;
    Option::OptionHandler& opt;
    GUI_Button_State st;
    UTF8string u8number;

    OptionMenuCallback(const OptionMenuCallback&);
    OptionMenuCallback& operator =(const OptionMenuCallback&);

public:

    OptionMenuCallback(LX_Win::LX_Window& win, LX_TextTexture& texture,
                       OptionGUI& o, Option::OptionHandler& hdl,
                       GUI_Button_State s)
        : _w(win), _t(texture), gui(o), opt(hdl), st(s) {}

    void operator ()(UTF8string& u8str, UTF8string& u8comp, const bool update,
                     size_t cursor, size_t prev_cur)
    {
        if(update)
        {
            /// Remove the last codepoint if the current cursor < the previous one
            if(cursor == prev_cur - 1)
            {
                if(!u8number.utf8_empty())
                    u8number.utf8_pop();
            }
            else if(!u8str.utf8_empty() && cursor == u8str.utf8_length())
            {
                /// Add the codepoint if it is a number
                std::string s = u8str.utf8_at(u8str.utf8_length() - 1);
                if(isNumber(s)) u8number += s;
            }

            if(!u8number.utf8_empty())
            {
                if(st == OV_TEXT_CLICK)
                    opt.setOverallVolume(toNumber(u8number));
                else if(st == MU_TEXT_CLICK)
                    opt.setMusicVolume(toNumber(u8number));
                else if(st == FX_TEXT_CLICK)
                {
                    opt.setFXVolume(toNumber(u8number));
                    ch.play();
                }
            }

            _t.setText(transformString(u8number));
            gui.draw();
        }
    }

    ~OptionMenuCallback() = default;
};

};


/** GUI */

GUI::GUI(LX_Win::LX_Window& w)
    : win(w), f(nullptr), title_text(nullptr), bg(nullptr),
      state(UNDEF_GUI), bstate(NORMAL) {}

GUI::~GUI()
{
    delete title_text;
    delete f;
}


/** Main GUI */

MainGUI::MainGUI(LX_Win::LX_Window& w)
    : GUI(w), title_font(nullptr), button_play(nullptr), button_option(nullptr),
      button_quit(nullptr), play_text(nullptr), option_text(nullptr),
      quit_text(nullptr)
{
    state = MAIN_GUI;
    const ResourceManager *rc = ResourceManager::getInstance();
    bg_id = LX_Random::crand()%3 +1; // 3 is the number of implemented levels
    bg = new LX_Sprite(TX_Asset::getInstance()->getLevelBg(bg_id),w);
    LX_Sprite *s = rc->getMenuResource(button_id);
    TX_Asset *a = TX_Asset::getInstance();

    f = new LX_TrueTypeFont::LX_Font(a->getFontFile(), BLACK_COLOUR, SELECT_SZ);
    title_font = new LX_TrueTypeFont::LX_Font(a->getFontFile(), WHITE_COLOUR, TITLE_SZ);
    button_play = s;
    button_option = s;
    button_quit = s;

    // Background
    title_text = new LX_BlendedTextTexture(TITLE, *title_font, win);
    title_text->setPosition(X_TITLE, Y_TITLE);

    // Text
    play_text = new LX_BlendedTextTexture(PLAY, *f, win);
    play_text->setPosition(X_PLAY, Y_PLAY);
    option_text = new LX_BlendedTextTexture(OPTION, *f, win);
    option_text->setPosition(X_PLAY, Y_OPT);
    quit_text = new LX_BlendedTextTexture(QUIT, *f, win);
    quit_text->setPosition(X_PLAY, Y_QUIT);
}

MainGUI::~MainGUI()
{
    delete quit_text;
    delete option_text;
    delete play_text;
    delete title_font;
    delete bg;
}


void MainGUI::draw()
{
    win.clearWindow();
    win.setViewPort(nullptr);
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
        button_play = b;
        button_option = bhover;
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

OptionGUI::OptionGUI(LX_Win::LX_Window& w, const Option::OptionHandler& opt)
    : GUI(w), text_font(nullptr), ov_volume_text(nullptr),
      ov_volume_vtext(nullptr), button_ov_down(nullptr), button_ov_up(nullptr),
      music_volume_text(nullptr), music_volume_vtext(nullptr),
      button_music_down(nullptr), button_music_up(nullptr),
      fx_volume_text(nullptr), fx_volume_vtext(nullptr),
      button_fx_down(nullptr), button_fx_up(nullptr),
      fullscreen_text(nullptr), fullscreen_vtext(nullptr),
      gp_text(nullptr), button_gp(nullptr), return_text(nullptr),
      button_back(nullptr)
{
    state = MAIN_GUI;

    ResourceManager *rc = ResourceManager::getInstance();
    LX_Sprite *s = rc->getMenuResource(button_id);
    LX_Sprite *ars = rc->getMenuResource(arrow_id);
    const std::string& ffile = TX_Asset::getInstance()->getFontFile();

    ch.load(TX_Asset::getInstance()->getSound(EXPLOSION_ID));   // Sound for volume
    bg = new LX_Sprite(TX_Asset::getInstance()->getLevelBg(bg_id),w);
    f = new LX_TrueTypeFont::LX_Font(ffile, WHITE_COLOUR, VOL_SZ);
    text_font = new LX_TrueTypeFont::LX_Font(ffile, BLACK_COLOUR, OPT_SZ);

    /// Labels
    title_text = new LX_BlendedTextTexture(OPTION, TITLE_SZ, *f, win);
    ov_volume_text = new LX_BlendedTextTexture(OVERALL_VOLUME, *f, win);
    music_volume_text = new LX_BlendedTextTexture(MUSIC_VOLUME, *f, win);
    fx_volume_text = new LX_BlendedTextTexture(FX_VOLUME, *f, win);
    fullscreen_text = new LX_BlendedTextTexture(FULLSCREEN, *f, win);
    gp_text = new LX_BlendedTextTexture(GAMEPAD, *text_font, win);
    return_text = new LX_BlendedTextTexture(BACK, *text_font, win);

    /// Values
    ov_volume_vtext = new LX_ShadedTextTexture(opt.stringOfOverallVolume(), *f, BLACK_COLOUR, win);
    music_volume_vtext = new LX_ShadedTextTexture(opt.stringOfMusicVolume(), *f, BLACK_COLOUR, win);
    fx_volume_vtext = new LX_ShadedTextTexture(opt.stringOfFXVolume(), *f, BLACK_COLOUR, win);
    fullscreen_vtext = new LX_ShadedTextTexture(opt.stringOfFullscreenFlag(), *f, BLACK_COLOUR, win);

    // Set the position of the textures and set the buttons
    position();
    button_gp = s;
    button_back = s;
    button_ov_down = ars;
    button_ov_up = ars;
    button_music_down = ars;
    button_music_up = ars;
    button_fx_down = ars;
    button_fx_up = ars;

    // Set the boxes of the values
    int width, h;
    ov_volume_vtext->getTextDimension(width, h);
    option_oval_box  = {option_ovd_box.x + option_ovd_box.w, option_ovd_box.y - OFFSET_Y,
                        option_ovu_box.x - option_ovd_box.x, h
                       };
    option_mval_box  = {option_mud_box.x + option_mud_box.w, option_mud_box.y - OFFSET_Y,
                        option_muu_box.x - option_mud_box.x, h
                       };
    option_fxval_box = {option_fxd_box.x + option_fxd_box.w, option_fxd_box.y - OFFSET_Y,
                        option_fxu_box.x - option_fxd_box.x, h
                       };
}

void OptionGUI::position()
{
    title_text->setPosition(X_TITLE, Y_TITLE);
    ov_volume_text->setPosition(X_OPT, Y_OV);
    music_volume_text->setPosition(X_OPT, Y_MUSIC);
    fx_volume_text->setPosition(X_OPT, Y_FX);
    fullscreen_text->setPosition(X_OPT, Y_FULLSCREEN);
    gp_text->setPosition(X_OPTION, Y_GP);
    return_text->setPosition(X_OPTION, Y_BACK);

    ov_volume_vtext->setPosition(option_ovd_box.x + option_ovd_box.w,
                                 option_ovd_box.y - OFFSET_Y);

    music_volume_vtext->setPosition(option_mud_box.x + option_mud_box.w,
                                    option_mud_box.y - OFFSET_Y);

    fx_volume_vtext->setPosition(option_fxd_box.x + option_fxd_box.w,
                                 option_fxd_box.y - OFFSET_Y);

    fullscreen_vtext->setPosition(option_fullscreen_box.x,
                                  option_fullscreen_box.y - OFFSET_Y);
}

void OptionGUI::draw()
{
    win.clearWindow();

    bg->draw();
    title_text->draw();
    ov_volume_text->draw();
    fx_volume_text->draw();
    music_volume_text->draw();
    fullscreen_text->draw();
    fullscreen_vtext->draw();

    button_ov_down->draw(&option_ovd_box, 0.0, LX_Graphics::LX_MIRROR_HORIZONTAL);
    ov_volume_vtext->draw();
    button_ov_up->draw(&option_ovu_box);

    button_music_down->draw(&option_mud_box, 0.0, LX_Graphics::LX_MIRROR_HORIZONTAL);
    music_volume_vtext->draw();
    button_music_up->draw(&option_muu_box);

    button_fx_down->draw(&option_fxd_box, 0.0, LX_Graphics::LX_MIRROR_HORIZONTAL);
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

    button_gp = opt;
    button_back = opt;
    button_ov_down = a;
    button_ov_up = a;
    button_music_down = a;
    button_music_up = a;
    button_fx_down = a;
    button_fx_up = a;
    fullscreen_vtext->setTextColour(WHITE_COLOUR);
    fullscreen_vtext->setBgColour(BLACK_COLOUR);

    switch(bstate)
    {
    case GP_BUTTON_HOVER:
        button_gp = opt_hover;
        break;

    case BACK_BUTTON_HOVER:
        button_back = opt_hover;
        break;

    case OVD_BUTTON_HOVER:
        button_ov_down = a_hover;
        break;

    case OVU_BUTTON_HOVER:
        button_ov_up = a_hover;
        break;

    case MUD_BUTTON_HOVER:
        button_music_down = a_hover;
        break;

    case MUU_BUTTON_HOVER:
        button_music_up = a_hover;
        break;

    case FXD_BUTTON_HOVER:
        button_fx_down = a_hover;
        break;

    case FXU_BUTTON_HOVER:
        button_fx_up = a_hover;
        break;

    case FS_BUTTON_HOVER:
        fullscreen_vtext->setTextColour(BLACK_COLOUR);
        fullscreen_vtext->setBgColour(WHITE_COLOUR);
        break;

    default:
        break;
    }

    fullscreen_vtext->setText(fullscreen_vtext->getText());     //Set text
}


unsigned short OptionGUI::incVolume(unsigned short vol)
{
    return (vol < Option::MAX_VOLUME) ? vol + 1: vol;
}

unsigned short OptionGUI::decVolume(unsigned short vol)
{
    return (vol > 0) ? vol - 1: vol;
}

void OptionGUI::updateTextVolume(GUI_Button_State st, Option::OptionHandler& opt)
{
    LX_TextTexture *t = nullptr;

    if(st == OV_TEXT_CLICK)
        t = ov_volume_vtext;
    else if(st == MU_TEXT_CLICK)
        t = music_volume_vtext;
    else if(st == FX_TEXT_CLICK)
        t = fx_volume_vtext;

    if(t == nullptr)
        return;

    // Text input
    LX_Text::LX_TextInput input;
    OptionMenuCallback clk(win, *t, *this, opt, st);
    input.eventLoop(clk);
}

void OptionGUI::updateVolume(GUI_Button_State st, Option::OptionHandler& opt)
{
    bstate = st;

    switch(bstate)
    {
    case OVD_BUTTON_CLICK:
        opt.setOverallVolume(decVolume(opt.getOverallVolume()));
        ov_volume_vtext->setText(opt.stringOfOverallVolume());
        break;

    case OVU_BUTTON_CLICK:
        opt.setOverallVolume(incVolume(opt.getOverallVolume()));
        ov_volume_vtext->setText(opt.stringOfOverallVolume());
        break;

    case MUD_BUTTON_CLICK:
        opt.setMusicVolume(decVolume(opt.getMusicVolume()));
        music_volume_vtext->setText(opt.stringOfMusicVolume());
        break;

    case MUU_BUTTON_CLICK:
        opt.setMusicVolume(incVolume(opt.getMusicVolume()));
        music_volume_vtext->setText(opt.stringOfMusicVolume());
        break;

    case FXD_BUTTON_CLICK:
        opt.setFXVolume(decVolume(opt.getFXVolume()));
        fx_volume_vtext->setText(opt.stringOfFXVolume());
        ch.play();
        break;

    case FXU_BUTTON_CLICK:
        opt.setFXVolume(incVolume(opt.getFXVolume()));
        fx_volume_vtext->setText(opt.stringOfFXVolume());
        ch.play();
        break;

    default:
        break;
    }
}

void OptionGUI::updateFullscreen(GUI_Button_State st, Option::OptionHandler& opt)
{
    bstate = st;

    switch(bstate)
    {
    case FS_BUTTON_CLICK:
        if(opt.getFullscreenFlag() == static_cast<uint8_t>(1))
        {
            win.toggleFullscreen(LX_Win::LX_WINDOW_NO_FULLSCREEN);
            opt.setFullscreenFlag(0);
        }
        else
        {
            win.toggleFullscreen(LX_Win::LX_WINDOW_FULLSCREEN);
            opt.setFullscreenFlag(1);
        }

        fullscreen_vtext->setBgColour(BLACK_COLOUR);
        fullscreen_vtext->setText(opt.stringOfFullscreenFlag());
        break;

    default:
        break;
    }
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
    aabb[8] = option_fullscreen_box;
    aabb[9] = option_oval_box;
    aabb[10] = option_mval_box;
    aabb[11] = option_fxval_box;
}


OptionGUI::~OptionGUI()
{
    delete return_text;
    delete gp_text;
    delete fullscreen_vtext;
    delete fullscreen_text;
    delete fx_volume_vtext;
    delete fx_volume_text;
    delete music_volume_vtext;
    delete music_volume_text;
    delete ov_volume_vtext;
    delete ov_volume_text;
    delete text_font;
    delete bg;
}


/** Gamepad GUI */

GamepadGUI::GamepadGUI(LX_Win::LX_Window& w): GUI(w)
{

}

void GamepadGUI::draw()
{

}

void GamepadGUI::setButtonState(GUI_Button_State st)
{

}

void GamepadGUI::getAABBs(LX_AABB * aabb)
{
    aabb[0] = option_back_box;
}

GamepadGUI::~GamepadGUI()
{

}
