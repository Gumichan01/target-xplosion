

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


#ifndef PLAYER_WIDTH
#define PLAYER_WIDTH 64
#endif

#ifndef PLAYER_HEIGHT
#define PLAYER_HEIGHT 64
#endif


#ifndef LASER_LIFETIME
#define LASER_LIFETIME  128
#endif


/**
*   @class Player
*   @brief The player character
*
*   This character gathers all informations about the player
*/
class Player: public Character {


    unsigned int critical_rate;     /// @todo integrate the critical
    unsigned int nb_bomb;           /// @todo integrate the number of bombs
    unsigned int laser_delay;
    bool rocket_activated;          /// check if the player can use the rockets
    bool laser_activated;
    bool is_invicible;
    LX_Circle hitbox;

    unsigned int LIMIT_WIDTH;
    unsigned int LIMIT_HEIGHT;

    void init_hitbox(int x, int y, int w, int h);


    public :

/**
* DOC
*
*
*/
    Player(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic, unsigned int bombs,
            SDL_Surface *image, Mix_Chunk *audio,
                int x, int y, int w, int h,int dX, int dY, unsigned int w_limit, unsigned h_limit)
        : Character(hp, att, sh, image, audio, x, y, w, h, dX, dY)
    {
        critical_rate = critic;
        nb_bomb = bombs;
        is_invicible = false;
        rocket_activated = false;
        laser_activated = false;

        laser_delay = LASER_LIFETIME;

        LIMIT_WIDTH = w_limit;
        LIMIT_HEIGHT = h_limit;

        init_hitbox(x,y,w,h);
    }

/**
* DOC
*
*
*/
    Player(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic, unsigned int bombs,
            SDL_Surface *image, Mix_Chunk *audio,SDL_Rect *rect,Speed *sp, unsigned int w_limit, unsigned h_limit)
        : Character(hp, att, sh, image, audio, rect, sp)
    {
        critical_rate = critic;
        nb_bomb = bombs;
        is_invicible = false;
        rocket_activated = false;
        laser_activated = false;

        laser_delay = LASER_LIFETIME;

        LIMIT_WIDTH = w_limit;
        LIMIT_HEIGHT = h_limit;

        init_hitbox(rect->x,rect->y,rect->w,rect->h);
    }

    Missile * shoot(MISSILE_TYPE m_type);
    void move();

    LX_Circle * get_hitbox(){return &hitbox;}
    bool isLaser_activated(){return laser_activated;}

    ~Player(){}

};








#endif // PLAYER_H_INCLUDED























