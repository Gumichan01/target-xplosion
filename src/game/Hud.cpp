
/*
*	Target_Xplosion - A classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
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
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

#include "Hud.hpp"
#include "Rank.hpp"
#include "../entities/Player.hpp"
#include "../level/Level.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Hitbox.hpp>
#include <sstream>

using namespace LX_TrueTypeFont;

const int HUD_SIZE = 28;             // The font size of the HUD texts
const int HUD_OFFSET = 800;          // The reference position of the HUD

// X position of the texts
const int HUD_XPOS1 = HUD_OFFSET/4;
const int HUD_XPOS2 = HUD_OFFSET/2;

const int VAL_YPOS = 32;             // Y position of the HUD values
const int HUD_YPOS = 1;              // Y position of the HUD texts


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
    LX_Win::LX_Window *win = LX_Win::LX_WindowManager::getInstance()->getWindow(0);

    std::string hp_info, rocket_info, bomb_info;                     // The strings
    std::string hp_val, rocket_val, bomb_val;                        // Values
    std::ostringstream hp_sentence, rocket_sentence, bomb_sentence;  // String streams

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

    // Display
    LX_Graphics::LX_BlendedTextImage hp_str_img(hp_info,HUD_SIZE,*hud_font,*win);
    LX_Graphics::LX_BlendedTextImage hp_val_texture(hp_val,HUD_SIZE,*hud_font,*win);

    hp_str_img.setPosition(HUD_XPOS1,HUD_YPOS);
    hp_val_texture.setPosition(HUD_XPOS1,VAL_YPOS);

    hp_str_img.draw();
    hp_val_texture.draw();

    // Display bombs and rockets info
    if(r == S_RANK)
    {
        LX_Graphics::LX_BlendedTextImage rocket_str_img(rocket_info,HUD_SIZE,*hud_font,*win);
        LX_Graphics::LX_BlendedTextImage bomb_str_img(bomb_info,HUD_SIZE,*hud_font,*win);
        LX_Graphics::LX_BlendedTextImage rocket_val_img(rocket_val,HUD_SIZE,*hud_font,*win);
        LX_Graphics::LX_BlendedTextImage bomb_val_img(bomb_val,HUD_SIZE,*hud_font,*win);

        rocket_str_img.setPosition(HUD_XPOS2,HUD_YPOS);
        bomb_str_img.setPosition(HUD_XPOS1 + HUD_XPOS2,HUD_YPOS);
        rocket_val_img.setPosition(HUD_XPOS2,VAL_YPOS);
        bomb_val_img.setPosition(HUD_XPOS1 + HUD_XPOS2,VAL_YPOS);

        rocket_str_img.draw();
        bomb_str_img.draw();
        rocket_val_img.draw();
        bomb_val_img.draw();
    }
}


HUD::~HUD()
{
    delete hud_font;
}

