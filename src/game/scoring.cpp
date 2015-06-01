

/*
*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*
*/

/**
*	@file scoring.cpp
*	@brief The scoring file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL.h>

#include "scoring.hpp"
#include "../engine/LX_Graphics.hpp"
#include "../engine/LX_TrueTypeFont.hpp"

#include <string>
#include <iostream>
#include <sstream>



Score::Score(unsigned int ps)
{
    previous_score = ps;
    current_score = 0;
    total_score = previous_score;
    tmp_ttf = new LX_TrueTypeFont(NULL);
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

    // Load the current instances of LX_Graphics
    LX_Graphics *tmp_graph = LX_Graphics::getInstance();
    SDL_Texture *score_str_surface = tmp_ttf->drawTextToTexture(LX_TTF_BLENDED,score_str.c_str(),VAL_SCORE_SIZE);
    SDL_Texture *score_val_surface = tmp_ttf->drawTextToTexture(LX_TTF_BLENDED,score_val.c_str(),VAL_SCORE_SIZE);

    // Get sizes of the text to display
    tmp_ttf->sizeOfText(score_str.c_str(),&w,&h);
    pos_score_str.w = w;
    pos_score_str.h = h;

    tmp_ttf->sizeOfText(score_val.c_str(),&w,&h);
    pos_score_val.w = w;
    pos_score_val.h = h;

    // Put textures
    tmp_graph->putTexture(score_str_surface,NULL,&pos_score_str);
    tmp_graph->putTexture(score_val_surface,NULL,&pos_score_val);

    SDL_DestroyTexture(score_str_surface);
    SDL_DestroyTexture(score_val_surface);

}






























