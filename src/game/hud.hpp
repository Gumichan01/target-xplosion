#ifndef HUD2_H_INCLUDED
#define HUD2_H_INCLUDED


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
*	@file hud.hpp
*	@brief The head-up display file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Observer.hpp"


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

namespace LX_TrueTypeFont{

class LX_Font;

};

using namespace LX_TrueTypeFont;



class HUD : public Observer
{

    unsigned int player_hp_max;
    unsigned int player_hp;
    unsigned int player_rockets;
    unsigned int player_bombs;

    Player *subject;
    LX_Font *hud_font;


    public :

    HUD(Player * sub);

    void update();
    void display_HUD();

    ~HUD();

};





#endif // HUD2_H_INCLUDED


