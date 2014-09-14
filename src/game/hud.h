#ifndef HUD2_H_INCLUDED
#define HUD2_H_INCLUDED



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
*	@file hud.h
*	@brief The head-up display file
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.2
*	@date September 14th, 2014
*
*
*
*/

#include "../engine/LX_graphics.h"
#include "../engine/LX_ttf.h"
#include "Observer.h"


#include <string>
#include <iostream>
#include <sstream>


#define ZERO 0          // to put width and height to 0
#define TOP 1
#define LEFT 1
#define VAL_YPOS 32
#define QUARTER 4
#define HALF 2
#define HUD_SIZE 24     // The font size of the HUD texts


class Player;

class HUD : public Observer
{

    Player *subject;
    unsigned int player_hp_max;
    unsigned int player_hp;
    unsigned int player_rockets;
    unsigned int player_bombs;


    public :

    HUD(Player * sub);

    void update();

    void display_HUD();

    ~HUD(){}

};





#endif // HUD2_H_INCLUDED


