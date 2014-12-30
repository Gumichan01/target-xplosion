

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


#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED


/**
*	@file Enemy.h
*	@brief The enemy class
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date August 8th, 2014
*
*
*
*/

#include "Character.h"
#include "Strategy.h"

//#include "../game/Game.h"

class Player;

class Enemy: public Character{

    protected:

    //double delay_rocket;      // The delay between two rocket shot
    //double delay_bomb;        // The delay between two basic missiles shot

    LX_AABB box;
    Strategy *strat;

    public:

    Enemy(unsigned int hp, unsigned int att, unsigned int sh,
            SDL_Surface *image, Mix_Chunk *audio,
                Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
        : Character(hp,att,sh,image, audio, x, y, w, h, dX, dY)
    {
        box = {x,y,w,h};
        wasKilled = false;

    }


    Enemy(unsigned int hp, unsigned int att, unsigned int sh,
            SDL_Surface *image, Mix_Chunk *audio,SDL_Rect *rect,Speed *sp)
        : Character(hp,att,sh,image, audio, rect, sp)
    {
        box = {rect->x,rect->y,rect->w,rect->h};
        wasKilled = false;

    }

    void move(void);
    void strategy(void);
    void receive_damages(unsigned int attacks);
    virtual void reaction(Missile *target);

    virtual void collision(Missile *mi);
    virtual void collision(Player *play);

    void addStrategy(Strategy *newStrat);
    void deleteStrategy();

    LX_AABB * get_hitbox(){return &box;}

    virtual ~Enemy()
    {
        delete strat;
    }

};


#endif // ENEMY_H_INCLUDED









