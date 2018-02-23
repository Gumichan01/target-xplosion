
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

#ifndef BOSS00_HPP_INCLUDED
#define BOSS00_HPP_INCLUDED

#include "Boss.hpp"


namespace LX_Graphics
{
class LX_Sprite;
}

class MoveAndShootStrategy;

class SemiBoss01 : public Boss
{
    unsigned int shot_delay;

    void shootLvl1() noexcept;
    void shootLvl2() noexcept;
    void shootLvl3() noexcept;
    void frontShot() noexcept;
    void rearShot() noexcept;
    void shot(LX_AABB& pos) noexcept;

    bool canShoot() const noexcept;
    void movePosition() noexcept;

public:

    explicit SemiBoss01(unsigned int hp, unsigned int att, unsigned int sh,
                        LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                        float vx, float vy);

    virtual void strategy() noexcept;
    void fire() noexcept;
    void die() noexcept;

    ~SemiBoss01() = default;
};

#endif // BOSS00_HPP_INCLUDED
