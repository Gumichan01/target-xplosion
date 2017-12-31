
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

#ifndef BOSS03_HPP_INCLUDED
#define BOSS03_HPP_INCLUDED

#include "Boss.hpp"
#include "../../pattern/BulletPattern.hpp"

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
    static const size_t BOSS03_PARTS = 2;

    Boss *boss_parts[BOSS03_PARTS];
    int index;

public:

    explicit Boss03(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                    float vx, float vy);

    virtual void draw();
    virtual void strategy();
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

//class Boss03RayBullet;
class Boss03Head;

class Boss03Body : public Boss
{
    friend class Boss03RayBullet;
    friend class Boss03RowBullet;
    friend class Boss03WaveBullet;

    static const size_t BOSS03_SPIN_NUM = 24;

    int ray_id;
    LX_Physics::LX_Polygon *poly;
    Boss03Head *observer;
    std::array<BulletPattern::SpinShot*, BOSS03_SPIN_NUM> vspin1, vspin2;

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

    void addObserver(Boss03Head& obs);
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

    explicit Boss03RayBullet(Boss03Body *b);
    virtual void proceed();
    virtual ~Boss03RayBullet() = default;
};

class Boss03RowBullet: public Boss03RayBullet
{
    uint32_t row_time;

public:

    explicit Boss03RowBullet(Boss03Body *b);
    virtual void proceed();
    virtual ~Boss03RowBullet() = default;
};

class Boss03WaveBullet: public Boss03RayBullet
{
    uint32_t wave_time;

public:

    explicit Boss03WaveBullet(Boss03Body *b);
    virtual void proceed();
    virtual ~Boss03WaveBullet() = default;
};


/** Head of the boss (Part 2) */

enum class Boss03_MSG
{
    MOVE, DEATH
};

class Boss03HeadStratBase;

class Boss03Head : public Boss
{
    friend class Boss03HeadStratBase;
    LX_Physics::LX_Polygon * poly;

    MoveAndShootStrategy * mvs;
    Boss03HeadStratBase  * head_stratb;
    BulletPattern::SpinShot pattern_up1;
    BulletPattern::SpinShot pattern_up2;
    BulletPattern::RevSpinShot pattern_down1;
    BulletPattern::RevSpinShot pattern_down2;
    std::array<BulletPattern::SpinShot*, 48> vspin;

    void propelShot();
    void prisonShot();
    void toPlayerShot01();
    void circleShot();
    void toPlayerShot02();
    void spinShot();

    void moveStrat();
    void runToLeftStrat();
    void runToRightStrat();
    void prisonStrat();
    void circle01Strat();
    void spinStrat();

protected:

    virtual void createHitSprite();

public:

    explicit Boss03Head(unsigned int hp, unsigned int att, unsigned int sh,
                        LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                        float vx, float vy);

    void notify(const Boss03_MSG& msg);

    virtual void fire();
    virtual void strategy();
    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    virtual void die();

    virtual ~Boss03Head();
};

class Boss03HeadStratBase: public Strategy
{

protected:
    Boss03Head *head;

public:

    explicit Boss03HeadStratBase(Boss03Head *b);
    virtual void proceed();
    virtual ~Boss03HeadStratBase() = default;
};

#endif // BOSS03_HPP_INCLUDED
