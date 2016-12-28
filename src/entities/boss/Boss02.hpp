
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
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


class Boss02 : public Boss
{
    static const int NB_BOSS_SPRITES = 2;

    bool shield;
    uint32_t shield_points;
    LX_Physics::LX_Circle core_hbox;
    LX_Graphics::LX_Sprite *asprite;
    LX_Graphics::LX_Sprite *asprite_sh;
    LX_Graphics::LX_Sprite *asprite_nosh;

    void bullets();
    void mbullets();
    void reload();
    void unleash();

public :

    explicit Boss02(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
                    int x, int y, int w, int h, float vx, float vy);

    void shotOnTarget();
    virtual void fire();
    virtual void strategy();
    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    virtual void die();

    ~Boss02();
};

class Boss02Shot : public BossStrategy
{
    uint32_t shot_t;
    uint32_t wave_t;
    uint32_t pause_t;
    bool shoot;

public:

    explicit Boss02Shot(Boss02 * nboss);
    virtual void proceed();
    ~Boss02Shot() = default;
};


/*class Boss02Bullet : public BossStrategy
{
    uint32_t shot_t;

public:

    explicit Boss02Bullet(Boss02 * nboss);
    virtual void proceed();
    ~Boss02Bullet() = default;
};*/


class Boss02Shot2 : public BossStrategy
{
    Boss02Shot bsstrat;
    ShotStrategy bbstrat;

public:

    explicit Boss02Shot2(Boss02 * nboss);
    virtual void proceed();
    ~Boss02Shot2() = default;
};

class Boss02Reload : public BossStrategy
{
    uint32_t t;

public:

    explicit Boss02Reload(Boss02 * nboss);
    virtual void proceed();
    ~Boss02Reload() = default;
};

#endif // BOSS02_HPP_INCLUDED
