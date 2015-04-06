

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
*	@file hud.cpp
*	@brief The HUD file
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.2
*	@date September 14th, 2014
*
*
*
*/

#include <SDL2/SDL.h>

#include "hud.hpp"
#include "../entities/Player.hpp"

HUD::HUD(Player * sub)
{
    subject = sub;

    player_hp = subject->getHP();
    player_hp_max = subject->getHP();
    player_bombs = subject->getBomb();
    player_rockets = subject->getRocket();
    tmp_ttf = new LX_TrueTypeFont(NULL);
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

    // The strings
    // Informations
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

    // Load the current instances of LX_TTF and LX_Graphics
    LX_Graphics *tmp_graph = LX_Graphics::getInstance();

    // The surfaces
    SDL_Texture *hp_str_texture = tmp_ttf->drawTextToTexture(LX_TTF_BLENDED,hp_info.c_str(),HUD_SIZE);
    SDL_Texture *rocket_str_texture = tmp_ttf->drawTextToTexture(LX_TTF_BLENDED,rocket_info.c_str(),HUD_SIZE);
    SDL_Texture *bomb_str_texture = tmp_ttf->drawTextToTexture(LX_TTF_BLENDED,bomb_info.c_str(),HUD_SIZE);

    SDL_Texture *hp_val_texture = tmp_ttf->drawTextToTexture(LX_TTF_BLENDED,hp_val.c_str(),HUD_SIZE);
    SDL_Texture *rocket_val_texture = tmp_ttf->drawTextToTexture(LX_TTF_BLENDED,rocket_val.c_str(),HUD_SIZE);
    SDL_Texture *bomb_val_texture = tmp_ttf->drawTextToTexture(LX_TTF_BLENDED,bomb_val.c_str(),HUD_SIZE);


    // Get sizes of the text to display
    tmp_ttf->sizeOfText(hp_info.c_str(),&w,&h);
    pos_hp_str.w = w;
    pos_hp_str.h = h;

    tmp_ttf->sizeOfText(rocket_info.c_str(),&w,&h);
    pos_rocket_str.w = w;
    pos_rocket_str.h = h;

    tmp_ttf->sizeOfText(bomb_info.c_str(),&w,&h);
    pos_bomb_str.w = w;
    pos_bomb_str.h = h;

    tmp_ttf->sizeOfText(hp_val.c_str(),&w,&h);
    pos_hp_val.w = w;
    pos_hp_val.h = h;

    tmp_ttf->sizeOfText(rocket_val.c_str(),&w,&h);
    pos_rocket_val.w = w;
    pos_rocket_val.h = h;

    tmp_ttf->sizeOfText(bomb_val.c_str(),&w,&h);
    pos_bomb_val.w = w;
    pos_bomb_val.h = h;

    // Put all texts on the screen
    tmp_graph->putTexture(hp_str_texture,NULL,&pos_hp_str);
    tmp_graph->putTexture(rocket_str_texture,NULL,&pos_rocket_str);
    tmp_graph->putTexture(bomb_str_texture,NULL,&pos_bomb_str);

    tmp_graph->putTexture(hp_val_texture,NULL,&pos_hp_val);
    tmp_graph->putTexture(rocket_val_texture,NULL,&pos_rocket_val);
    tmp_graph->putTexture(bomb_val_texture,NULL,&pos_bomb_val);

    SDL_DestroyTexture(hp_str_texture);
    SDL_DestroyTexture(rocket_str_texture);
    SDL_DestroyTexture(bomb_str_texture);

    SDL_DestroyTexture(hp_val_texture);
    SDL_DestroyTexture(rocket_val_texture);
    SDL_DestroyTexture(bomb_val_texture);

}











