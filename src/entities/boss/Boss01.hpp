
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017  Luxon Jean-Pierre
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
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
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
struct LX_Polygon;
};

class Boss01 : public Boss
{
    uint32_t wall_time;
    uint32_t row_time;
    LX_Physics::LX_Polygon *hpoly;

    void rowShot();
    void wallShot();
    void bulletCirclesShot();

public :

    explicit Boss01(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
                    int x, int y, int w, int h, float vx, float vy);


    virtual void fire();
    virtual void strategy();
    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    virtual void die();

    ~Boss01();
};


/* Position strategy */
class Boss01PositionStrat : virtual public BossStrategy
{
public:

    explicit Boss01PositionStrat(Boss01 * newEnemy);
    void proceed();
    ~Boss01PositionStrat();
};


/* Wall shot strategy */
class Boss01WallStrat : virtual public BossStrategy
{
    uint32_t begin_wall;
    int first;

public:

    explicit Boss01WallStrat(Boss01 * newEnemy);
    void proceed();
    ~Boss01WallStrat();
};


/* Row shot strategy */
class Boss01RowStrat : virtual public BossStrategy
{
    uint32_t begin_row;
    int first;

public:

    explicit Boss01RowStrat(Boss01 * newEnemy);
    void proceed();
    ~Boss01RowStrat();
};

#endif // BOSS01_HPP_INCLUDED

