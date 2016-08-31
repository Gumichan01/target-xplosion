
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/

#ifndef BOSS01_HPP_INCLUDED
#define BOSS01_HPP_INCLUDED

#include "Boss.hpp"


namespace LX_Graphics
{
class LX_Sprite;
};

namespace LX_Physics
{
struct LX_Circle;
};

class Boss01 : public Boss
{
    int id_strat;
    Uint32 wallTime;
    Uint32 rowTime;

    LX_Physics::LX_Circle *htop, *hdown;

    void rowShot();
    void wallShot();

public :

    explicit Boss01(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
                    int x, int y, int w, int h,float vx, float vy);


    void fire();
    void strategy(void);
    void shoot(const MISSILE_TYPE& m_type);
    void move(void);
    virtual void collision(Missile *mi);
    void die();

    ~Boss01();
};


/* Position strategy */
class Boss01PositionStrat : virtual public BossStrategy
{
public:

    explicit Boss01PositionStrat(Boss01 * newEnemy);

    void proceed(void);
    void fire(const MISSILE_TYPE& m_type);

    ~Boss01PositionStrat();
};


/* Wall shot strategy */
class Boss01WallStrat : virtual public BossStrategy
{
    Uint32 beginWall;
    int first;

public:

    explicit Boss01WallStrat(Boss01 * newEnemy);

    void proceed(void);
    void fire(const MISSILE_TYPE& m_type);

    ~Boss01WallStrat();
};


/* Row shot strategy */
class Boss01RowStrat : virtual public BossStrategy
{
    Uint32 beginRow;
    int first;

public:

    explicit Boss01RowStrat(Boss01 * newEnemy);

    void proceed(void);
    void fire(const MISSILE_TYPE& m_type);

    ~Boss01RowStrat();
};

#endif // BOSS01_HPP_INCLUDED

