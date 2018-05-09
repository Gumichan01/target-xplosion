
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

#ifndef TOWER_HPP_INCLUDED
#define TOWER_HPP_INCLUDED

#include "Enemy.hpp"
#include "../pattern/Strategy.hpp"


namespace LX_Graphics
{
class LX_Sprite;
}


class Tower1 final: public Enemy
{
    PolygonShape shape;

    Tower1(const Tower1&) = delete;
    Tower1& operator=(const Tower1&) = delete;

public:

    explicit Tower1(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                    float vx, float vy);

    virtual void move() noexcept override;
    virtual void collision(Missile *mi) noexcept override;
    virtual void collision(Player *p) noexcept override;
    virtual void boom() noexcept override;
    virtual void draw() noexcept override;
    virtual void fire() noexcept override;
    virtual void die() noexcept override;
    ~Tower1() = default;

};

// Strategy of Tower1
class Tower1Strat final: public Strategy
{

    Tower1Strat(const Tower1Strat&) = delete;
    Tower1Strat& operator=(const Tower1Strat&) = delete;

public:

    explicit Tower1Strat(Enemy *newEnemy);
    void proceed() noexcept override;
    ~Tower1Strat() = default;
};

#endif // TOWER_HPP_INCLUDED
