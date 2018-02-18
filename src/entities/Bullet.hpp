
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

#ifndef BULLET_HPP_INCLUDED
#define BULLET_HPP_INCLUDED

#include "Missile.hpp"

namespace LX_Graphics
{
class LX_Sprite;
}


class Bullet : public Missile
{
protected:
    long bullet_time;

public:

    Bullet(unsigned int pow, LX_Graphics::LX_Sprite *image,
           LX_AABB& rect, LX_Physics::LX_Vector2D& sp);

    virtual void draw() noexcept;
    virtual ~Bullet() = default;
};

/* **************
    Trail Bullet
   ************** */

class TrailBullet : public Bullet
{

public:

    TrailBullet(unsigned int pow, LX_Graphics::LX_Sprite *image,
                LX_AABB& rect, LX_Physics::LX_Vector2D& sp);

    virtual void move() noexcept;
    virtual ~TrailBullet() = default;
};

/* ****************
    Lunatic Bullet
   **************** */

class LunaticBullet : public Bullet
{
    static const unsigned int SPIN_BULLET_DELAY = 75;

    unsigned int colour_time;
    const unsigned int CTIME_LIMIT;
    bool is_lunatic;

    void lunatic() noexcept;

public:

    LunaticBullet(unsigned int pow, LX_Graphics::LX_Sprite *image,
                  LX_AABB& rect, LX_Physics::LX_Vector2D& sp);

    virtual void move() noexcept;
    virtual ~LunaticBullet() = default;
};

/* *************
    Mega bullet
   ************* */

class MegaBullet : public Bullet
{

protected:

    float circle_vel;
    virtual void explosion() noexcept;

public:

    MegaBullet(unsigned int pow, LX_Graphics::LX_Sprite *image, LX_AABB& rect,
               LX_Physics::LX_Vector2D& sp, float explosion_vel);

    virtual void move() noexcept;
    ~MegaBullet() = default;
};

/* *************
    Giga bullet
   ************* */

class GigaBullet : public MegaBullet
{
    int vel;

protected:

    virtual void explosion() noexcept;

public:

    GigaBullet(unsigned int pow, LX_Graphics::LX_Sprite *image, LX_AABB& rect,
               LX_Physics::LX_Vector2D& sp, int explosion_mbv1, float explosion_mbv2);

    ~GigaBullet() = default;
};

#endif // BULLET_HPP_INCLUDED
