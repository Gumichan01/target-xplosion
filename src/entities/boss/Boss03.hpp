
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

namespace LX_Physics
{
//struct LX_Circle;
//class LX_Polygon;
}


class Boss03: public Boss
{
    //LX_Graphics::LX_Sprite *body_sprite;
    //LX_Graphics::LX_Sprite *head_sprite;
    static const int BOSS03_PARTS = 1;
    Boss *boss_parts[BOSS03_PARTS];
    int index;

public:

    explicit Boss03(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                    float vx, float vy);

    virtual void draw();
    virtual void strategy();
    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    virtual void die();

    virtual ~Boss03();
};


class Boss03RayBullet;

class Boss03Body : public Boss
{
    friend class Boss03RayBullet;
    int ray_id;
    // strategies
    void strat0();
    void strat1Row();

    // Fire!!
    void rayShot();
    void circleShot();

public:

    explicit Boss03Body(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                    float vx, float vy);

    virtual void fire();
    virtual void strategy();
    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    virtual void die();

    virtual ~Boss03Body() = default;
};


class Boss03RayBullet: public Strategy
{
    Boss03Body *body;
    uint32_t ray_time;

public:

    Boss03RayBullet(Boss03Body *b);
    virtual void proceed();

    virtual ~Boss03RayBullet() = default;
};

#endif // BOSS03_HPP_INCLUDED
