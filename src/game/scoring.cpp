
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
*	@file scoring.cpp
*	@brief The scoring file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <string>
#include <iostream>
#include <sstream>

#include <SDL2/SDL.h>

#include "scoring.hpp"
#include "../engine/LX_Graphics.hpp"
#include "../engine/LX_TrueTypeFont.hpp"
#include "../engine/LX_WindowManager.hpp"
#include "../engine/LX_Window.hpp"

using namespace LX_Graphics;
using namespace LX_TrueTypeFont;


Score::Score(unsigned int ps)
{
    previous_score = ps;
    current_score = 0;
    total_score = previous_score;
    score_font = new LX_Font(NULL);
}



void Score::notify(int newScore)
{
    current_score += newScore;
    total_score += newScore;
}



void Score::display(void)
{
    std::ostringstream score_sentence;  // The output string
    std::string score_str;              // The score string
    std::string score_val;              // The score value
    int w,h;

    SDL_Rect pos_score_str = {1,1,1,1};
    SDL_Rect pos_score_val = {1,32,1,1};

    score_sentence << "Score";
    score_str = score_sentence.str();
    score_sentence.str("");
    score_sentence << current_score;
    score_val = score_sentence.str();

    SDL_Texture *score_str_surface = score_font->drawTextToTexture(LX_TTF_BLENDED,score_str.c_str(),VAL_SCORE_SIZE,0);
    SDL_Texture *score_val_surface = score_font->drawTextToTexture(LX_TTF_BLENDED,score_val.c_str(),VAL_SCORE_SIZE,0);

    // Get sizes of the text to display
    score_font->sizeOfText(score_str.c_str(),&w,&h);
    pos_score_str.w = w;
    pos_score_str.h = h;

    score_font->sizeOfText(score_val.c_str(),&w,&h);
    pos_score_val.w = w;
    pos_score_val.h = h;

    // Put textures

    LX_WindowManager::getInstance()->getWindow(0)->putTexture(score_str_surface,NULL,&pos_score_str);
    LX_WindowManager::getInstance()->getWindow(0)->putTexture(score_val_surface,NULL,&pos_score_val);

    SDL_DestroyTexture(score_str_surface);
    SDL_DestroyTexture(score_val_surface);

}


Score::~Score()
{
    delete score_font;
}

