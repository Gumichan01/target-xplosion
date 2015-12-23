
/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2015  Luxon Jean-Pierre
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Luxon Jean-Pierre (Gumichan01)
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file hud.cpp
*	@brief The HUD file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <sstream>

#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_TrueTypeFont.hpp>
#include <LunatiX/LX_Hitbox.hpp>

#include "hud.hpp"
#include "Rank.hpp"
#include "../entities/Player.hpp"
#include "../level/Level.hpp"

using namespace std;
using namespace LX_Graphics;
using namespace LX_TrueTypeFont;

static const int HUD_SIZE = 28;             // The font size of the HUD texts
static const int HUD_OFFSET = 800;          // The reference position of the HUD

// X position of the texts
static const int HUD_XPOS1 = HUD_OFFSET/4;
static const int HUD_XPOS2 = HUD_OFFSET/2;

static const int VAL_YPOS = 32;             // Y position of the HUD values
static const int HUD_YPOS = 1;              // Y position of the HUD texts


HUD::HUD(Player& sub)
    : subject(sub),player_hp(sub.getHP()),player_hp_max(sub.getHP()),
      player_rockets(sub.getRocket()),player_bombs(sub.getBomb()),
      hud_font(new LX_Font({255,255,255,0}))
{
    // Empty
}

// Update information
void HUD::update()
{
    player_hp = subject.getHP();
    player_bombs = subject.getBomb();
    player_rockets = subject.getRocket();
}

// Display information
void HUD::displayHUD()
{
    const unsigned int r = Rank::getRank();
    const unsigned int idLevel = Level::getLevelNum();
    LX_Window *win = LX_WindowManager::getInstance()->getWindow(0);

    if(win == nullptr)
        return;

    string hp_info, rocket_info, bomb_info;                     // The strings
    string hp_val, rocket_val, bomb_val;                        // Values
    ostringstream hp_sentence, rocket_sentence, bomb_sentence;  // String streams

    // Get the strings
    hp_sentence << "Health";
    rocket_sentence << "Missiles";
    bomb_sentence << "Bomb";

    hp_info = hp_sentence.str();
    rocket_info = rocket_sentence.str();
    bomb_info = bomb_sentence.str();

    // Clear the content
    hp_sentence.str("");
    rocket_sentence.str("");
    bomb_sentence.str("");

    // Get the values
    hp_sentence << player_hp << " / " << player_hp_max;
    rocket_sentence << player_rockets;
    bomb_sentence << player_bombs;

    hp_val = hp_sentence.str();
    rocket_val = rocket_sentence.str();
    bomb_val = bomb_sentence.str();

    /** Create the display  */
    // The strings positions
    SDL_Rect pos_hp_str = {HUD_XPOS1,HUD_YPOS,0,0};
    SDL_Rect pos_rocket_str = {HUD_XPOS2,HUD_YPOS,0,0};
    SDL_Rect pos_bomb_str = {HUD_XPOS1 + HUD_XPOS2,HUD_YPOS,0,0};

    // The values positions
    SDL_Rect pos_hp_val = {pos_hp_str.x,VAL_YPOS,0,0};
    SDL_Rect pos_rocket_val = {pos_rocket_str.x,VAL_YPOS,0,0};
    SDL_Rect pos_bomb_val = {pos_bomb_str.x,VAL_YPOS,0,0};

    // The textures
    SDL_Texture *hp_str_texture = hud_font->drawTextToTexture(LX_TTF_BLENDED,hp_info.c_str(),HUD_SIZE);
    SDL_Texture *rocket_str_texture = hud_font->drawTextToTexture(LX_TTF_BLENDED,rocket_info.c_str(),HUD_SIZE);
    SDL_Texture *bomb_str_texture = hud_font->drawTextToTexture(LX_TTF_BLENDED,bomb_info.c_str(),HUD_SIZE);
    SDL_Texture *hp_val_texture = hud_font->drawTextToTexture(LX_TTF_BLENDED,hp_val.c_str(),HUD_SIZE);
    SDL_Texture *rocket_val_texture = hud_font->drawTextToTexture(LX_TTF_BLENDED,rocket_val.c_str(),HUD_SIZE);
    SDL_Texture *bomb_val_texture = hud_font->drawTextToTexture(LX_TTF_BLENDED,bomb_val.c_str(),HUD_SIZE);

    // Get sizes of the text to display
    hud_font->sizeOfText(hp_info.c_str(),HUD_SIZE,pos_hp_str.w,pos_hp_str.h);
    hud_font->sizeOfText(rocket_info.c_str(),HUD_SIZE,pos_rocket_str.w,
                         pos_rocket_str.h);
    hud_font->sizeOfText(bomb_info.c_str(),HUD_SIZE,pos_bomb_str.w,
                         pos_bomb_str.h);
    hud_font->sizeOfText(hp_val.c_str(),HUD_SIZE,pos_hp_val.w,pos_hp_val.h);
    hud_font->sizeOfText(rocket_val.c_str(),HUD_SIZE,pos_rocket_val.w,
                         pos_rocket_val.h);
    hud_font->sizeOfText(bomb_val.c_str(),HUD_SIZE,pos_bomb_val.w,
                         pos_bomb_val.h);

    // Put all texts on the screen
    win->putTexture(hp_str_texture,nullptr,&pos_hp_str);
    win->putTexture(hp_val_texture,nullptr,&pos_hp_val);

    // Display bombs and rockets info
    if(r == S_RANK)
    {
        win->putTexture(rocket_str_texture,nullptr,&pos_rocket_str);
        win->putTexture(rocket_val_texture,nullptr,&pos_rocket_val);
        win->putTexture(bomb_str_texture,nullptr,&pos_bomb_str);
        win->putTexture(bomb_val_texture,nullptr,&pos_bomb_val);
    }

    SDL_DestroyTexture(hp_str_texture);
    SDL_DestroyTexture(rocket_str_texture);
    SDL_DestroyTexture(bomb_str_texture);
    SDL_DestroyTexture(hp_val_texture);
    SDL_DestroyTexture(rocket_val_texture);
    SDL_DestroyTexture(bomb_val_texture);
}


HUD::~HUD()
{
    delete hud_font;
}

