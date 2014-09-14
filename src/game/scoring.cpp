

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
*	@version 0.2
*	@date September 14th, 2014
*
*
*/

#include "scoring.h"
#include "../engine/LX_graphics.h"
#include "../engine/LX_ttf.h"

#include <string>
#include <iostream>
#include <sstream>


void Score::display(void)
{
    std::ostringstream score_sentence;  // The output string

    std::string score_str;              // The score string
    std::string score_val;              // The score value

    SDL_Rect pos_score_str = {1,1,1,1};
    SDL_Rect pos_score_val = {1,32,1,1};

    score_sentence << "Score";
    score_str = score_sentence.str();
    score_sentence.str("");
    score_sentence << current_score;
    score_val = score_sentence.str();

    // Load the current instances of LX_TTF and LX_graphics
    LX_ttf *tmp_ttf = LX_ttf::getInstance();
    LX_graphics *tmp_graph = LX_graphics::getInstance();

    SDL_Surface *score_str_surface = tmp_ttf->draw_BlendedText_WithSize(score_str,VAL_SCORE_SIZE);
    SDL_Surface *score_val_surface = tmp_ttf->draw_BlendedText_WithSize(score_val,VAL_SCORE_SIZE);

    tmp_graph->put_image(score_str_surface,NULL,&pos_score_str);
    tmp_graph->put_image(score_val_surface,NULL,&pos_score_val);

}






























