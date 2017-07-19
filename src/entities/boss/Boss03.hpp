
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
class LX_Polygon;
}

/** Boss */

class Boss03: public Enemy
{
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

    virtual int getX() const;
    virtual int getY() const;
    virtual int getWidth() const;
    virtual int getHeight() const;

    virtual ~Boss03();
};

/** Body of the boss (Part1) */

class Boss03RayBullet;

class Boss03Body : public Boss
{
    friend class Boss03RayBullet;
    friend class Boss03RowBullet;
    friend class Boss03WaveBullet;
    int ray_id;
    LX_Physics::LX_Polygon *poly;
    // strategies
    void strat0();
    void strat1Row();
    void strat2Wave();

    // Fire!!
    void rayShot();
    void circleShot();
    void rowShot();
    void dShot();
    void finalWave();

public:

    explicit Boss03Body(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                    float vx, float vy);

    virtual void strategy();
    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    virtual void die();

    virtual ~Boss03Body();
};


class Boss03RayBullet: public Strategy
{
    uint32_t ray_time;

protected:
    Boss03Body *body;

public:

    Boss03RayBullet(Boss03Body *b);
    virtual void proceed();
    virtual ~Boss03RayBullet() = default;
};

class Boss03RowBullet: public Boss03RayBullet
{
    uint32_t row_time;

public:

    Boss03RowBullet(Boss03Body *b);
    virtual void proceed();
    virtual ~Boss03RowBullet() = default;
};

class Boss03WaveBullet: public Boss03RayBullet
{
    uint32_t wave_time;

public:

    Boss03WaveBullet(Boss03Body *b);
    virtual void proceed();
    virtual ~Boss03WaveBullet() = default;
};


/** Head of the boss (Part 2) */

class Boss03Head : public Boss
{
    //LX_Physics::LX_Polygon *poly;

public:

    explicit Boss03Head(unsigned int hp, unsigned int att, unsigned int sh,
    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
    float vx, float vy);

    virtual void strategy();
    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    virtual void die();

    virtual ~Boss03Head();
};

#endif // BOSS03_HPP_INCLUDED
