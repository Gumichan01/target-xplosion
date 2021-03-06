
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
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#ifndef BOSS03_HPP_INCLUDED
#define BOSS03_HPP_INCLUDED

#include "Boss.hpp"
#include "../../pattern/BulletPattern.hpp"

namespace lx
{

namespace Graphics
{
class Sprite;
}

}

/** Boss */

class Boss03 final: public Enemy
{
    static const size_t BOSS03_PARTS = 2;

    Boss * boss_parts[BOSS03_PARTS];
    int index;

    Boss03( const Boss03& ) = delete;
    Boss03( const Boss03&& ) = delete;
    Boss03& operator =( const Boss03& ) = delete;
    Boss03& operator =( const Boss03&& ) = delete;

public:

    explicit Boss03( unsigned int hp, unsigned int att, unsigned int sh,
                     lx::Graphics::Sprite * image, int x, int y, int w, int h,
                     float vx, float vy );

    virtual void draw() noexcept override;
    virtual void strategy() noexcept override;
    virtual void collision( Missile * mi ) noexcept override;
    virtual void collision( Player * play ) noexcept override;
    virtual void die() noexcept override;

    virtual Float getX() const noexcept override;
    virtual Float getY() const noexcept override;
    virtual int getWidth() const noexcept override;
    virtual int getHeight() const noexcept override;

    virtual ~Boss03();
};

/** Body of the boss (Part 1) */

class Boss03Head;

class Boss03Body final: public Boss
{
    friend class Boss03RayBullet;
    friend class Boss03RowBullet;
    friend class Boss03WaveBullet;

    static const size_t BOSS03_SPIN_NUM = 24;

    int ray_id;
    PolygonShape shape;
    Boss03Head * observer;
    std::array<BulletPattern::SpinShot *, BOSS03_SPIN_NUM> vspin1, vspin2;

    // strategies
    void strat0()     noexcept;
    void strat1Row()  noexcept;
    void strat2Wave() noexcept;

    // Fire!!
    void rayShot() noexcept;
    void circleShot() noexcept;
    void rowShot() noexcept;
    void dShot() noexcept;
    void finalWave() noexcept;

    Boss03Body( const Boss03Body& ) = delete;
    Boss03Body( const Boss03Body&& ) = delete;
    Boss03Body& operator =( const Boss03Body& ) = delete;
    Boss03Body& operator =( const Boss03Body&& ) = delete;

public:

    explicit Boss03Body( unsigned int hp, unsigned int att, unsigned int sh,
                         lx::Graphics::Sprite * image, int x, int y, int w, int h,
                         float vx, float vy );

    void addObserver( Boss03Head& obs ) noexcept;
    virtual void strategy() noexcept override;
    virtual void move() noexcept override;
    virtual void collision( Missile * mi ) noexcept override;
    virtual void collision( Player * play ) noexcept override;
    virtual void die() noexcept override;

    virtual ~Boss03Body();
};


class Boss03RayBullet: public Strategy
{
    unsigned int ray_time;

    Boss03RayBullet( const Boss03RayBullet& ) = delete;
    Boss03RayBullet( const Boss03RayBullet&& ) = delete;
    Boss03RayBullet& operator =( const Boss03RayBullet& ) = delete;
    Boss03RayBullet& operator =( const Boss03RayBullet&& ) = delete;

protected:

    Boss03Body * body;

public:

    explicit Boss03RayBullet( Boss03Body * b );
    virtual void proceed() noexcept override;
    virtual ~Boss03RayBullet() = default;
};

class Boss03RowBullet final: public Boss03RayBullet
{
    unsigned int row_time;

public:

    explicit Boss03RowBullet( Boss03Body * b );
    virtual void proceed() noexcept override;
    virtual ~Boss03RowBullet() = default;
};

class Boss03WaveBullet final: public Boss03RayBullet
{
    unsigned int wave_time;

public:

    explicit Boss03WaveBullet( Boss03Body * b );
    virtual void proceed() noexcept override;
    virtual ~Boss03WaveBullet() = default;
};


/** Head of the boss (Part 2) */

enum class Boss03_MSG
{
    MOVE, DEATH
};

class Boss03HeadStratBase;

class Boss03Head final: public Boss
{
    friend class Boss03HeadStratBase;
    static const size_t NB_SPINSHOT = 48U;
    PolygonShape shape;

    Boss03HeadStratBase * head_stratb;
    BulletPattern::SpinShot pattern_up1;
    BulletPattern::SpinShot pattern_up2;
    BulletPattern::RevSpinShot pattern_down1;
    BulletPattern::RevSpinShot pattern_down2;
    std::array<BulletPattern::SpinShot *, NB_SPINSHOT> vspin;

    void propelShot() noexcept;
    void prisonShot() noexcept;
    void toPlayerShot01() noexcept;
    void circleShot() noexcept;
    void toPlayerShot02() noexcept;
    void spinShot() noexcept;

    void moveStrat() noexcept;
    void runToLeftStrat() noexcept;
    void runToRightStrat() noexcept;
    void prisonStrat() noexcept;
    void circle01Strat() noexcept;
    void spinStrat() noexcept;

    Boss03Head( const Boss03Head& ) = delete;
    Boss03Head( const Boss03Head&& ) = delete;
    Boss03Head& operator =( const Boss03Head& ) = delete;
    Boss03Head& operator =( const Boss03Head&& ) = delete;

protected:

    virtual void createHitSprite() override;

public:

    explicit Boss03Head( unsigned int hp, unsigned int att, unsigned int sh,
                         lx::Graphics::Sprite * image, int x, int y, int w, int h,
                         float vx, float vy );

    void notify( const Boss03_MSG& msg ) noexcept;

    virtual void fire() noexcept override;
    virtual void strategy() noexcept override;
    virtual void move() noexcept override;
    virtual void collision( Missile * mi ) noexcept override;
    virtual void collision( Player * play ) noexcept override;
    virtual void die() noexcept override;

    virtual ~Boss03Head();
};

class Boss03HeadStratBase final: public Strategy
{

    Boss03HeadStratBase( const Boss03HeadStratBase& ) = delete;
    Boss03HeadStratBase( const Boss03HeadStratBase&& ) = delete;
    Boss03HeadStratBase& operator =( const Boss03HeadStratBase& ) = delete;
    Boss03HeadStratBase& operator =( const Boss03HeadStratBase&& ) = delete;

protected:
    Boss03Head * head;

public:

    explicit Boss03HeadStratBase( Boss03Head * b );
    virtual void proceed() noexcept override;
    virtual ~Boss03HeadStratBase() = default;
};

#endif // BOSS03_HPP_INCLUDED
