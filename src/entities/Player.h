

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2014  Luxon Jean-Pierre
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


#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED


/**
*	@file Player.h
*	@brief The player class library
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date July 11th, 2014
*
*
*/

#include "Character.h"
#include "../game/hud.h"

#include "Item.h"

#ifndef PLAYER_SPEED
#define PLAYER_SPEED 8      // the player speed (the same for the two players)
#endif


#ifndef PLAYER_WIDTH
#define PLAYER_WIDTH 64
#endif

#ifndef PLAYER_HEIGHT
#define PLAYER_HEIGHT 64
#endif


#define QUARTER 4
#define HALF 2


/**
*   @class Player
*   @brief The player character
*
*   This character gathers all informations about the player
*/
class Player: public Character{

    unsigned int critical_rate;     /// @todo integrate the critical rate
    unsigned int nb_bomb;
    unsigned int nb_rocket;
    bool rocket_activated;
    bool laser_activated;
    bool is_invicible;
    LX_Circle hitbox;

    unsigned int LIMIT_WIDTH;
    unsigned int LIMIT_HEIGHT;

    HUD *display;

    void init_hitbox(int x, int y, int w, int h);


    public :


    Player(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic,
            SDL_Surface *image, Mix_Chunk *audio,
                int x, int y, int w, int h,int dX, int dY, unsigned int w_limit, unsigned h_limit)
        : Character(hp, att, sh, image, audio, x, y, w, h, dX, dY)
    {
        critical_rate = critic;
        nb_bomb = 0;
        nb_rocket = 0;
        is_invicible = false;
        rocket_activated = false;
        laser_activated = false;

        LIMIT_WIDTH = w_limit;
        LIMIT_HEIGHT = h_limit;

        display = new HUD(this);

        init_hitbox(x,y,w,h);
    }


    Player(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic,
                SDL_Surface *image, Mix_Chunk *audio,SDL_Rect *rect,Speed *sp,
                    unsigned int w_limit, unsigned h_limit)
        : Character(hp, att, sh, image, audio, rect, sp)
    {
        critical_rate = critic;
        nb_bomb = 0;
        nb_rocket = 0;
        is_invicible = false;
        rocket_activated = false;
        laser_activated = false;

        LIMIT_WIDTH = w_limit;
        LIMIT_HEIGHT = h_limit;

        display = new HUD(this);

        init_hitbox(rect->x,rect->y,rect->w,rect->h);
    }

    void fire(MISSILE_TYPE m_type);
    Missile * shoot(MISSILE_TYPE m_type);
    void move();
    void die();
    void receive_damages(unsigned int attacks);
    void updateHUD(){display->display_HUD();}

    void takeBonus(POWER_UP powerUp);

    void healQuarter(void);
    void healHalf(void);


    unsigned int getBomb(){return nb_bomb;}
    unsigned int getRocket(){return nb_rocket;}

    LX_Circle * get_hitbox(){return &hitbox;}
    bool isLaser_activated(){return laser_activated;}

    ~Player()
    {
        delete display;
    }

};



#endif // PLAYER_H_INCLUDED























