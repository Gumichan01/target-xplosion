

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

#include "hud.h"
#include "../entities/Player.h"

HUD::HUD(Player * sub)
{
    subject = sub;

    player_hp = subject->getHP();
    player_hp_max = subject->getHP();
    player_bombs = subject->getBomb();
    player_rockets = subject->getRocket();
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

    // Load the current instances of LX_TTF and LX_graphics
    LX_ttf *tmp_ttf = LX_ttf::getInstance();
    LX_graphics *tmp_graph = LX_graphics::getInstance();

    // The surfaces
    SDL_Surface *hp_str_surface = tmp_ttf->draw_BlendedText_WithSize(hp_info,HUD_SIZE);
    SDL_Surface *rocket_str_surface = tmp_ttf->draw_BlendedText_WithSize(rocket_info,HUD_SIZE);
    SDL_Surface *bomb_str_surface = tmp_ttf->draw_BlendedText_WithSize(bomb_info,HUD_SIZE);

    SDL_Surface *hp_val_surface = tmp_ttf->draw_BlendedText_WithSize(hp_val,HUD_SIZE);
    SDL_Surface *rocket_val_surface = tmp_ttf->draw_BlendedText_WithSize(rocket_val,HUD_SIZE);
    SDL_Surface *bomb_val_surface = tmp_ttf->draw_BlendedText_WithSize(bomb_val,HUD_SIZE);


    // Put all texts on the screen
    tmp_graph->put_image(hp_str_surface,NULL,&pos_hp_str);
    tmp_graph->put_image(rocket_str_surface,NULL,&pos_rocket_str);
    tmp_graph->put_image(bomb_str_surface,NULL,&pos_bomb_str);

    tmp_graph->put_image(hp_val_surface,NULL,&pos_hp_val);
    tmp_graph->put_image(rocket_val_surface,NULL,&pos_rocket_val);
    tmp_graph->put_image(bomb_val_surface,NULL,&pos_bomb_val);

    SDL_FreeSurface(hp_str_surface);
    SDL_FreeSurface(rocket_str_surface);
    SDL_FreeSurface(bomb_str_surface);

    SDL_FreeSurface(hp_val_surface);
    SDL_FreeSurface(rocket_val_surface);
    SDL_FreeSurface(bomb_val_surface);

}











