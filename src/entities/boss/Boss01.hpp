#ifndef BOSS01_HPP_INCLUDED
#define BOSS01_HPP_INCLUDED


/*
*   Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
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
*	@file Boss01.hpp
*	@brief The header file of Boss01
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Boss.hpp"


class Boss01 : public Boss
{
    int idStrat;
    Uint32 wallTime;
    Uint32 rowTime;
    SDL_Rect sprite[16];

    void bossInit(void);
    void rowShot();
    void wallShot();

public :

    explicit Boss01(unsigned int hp, unsigned int att, unsigned int sh,
                    SDL_Texture *image, LX_Mixer::LX_Chunk *audio,
                    int x, int y, int w, int h,float vx, float vy);

    void die();
    void shoot();
    void strategy(void);
    void propulsion(void);
    void shoot(MISSILE_TYPE m_type);
    SDL_Rect * getAreaToDisplay();

    ~Boss01();
};


/* Position strategy */
class Boss01PositionStrat : virtual public BossStrategy
{
public:

    explicit Boss01PositionStrat(Boss01 * newEnemy);

    void proceed(void);
    void fire(MISSILE_TYPE m_type);

    ~Boss01PositionStrat();
};


/* Wall shoot strategy */
class Boss01WallStrat : virtual public BossStrategy
{
    Uint32 beginWall;
    int first;

public:

    explicit Boss01WallStrat(Boss01 * newEnemy);

    void proceed(void);
    void fire(MISSILE_TYPE m_type);

    ~Boss01WallStrat();
};


/* Row shoot strategy */
class Boss01RowStrat : virtual public BossStrategy
{
    Uint32 beginRow;
    int first;

public:

    explicit Boss01RowStrat(Boss01 * newEnemy);

    void proceed(void);
    void fire(MISSILE_TYPE m_type);

    ~Boss01RowStrat();
};

#endif // BOSS01_HPP_INCLUDED

