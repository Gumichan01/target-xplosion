
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

#ifndef TOWER_HPP_INCLUDED
#define TOWER_HPP_INCLUDED

#include "Enemy.hpp"
#include "../pattern/Strategy.hpp"


namespace LX_Graphics
{
class LX_Sprite;
}

/// @todo enemies with polygon hitbox → PolygonalEnemy

class Tower1 : public LargeEnemy
{
    LX_AABB main_hitbox;
    PolygonShape shape;

public:

    explicit Tower1(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                    float vx, float vy);

    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *p);
    virtual void boom();
    virtual void draw();
    virtual void fire();
    virtual void die();
    ~Tower1() = default;

};

// Strategy of Tower1
class Tower1Strat: public Strategy
{

public:

    explicit Tower1Strat(Enemy *newEnemy);
    void proceed();
    ~Tower1Strat() = default;
};

#endif // TOWER_HPP_INCLUDED

