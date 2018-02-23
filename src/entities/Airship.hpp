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

#ifndef AIRSHIP_H
#define AIRSHIP_H

#include "Enemy.hpp"
#include "../pattern/BulletPattern.hpp"

namespace LX_Graphics
{
class LX_Sprite;
}


class Airship : public LargeEnemy
{
    int idstrat;
    LX_AABB main_hitbox;
    PolygonShape shape;
    BulletPattern::SpinShot pattern1, pattern2;

    // Strategy
    void prepare() noexcept;
    void aposition() noexcept;

    // Fire
    void bomb() noexcept;
    void frontShot() noexcept;
    void doubleSpinShot() noexcept;

public:
    Airship(unsigned int hp, unsigned int att, unsigned int sh,
            LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
            float vx, float vy);

    virtual void boom() noexcept;
    virtual void move() noexcept;
    virtual void draw() noexcept;
    virtual void collision(Missile *mi) noexcept;
    virtual void collision(Player *play) noexcept;
    virtual void strategy() noexcept;
    virtual void fire() noexcept;
    virtual void die() noexcept;

    ~Airship() = default;
};

#endif // AIRSHIP_H
