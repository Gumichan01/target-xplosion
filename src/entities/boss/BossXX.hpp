
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

#ifndef BOSS02_HPP_INCLUDED
#define BOSS02_HPP_INCLUDED

#include "Boss.hpp"


namespace LX_Graphics
{
class LX_Sprite;
};


class BossXX : public Boss
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

public :

    explicit BossXX(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
                    int x, int y, int w, int h, float vx, float vy);

    void shotOnTarget();
    void stratX();
    virtual void fire();
    virtual void strategy();
    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    virtual void die();

    ~BossXX();
};

class BossXXShot : public BossStrategy
{
    uint32_t shot_t;
    uint32_t wave_t;
    uint32_t pause_t;
    bool shoot;

public:

    explicit BossXXShot(BossXX * nboss);
    virtual void proceed();
    ~BossXXShot() = default;
};

class BossXXShot2 : public BossStrategy
{
    BossXXShot bsstrat;
    ShotStrategy bbstrat;

public:

    explicit BossXXShot2(BossXX * nboss);
    virtual void proceed();
    ~BossXXShot2() = default;
};

class BossXXBreak : public BossStrategy
{
    uint32_t xtime;

public:

    explicit BossXXBreak(BossXX * nboss);
    virtual void proceed();
    ~BossXXBreak() = default;
};

class BossXXReload : public BossStrategy
{
    uint32_t t;

public:

    explicit BossXXReload(BossXX * nboss);
    virtual void proceed();
    ~BossXXReload() = default;
};

#endif // BOSS02_HPP_INCLUDED
