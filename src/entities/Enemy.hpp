

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2015 Luxon Jean-Pierre
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
*	@file Enemy.hpp
*	@brief The enemy class
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Character.hpp"
#include "Strategy.hpp"



class Player;

class Enemy: public Character
{

protected:

    //double delay_rocket;      // The delay between two rocket shot
    //double delay_bomb;        // The delay between two basic missiles shot

    LX_AABB box;
    Strategy *strat;

public:

    Enemy(unsigned int hp, unsigned int att, unsigned int sh,
          SDL_Texture *image, LX_Chunk *audio,
          Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY);


    Enemy(unsigned int hp, unsigned int att, unsigned int sh,
          SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp);

    void move(void);
    void strategy(void);
    void receive_damages(unsigned int attacks);
    virtual void reaction(Missile *target);

    virtual void collision(Missile *mi);
    virtual void collision(Player *play);

    void addStrategy(Strategy *newStrat);
    void deleteStrategy();

    LX_AABB * get_hitbox();

    virtual ~Enemy()
    {
        delete strat;
    }

};


#endif // ENEMY_H_INCLUDED



