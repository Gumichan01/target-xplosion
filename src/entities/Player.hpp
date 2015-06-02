

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
*	@file Player.hpp
*	@brief The player class library
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Character.hpp"
#include "../game/hud.hpp"

#include "Item.hpp"

#ifndef PLAYER_SPEED
#define PLAYER_SPEED 9      // the player speed (the same for the two players)
#endif


#ifndef PLAYER_WIDTH
#define PLAYER_WIDTH 64
#endif

#ifndef PLAYER_HEIGHT
#define PLAYER_HEIGHT 64
#endif


#define QUARTER 4
#define HALF 2

#ifndef SHIELD_TIME
#define SHIELD_TIME 10000
#endif

#ifndef HITS_UNDER_SHIELD
#define HITS_UNDER_SHIELD 16
#endif

class Missile;
struct LX_Vector2D;

/**
*   @class Player
*   @brief The player character
*
*   This character gathers all informations about the player
*/
class Player: public Character{

    unsigned int critical_rate;
    unsigned int nb_bomb;
    unsigned int nb_rocket;
    bool bomb_activated;
    bool rocket_activated;
    bool laser_activated;

    bool shield;
    unsigned int nb_hits;           // Maximum number of hits under shield
    double shield_time;             // Time of begining of shield

    unsigned int LIMIT_WIDTH;
    unsigned int LIMIT_HEIGHT;

    LX_Circle hitbox;
    HUD *display;

    void init_hitbox(int x, int y, int w, int h);


    public :

    Player(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic,
            SDL_Texture *image, LX_Chunk *audio,
                int x, int y, int w, int h,int dX, int dY,
                    unsigned int w_limit, unsigned h_limit);

    Player(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic,
                SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp,
                    unsigned int w_limit, unsigned h_limit);

    void fire(MISSILE_TYPE m_type);
    Missile * shoot(MISSILE_TYPE m_type);
    void move();
    void die();
    void receive_damages(unsigned int attacks);
    void updateHUD(){display->display_HUD();}

    void takeBonus(POWER_UP powerUp);

    void healQuarter(void);
    void healHalf(void);
    void set_shield(bool sh);

    void collision(Missile *mi);

    unsigned int getBomb();
    unsigned int getRocket();

    LX_Circle * get_hitbox();
    bool isLaser_activated();

    ~Player();

};


#endif // PLAYER_H_INCLUDED


