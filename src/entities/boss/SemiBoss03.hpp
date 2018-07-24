
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

namespace lx
{

namespace Graphics
{
class Sprite;
}

}

namespace BulletPattern
{
class SpinShot;
}

class SemiBoss03Target;
class MultiStrategy;

class SemiBoss03 final: public Boss
{
    friend class SemiBoss03Target;
    static const size_t SEMIBOSS03_SPIN_NUM = 16;

    MultiStrategy * mult;
    // Last strategies
    SemiBoss03Target * sbt;
    ShotStrategy * shot;
    std::array<BulletPattern::SpinShot *, SEMIBOSS03_SPIN_NUM> vspin;

    SemiBoss03( const SemiBoss03& ) = delete;
    SemiBoss03( const SemiBoss03&& ) = delete;
    SemiBoss03& operator =( const SemiBoss03& ) = delete;
    SemiBoss03& operator =( const SemiBoss03&& ) = delete;

    void bpos() noexcept;
    void spinShotStratEasy() noexcept;
    void spinShotStratNormal() noexcept;
    void spinShotStratHard() noexcept;
    void explosionShot() noexcept;

    void waveShot() noexcept;
    void spinShot() noexcept;

public:

    explicit SemiBoss03( unsigned int hp, unsigned int att, unsigned int sh,
                         lx::Graphics::Sprite * image, int x, int y, int w, int h,
                         float vx, float vy );

    virtual void strategy() noexcept override;
    virtual void fire() noexcept override;
    virtual void die() noexcept override;
    ~SemiBoss03();
};

class SemiBoss03Target final: public BossStrategy
{
    SemiBoss03 * b;

    SemiBoss03Target( const SemiBoss03Target& ) = delete;
    SemiBoss03Target( const SemiBoss03Target&& ) = delete;
    SemiBoss03Target& operator =( const SemiBoss03Target& ) = delete;
    SemiBoss03Target& operator =( const SemiBoss03Target&& ) = delete;

public:

    explicit SemiBoss03Target( SemiBoss03 * nboss );
    virtual void proceed() noexcept override;
    ~SemiBoss03Target() = default;
};

#endif // SEMIBOSS03_HPP_INCLUDED
