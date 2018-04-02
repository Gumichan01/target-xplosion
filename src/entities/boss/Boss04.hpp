
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

#ifndef BOSS04_HPP_INCLUDED
#define BOSS04_HPP_INCLUDED

#include "Boss.hpp"


namespace LX_Graphics
{
class LX_Sprite;
}


class Boss04 final: public Boss
{
    static const int NB_BOSS_SPRITES = 2;
    const unsigned int HEALTH_80;
    const unsigned int HEALTH_55;
    const unsigned int HEALTH_25;

    bool shield;
    Float alpha;
    unsigned int shield_points;
    LX_Physics::LX_Circle core_hbox;
    LX_Graphics::LX_Sprite *asprite;
    LX_Graphics::LX_Sprite *asprite_sh;
    LX_Graphics::LX_Sprite *asprite_x;
    LX_Graphics::LX_Sprite *asprite_nosh;

    Boss04(const Boss04&) = delete;
    Boss04& operator=(const Boss04&) = delete;

    void bullets() noexcept;
    void mbullets() noexcept;
    void reload() noexcept;
    void unleash() noexcept;

    void stratPos() noexcept;
    void stratReload() noexcept;
    void stratUnleash() noexcept;

public:

    explicit Boss04(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                    float vx, float vy);

    void shotOnTarget() noexcept;
    void stratX() noexcept;
    virtual void fire() noexcept override;
    virtual void strategy() noexcept override;
    virtual void move() noexcept override;
    virtual void collision(Missile *mi) noexcept override;
    virtual void collision(Player *play) noexcept override;
    virtual void reaction(Missile *target) noexcept override;
    virtual void die() noexcept override;

    ~Boss04() = default;
};

class Boss04Shot final: public BossStrategy
{
    Boss04 *boss04;
    unsigned int shot_t;
    unsigned int wave_t;
    unsigned int pause_t;
    bool shoot;

    Boss04Shot(const Boss04Shot&) = delete;
    Boss04Shot& operator=(const Boss04Shot&) = delete;

public:

    explicit Boss04Shot(Boss04 * nboss);
    virtual void proceed() noexcept override;
    ~Boss04Shot() = default;
};

class Boss04Shot2 final: public BossStrategy
{
    Boss04Shot bsstrat;
    ShotStrategy bbstrat;

    Boss04Shot2(const Boss04Shot2&) = delete;
    Boss04Shot2& operator=(const Boss04Shot2&) = delete;

public:

    explicit Boss04Shot2(Boss04 * nboss);
    virtual void proceed() noexcept override;
    ~Boss04Shot2() = default;
};

class Boss04Break final: public BossStrategy
{
    Boss04 *boss04;
    unsigned int xtime;

    Boss04Break(const Boss04Break&) = delete;
    Boss04Break& operator=(const Boss04Break&) = delete;

public:

    explicit Boss04Break(Boss04 * nboss);
    virtual void proceed() noexcept override;
    ~Boss04Break() = default;
};

class Boss04Reload final: public BossStrategy
{
    unsigned int t;

public:

    explicit Boss04Reload(Boss04 * nboss);
    virtual void proceed() noexcept override;
    ~Boss04Reload() = default;
};

#endif // BOSS04_HPP_INCLUDED
