#ifndef BOSS00_HPP_INCLUDED
#define BOSS00_HPP_INCLUDED



/*
*   Target_Xplosion - The classic shoot'em up video game
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
*	@file SemiBoss01.hpp
*	@brief The header file of SemiBoss01
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Boss.hpp"


class SemiBoss01 : public Boss
{
    SDL_Rect sprite[7];
    SDL_Surface *shot_surface;

    void bossInit(void);
    bool canShoot(void);

public :

    SemiBoss01(unsigned int hp, unsigned int att, unsigned int sh,
           SDL_Texture *image, LX_Chunk *audio,
           Sint16 x, Sint16 y, Uint16 w, Uint16 h,float vx, float vy);

    void reaction(Missile *target);
    //void die();
    void strategy(void);
    Missile * shoot(MISSILE_TYPE m_type);
    SDL_Rect * getAreaToDisplay();

    ~SemiBoss01();

};


class SemiBoss01ShootStrat : public Strategy
{
    unsigned int shot_delay;
    unsigned int begin_time;

public :

    SemiBoss01ShootStrat(Enemy * newEnemy);

    void proceed(void);
    void fire(MISSILE_TYPE m_type);

    ~SemiBoss01ShootStrat();
};

#endif // BOSS00_HPP_INCLUDED

