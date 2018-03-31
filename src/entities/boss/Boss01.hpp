
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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
}

namespace LX_Physics
{
struct LX_Circle;
}

class UpDownMoveStrategy;

class Boss01 final: public Boss
{
    bool bshield;
    unsigned int scircle_time;
    unsigned int circle01_time;
    PolygonShape shape;
    int id_pos;

    void bposition() noexcept;
    void circle01() noexcept;
    void circle02() noexcept;
    void shootToKill() noexcept;
    void sideCircleShot() noexcept;
    void bulletCircleShot() noexcept;

public:

    explicit Boss01(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                    float vx, float vy);

    virtual void fire() noexcept override;
    virtual void strategy() noexcept override;
    virtual void move() noexcept override;
    virtual void collision(Missile *mi) noexcept override;
    virtual void collision(Player *play) noexcept override;
    virtual void die() noexcept override;

    ~Boss01() = default;
};


/* Position strategy */
class Boss01PositionStrat final: virtual public BossStrategy
{
public:

    explicit Boss01PositionStrat(Boss01 * newEnemy);
    void proceed() noexcept override;
    ~Boss01PositionStrat();
};


/* Circle #1 shot strategy */
class Boss01Circle01Strat final: virtual public BossStrategy
{
    unsigned int begin_circle01;
    bool first;

public:

    explicit Boss01Circle01Strat(Boss01 * newEnemy);
    void proceed() noexcept override;
    ~Boss01Circle01Strat();
};


/* Circle #2 shot strategy */
class Boss01Circle02Strat final: virtual public BossStrategy
{
    bool first;
    unsigned int begin_scircle;
    UpDownMoveStrategy *mv;

    Boss01Circle02Strat(const Boss01Circle02Strat&) = delete;
    Boss01Circle02Strat(const Boss01Circle02Strat&&) = delete;
    Boss01Circle02Strat& operator =(const Boss01Circle02Strat&) = delete;
    Boss01Circle02Strat& operator =(const Boss01Circle02Strat&&) = delete;

public:

    explicit Boss01Circle02Strat(Boss01 * newEnemy);
    void proceed() noexcept override;
    ~Boss01Circle02Strat();
};

#endif // BOSS01_HPP_INCLUDED
