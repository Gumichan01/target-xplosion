
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

#include <string>
#include <iostream>
#include <sstream>

#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Window.hpp>

#include "hud.hpp"
#include "../entities/Player.hpp"

#include <LunatiX/LX_TrueTypeFont.hpp>

using namespace LX_TrueTypeFont;


HUD::HUD(Player * sub)
{
    subject = sub;

    player_hp = subject->getHP();
    player_hp_max = subject->getHP();
    player_bombs = subject->getBomb();
    player_rockets = subject->getRocket();
    hud_font = new LX_Font(NULL);
}


void HUD::update()
{
    player_hp = subject->getHP();
    player_bombs = subject->getBomb();
    player_rockets = subject->getRocket();
}


void HUD::display_HUD()
{
    std::ostringstream hp_sentence;
    std::ostringstream rocket_sentence;
    std::ostringstream bomb_sentence;
    LX_Window *win = LX_Graphics::LX_WindowManager::getInstance()->getWindow(0);

    if(win == NULL)
    {
        return;
    }

    // The strings
    std::string hp_info;
    std::string rocket_info;
    std::string bomb_info;

    // Values
    std::string hp_val;
    std::string rocket_val;
    std::string bomb_val;

    int w,h;

    // Getting the strings
    hp_sentence << "Health";
    rocket_sentence << "Rockets";
    bomb_sentence << "Bomb";

    hp_info = hp_sentence.str();
    rocket_info = rocket_sentence.str();
    bomb_info = bomb_sentence.str();

    //clear the content
    hp_sentence.str("");
    rocket_sentence.str("");
    bomb_sentence.str("");


    //Getting the values
    hp_sentence << player_hp << "/" << player_hp_max;
    rocket_sentence << player_rockets;
    bomb_sentence << player_bombs;

    hp_val = hp_sentence.str();
    rocket_val = rocket_sentence.str();
    bomb_val = bomb_sentence.str();


    /* ******************* *
     * Create the display  *
     * ******************* */


    /* Positions */
    // The strings positions
    SDL_Rect pos_hp_str = {800/QUARTER,TOP,ZERO,ZERO};
    SDL_Rect pos_rocket_str = {800/HALF,TOP,ZERO,ZERO};
    SDL_Rect pos_bomb_str = {800/QUARTER + 800/HALF,TOP,ZERO,ZERO};

    // The values positions
    SDL_Rect pos_hp_val = {(Sint16) pos_hp_str.x,VAL_YPOS,ZERO,ZERO};
    SDL_Rect pos_rocket_val = {(Sint16)pos_rocket_str.x,VAL_YPOS,ZERO,ZERO};
    SDL_Rect pos_bomb_val = {(Sint16)pos_bomb_str.x,VAL_YPOS,ZERO,ZERO};

    // The surfaces
    SDL_Texture *hp_str_texture = hud_font->drawTextToTexture(LX_TTF_BLENDED,hp_info.c_str(),HUD_SIZE,0);
    SDL_Texture *rocket_str_texture = hud_font->drawTextToTexture(LX_TTF_BLENDED,rocket_info.c_str(),HUD_SIZE,0);
    SDL_Texture *bomb_str_texture = hud_font->drawTextToTexture(LX_TTF_BLENDED,bomb_info.c_str(),HUD_SIZE,0);

    SDL_Texture *hp_val_texture = hud_font->drawTextToTexture(LX_TTF_BLENDED,hp_val.c_str(),HUD_SIZE,0);
    SDL_Texture *rocket_val_texture = hud_font->drawTextToTexture(LX_TTF_BLENDED,rocket_val.c_str(),HUD_SIZE,0);
    SDL_Texture *bomb_val_texture = hud_font->drawTextToTexture(LX_TTF_BLENDED,bomb_val.c_str(),HUD_SIZE,0);


    // Get sizes of the text to display
    hud_font->sizeOfText(hp_info.c_str(),&w,&h);
    pos_hp_str.w = w;
    pos_hp_str.h = h;

    hud_font->sizeOfText(rocket_info.c_str(),&w,&h);
    pos_rocket_str.w = w;
    pos_rocket_str.h = h;

    hud_font->sizeOfText(bomb_info.c_str(),&w,&h);
    pos_bomb_str.w = w;
    pos_bomb_str.h = h;

    hud_font->sizeOfText(hp_val.c_str(),&w,&h);
    pos_hp_val.w = w;
    pos_hp_val.h = h;

    hud_font->sizeOfText(rocket_val.c_str(),&w,&h);
    pos_rocket_val.w = w;
    pos_rocket_val.h = h;

    hud_font->sizeOfText(bomb_val.c_str(),&w,&h);
    pos_bomb_val.w = w;
    pos_bomb_val.h = h;



    // Put all texts on the screen
    win->putTexture(hp_str_texture,NULL,&pos_hp_str);
    win->putTexture(rocket_str_texture,NULL,&pos_rocket_str);
    win->putTexture(bomb_str_texture,NULL,&pos_bomb_str);

    win->putTexture(hp_val_texture,NULL,&pos_hp_val);
    win->putTexture(rocket_val_texture,NULL,&pos_rocket_val);
    win->putTexture(bomb_val_texture,NULL,&pos_bomb_val);

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
