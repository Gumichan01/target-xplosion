
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

#ifndef SEMIBOSS03_HPP_INCLUDED
#define SEMIBOSS03_HPP_INCLUDED

#include "Boss.hpp"
#include "../../pattern/Strategy.hpp"

#include <array>

namespace LX_Graphics
{
class LX_Sprite;
}

namespace BulletPattern
{
class SpinShot;
}

class SemiBoss03Target;
class MultiStrategy;

class SemiBoss03 : public Boss
{
    friend class SemiBoss03Target;
    static const size_t SEMIBOSS03_SPIN_NUM = 16;

    MultiStrategy *mult;
    // Last strategies
    SemiBoss03Target *sbt;
    ShotStrategy *shot;
    std::array<BulletPattern::SpinShot*, SEMIBOSS03_SPIN_NUM> vspin;

    void bpos() noexcept;
    void spinShotStratEasy() noexcept;
    void spinShotStratNormal() noexcept;
    void spinShotStratHard() noexcept;
    void explosionShot() noexcept;

    void waveShot() noexcept;
    void spinShot() noexcept;

public:

    explicit SemiBoss03(unsigned int hp, unsigned int att, unsigned int sh,
                        LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                        float vx, float vy);

    virtual void strategy() noexcept;
    virtual void fire() noexcept;
    virtual void die() noexcept;
    ~SemiBoss03();
};

class SemiBoss03Target : public BossStrategy
{
    SemiBoss03 *b;

public:

    explicit SemiBoss03Target(SemiBoss03 * nboss);
    virtual void proceed() noexcept;
    ~SemiBoss03Target() = default;
};

#endif // SEMIBOSS03_HPP_INCLUDED
