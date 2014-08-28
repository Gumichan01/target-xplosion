

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


#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


/**
*	@file Character.h
*	@brief The Character header
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date July 7th, 2014
*
*/


#include "../engine/LX_graphics.h"

#include "Entity.h"
#include "Basic_missile.h"
#include "Rocket.h"
#include "Laser.h"



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












