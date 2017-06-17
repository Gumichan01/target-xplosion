
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

#ifndef BOSS03_HPP_INCLUDED
#define BOSS03_HPP_INCLUDED

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
    uint32_t shield_points;
    LX_Physics::LX_Circle core_hbox;
    LX_Graphics::LX_Sprite *asprite;
    LX_Graphics::LX_Sprite *asprite_sh;
    LX_Graphics::LX_Sprite *asprite_x;
    LX_Graphics::LX_Sprite *asprite_nosh;

    void bullets();
    void mbullets();
    void reload();
    void unleash();

    void stratPos();
    void stratReload();
    void stratUnleash();

public:

    explicit Boss04(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                    float vx, float vy);

    void shotOnTarget();
    void stratX();
    virtual void fire();
    virtual void strategy();
    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    virtual void reaction(Missile *target);
    virtual void die();

    ~Boss04();
};

class Boss04Shot : public BossStrategy
{
    uint32_t shot_t;
    uint32_t wave_t;
    uint32_t pause_t;
    bool shoot;

public:

    explicit Boss04Shot(Boss04 * nboss);
    virtual void proceed();
    ~Boss04Shot() = default;
};

class Boss04Shot2 : public BossStrategy
{
    Boss04Shot bsstrat;
    ShotStrategy bbstrat;

public:

    explicit Boss04Shot2(Boss04 * nboss);
    virtual void proceed();
    ~Boss04Shot2() = default;
};

class Boss04Break : public BossStrategy
{
    uint32_t xtime;

public:

    explicit Boss04Break(Boss04 * nboss);
    virtual void proceed();
    ~Boss04Break() = default;
};

class Boss04Reload : public BossStrategy
{
    uint32_t t;

public:

    explicit Boss04Reload(Boss04 * nboss);
    virtual void proceed();
    ~Boss04Reload() = default;
};

#endif // BOSS03_HPP_INCLUDED
