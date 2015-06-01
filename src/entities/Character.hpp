

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
*
*/



#include "Entity.hpp"

#include "Missile.hpp"


#ifndef LASER_LIFETIME
#define LASER_LIFETIME  128
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
    unsigned int laser_delay;
    bool wasKilled;

    void characterInit(unsigned int hp, unsigned int att, unsigned int sh);

    public :


    Character(unsigned int hp, unsigned int att, unsigned int sh,
                SDL_Texture *image, LX_Chunk *audio,
                    int x, int y, int w, int h,int dX, int dY);


    Character(unsigned int hp, unsigned int att, unsigned int sh,
                SDL_Texture *image, LX_Chunk *audio, SDL_Rect *rect,Speed *sp);


    virtual void receive_damages(unsigned int attacks);
    virtual Missile * shoot(MISSILE_TYPE m_type) = 0;
    virtual void collision(Missile *mi) = 0;
    void kill(void);


    unsigned int getHP(){return health_point;}
    unsigned int getMAX_HP(){return max_health_point;}
    unsigned int getATT(){return attack_val;}
    unsigned int getDEF(){return shield;}
    bool killed(){return wasKilled;}


    void setHP(unsigned int newHP);
    void setMAX_HP(unsigned int newMAX_HP){max_health_point = newMAX_HP;}
    void setATT(unsigned int newATT){attack_val = newATT;}
    void setDEF(unsigned int newDEF){shield = newDEF;}

    virtual ~Character(){}

};




#endif // CHARACTER_H_INCLUDED












