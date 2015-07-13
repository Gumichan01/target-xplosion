#ifndef TOWER_HPP_INCLUDED
#define TOWER_HPP_INCLUDED



/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2015  Luxon Jean-Pierre
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

/**
*	@file Tower.hpp
*	@brief The Tower(s) class
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Enemy.hpp"


class Tower1 : public Enemy
{

public:


    Tower1(unsigned int hp, unsigned int att, unsigned int sh,
           SDL_Texture *image, LX_Chunk *audio,
           Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY);

    Tower1(unsigned int hp, unsigned int att, unsigned int sh,
           SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp);

    Missile * shoot(MISSILE_TYPE m_type);
    void init_box(void);

    ~Tower1();

};

// Strategy of Tower1
class Tower1_Strat: public Strategy
{
    double rtime;

public:

    Tower1_Strat(Enemy *newEnemy);

    void proceed(void);
    void fire(MISSILE_TYPE m_type);

    ~Tower1_Strat();
};


#endif // TOWER_HPP_INCLUDED

