

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

#include "../pattern/Strategy.hpp"
#include "Character.hpp"


class Player;
struct LX_Circle;
struct SDL_Surface;

class Enemy: public Character
{

protected:

    Strategy *strat;

    SDL_Surface ** getResources();

public:

    Enemy(unsigned int hp, unsigned int att, unsigned int sh,
          SDL_Texture *image, LX_Chunk *audio,
          Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY);

    static void createMissileRessources();
    static void destroyMissileRessources();

    void move(void);
    virtual void strategy(void);
    void receiveDamages(unsigned int attacks);
    virtual void reaction(Missile *target);

    virtual void collision(Missile *mi);
    virtual void collision(Player *play);

    void addStrategy(Strategy *new_strat);
    void deleteStrategy();

    const LX_Circle * getHitbox();

    virtual ~Enemy();

};


#endif // ENEMY_H_INCLUDED

