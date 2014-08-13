#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED



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
*	@file Character.h
*	@brief The Character header
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date July 7th, 2014
*
*/

#include <cstdio>
#include <cstdlib>

#include "../engine/LX_graphics.h"
#include "../entities/Rocket.h"

#include "Entity.h"
#include "Basic_missile.h"

#ifndef PLAYER_WIDTH
#define PLAYER_WIDTH 64
#endif

#ifndef PLAYER_HEIGHT
#define PLAYER_HEIGHT 64
#endif



/**
*   @class Character
*   @brief The character entity
*
*   It gathers the player and all ennemies
*/
class Character: public Entity{

    protected :

    unsigned int health_point;
    unsigned int max_health_point;
    unsigned int attack_val;
    unsigned int shield;


    public :


/**
*   DOC
*
*
*/
    Character(unsigned int hp, unsigned int att, unsigned int sh,
                SDL_Surface *image, Mix_Chunk *audio,
                    int x, int y, int w, int h,int dX, int dY)
        : Entity(image, audio, x, y, w, h, dX, dY)
    {
        health_point = hp;
        max_health_point = hp;
        attack_val = att;
        shield = sh;

    }

 /**
*   DOC
*
*
*/
    Character(unsigned int hp, unsigned int att, unsigned int sh,
                SDL_Surface *image, Mix_Chunk *audio, SDL_Rect *rect,Speed *sp)
        : Entity(image, audio, rect, sp)
    {
        health_point = hp;
        max_health_point = hp;
        attack_val = att;
        shield = sh;

    }


    // Operations

    void receive_damages(unsigned int attacks);
    virtual Missile * shoot(MISSILE_TYPE m_type) = 0;


    // Getters

/**
*   DOC
*
*
*/
    unsigned int getHP(){return health_point;}

/**
*   DOC
*
*
*/
    unsigned int getMAX_HP(){return max_health_point;}

/**
*   DOC
*
*
*/
    unsigned int getATT(){return attack_val;}

/**
*   DOC
*
*
*/
    unsigned int getDEF(){return shield;}

    //Setters

/**
*   DOC
*
*
*/
    void setHP(unsigned int newHP);

/**
*   DOC
*
*
*/
    void setMAX_HP(unsigned int newMAX_HP){max_health_point = newMAX_HP;}

/**
*   DOC
*
*
*/
    void setATT(unsigned int newATT){attack_val = newATT;}

/**
*   DOC
*
*
*/
    void setDEF(unsigned int newDEF){shield = newDEF;}

    virtual ~Character(){}

};






#endif // CHARACTER_H_INCLUDED












