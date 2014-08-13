#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED



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
*	@file Player.h
*	@brief The player class library
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date July 11th, 2014
*
*
*/

#include "Character.h"

//#include "Basic_missile.h"


/**
*   @class Player
*   @brief The player character
*
*   This character gathers all informations about the player
*/
class Player: public Character {


    unsigned int critical_rate;
    unsigned int nb_bomb;
    bool double_missile_activated;
    bool is_invicible;
    LX_Circle hitbox;

    void init_hitbox(int x, int y, int w, int h);


    public :

/**
* DOC
*
*
*/
    Player(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic, unsigned int bombs,
            SDL_Surface *image, Mix_Chunk *audio,
                int x, int y, int w, int h,int dX, int dY)
        : Character(hp, att, sh, image, audio, x, y, w, h, dX, dY)
    {
        critical_rate = critic;
        nb_bomb = bombs;
        is_invicible = false;
        double_missile_activated = false;

        init_hitbox(x,y,w,h);
    }

/**
* DOC
*
*
*/
    Player(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic, unsigned int bombs,
            SDL_Surface *image, Mix_Chunk *audio,SDL_Rect *rect,Speed *sp)
        : Character(hp, att, sh, image, audio, rect, sp)
    {
        critical_rate = critic;
        nb_bomb = bombs;
        is_invicible = false;
        double_missile_activated = false;

        init_hitbox(rect->x,rect->y,rect->w,rect->h);
    }

    Missile * shoot(MISSILE_TYPE m_type);
    void move();

    LX_Circle * get_hitbox(){return &hitbox;}

    ~Player(){}

};








#endif // PLAYER_H_INCLUDED























