
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017 Luxon Jean-Pierre
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


class Boss04 : public Boss
{
    static const int NB_BOSS_SPRITES = 2;
    const unsigned int HEALTH_80;
    const unsigned int HEALTH_55;
    const unsigned int HEALTH_25;

    bool shield;
    unsigned int shield_points;
    LX_Physics::LX_Circle core_hbox;
    FloatPosition core_fpos;
    LX_Graphics::LX_Sprite *asprite;
    LX_Graphics::LX_Sprite *asprite_sh;
    LX_Graphics::LX_Sprite *asprite_x;
    LX_Graphics::LX_Sprite *asprite_nosh;

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
    virtual void fire() noexcept;
    virtual void strategy() noexcept;
    virtual void move() noexcept;
    virtual void collision(Missile *mi) noexcept;
    virtual void collision(Player *play) noexcept;
    virtual void reaction(Missile *target) noexcept;
    virtual void die() noexcept;

    ~Boss04() = default;
};

class Boss04Shot : public BossStrategy
{
    Boss04 *boss04;
    unsigned int shot_t;
    unsigned int wave_t;
    unsigned int pause_t;
    bool shoot;

public:

    explicit Boss04Shot(Boss04 * nboss);
    virtual void proceed() noexcept;
    ~Boss04Shot() = default;
};

class Boss04Shot2 : public BossStrategy
{
    Boss04Shot bsstrat;
    ShotStrategy bbstrat;

public:

    explicit Boss04Shot2(Boss04 * nboss);
    virtual void proceed() noexcept;
    ~Boss04Shot2() = default;
};

class Boss04Break : public BossStrategy
{
    Boss04 *boss04;
    unsigned int xtime;

public:

    explicit Boss04Break(Boss04 * nboss);
    virtual void proceed() noexcept;
    ~Boss04Break() = default;
};

class Boss04Reload : public BossStrategy
{
    unsigned int t;

public:

    explicit Boss04Reload(Boss04 * nboss);
    virtual void proceed() noexcept;
    ~Boss04Reload() = default;
};

#endif // BOSS04_HPP_INCLUDED
