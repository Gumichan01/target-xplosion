#ifndef BOSS01_HPP_INCLUDED
#define BOSS01_HPP_INCLUDED



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
*	@file Boss01.hpp
*	@brief The header of the Boss
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "../Enemy.hpp"


class Boss01 : public Enemy
{
    //bool dying;
    double ref_timeB;
    int idStrat;
    Uint32 wallTime;
    Uint32 rowTime;
    //SDL_Rect sprite[7];

    void init(void);

public :

    Boss01(unsigned int hp, unsigned int att, unsigned int sh,
           SDL_Texture *image, LX_Chunk *audio,
           Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY);

    Boss01(unsigned int hp, unsigned int att, unsigned int sh,
           SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp);

    void move(void);
    void reaction(Missile *target);
    void die();
    void strategy(void);
    Missile * shoot(MISSILE_TYPE m_type);
    SDL_Rect * getAreaToDisplay();

    ~Boss01();

};


/* Position strategy */
class Boss01_PositionStrat : public Strategy
{

public:

    Boss01_PositionStrat(Enemy * newEnemy);

    void proceed(void);
    void fire(MISSILE_TYPE m_type);

    ~Boss01_PositionStrat();
};


/* Wall shoot strategy */
class Boss01_WallStrat : public Strategy
{

    Uint32 beginWall;

public:

    Boss01_WallStrat(Enemy * newEnemy);

    void proceed(void);
    void fire(MISSILE_TYPE m_type);

    ~Boss01_WallStrat();
};



/* Row shoot strategy */
class Boss01_RowStrat : public Strategy
{

    Uint32 beginRow;

public:

    Boss01_RowStrat(Enemy * newEnemy);

    void proceed(void);
    void fire(MISSILE_TYPE m_type);

    ~Boss01_RowStrat();
};



#endif // BOSS01_HPP_INCLUDED

