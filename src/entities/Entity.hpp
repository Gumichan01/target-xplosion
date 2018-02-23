
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

#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Polygon.hpp>

#include <vector>


namespace LX_Graphics
{
class LX_Sprite;
}

namespace LX_Physics
{
struct LX_Circle;
}

class Enemy;

/// @todo remove it (integration of LunatiX v0.13.0)
struct FloatPosition
{
    float x;
    float y;

    FloatPosition();
    FloatPosition(float fx, float fy);
    FloatPosition(const FloatPosition& fp);
    explicit FloatPosition(const LX_AABB& b);
    FloatPosition& operator =(const FloatPosition& fp);
    FloatPosition& operator =(const LX_AABB& aabb);
    FloatPosition& operator =(const LX_Physics::LX_Circle& circle);
    FloatPosition& operator +=(const LX_Physics::LX_Vector2D& v);

    void toPixelUnit(LX_AABB& aabb);
    void toPixelUnit(LX_Physics::LX_Circle& circle);
};


class Entity
{

protected:

    LX_Graphics::LX_Sprite *graphic = nullptr;
    LX_AABB position;
    FloatPosition fpos;
    LX_Physics::LX_Vector2D speed;
    bool still_alive = true;

public:

    Entity() = default;

    Entity(LX_Graphics::LX_Sprite *image, const LX_AABB& rect,
           const LX_Physics::LX_Vector2D& sp);

    virtual void move() noexcept = 0;
    virtual void die() noexcept;
    virtual void draw() noexcept;
    bool isDead() const noexcept;

    virtual void setX(int newX) noexcept;
    virtual void setY(int newY) noexcept;
    void setXvel(float xvel) noexcept;
    void setYvel(float yvel) noexcept;

    float getXvel() const noexcept;
    float getYvel() const noexcept;
    virtual int getX() const noexcept;
    virtual int getY() const noexcept;
    virtual int getWidth() const noexcept;
    virtual int getHeight() const noexcept;

    virtual ~Entity() = default;
};


class PolygonShape
{
    LX_Physics::LX_Polygon polygon_hitbox;

    PolygonShape(const PolygonShape&) = delete;
    PolygonShape& operator =(const PolygonShape&) = delete;

public:

    PolygonShape(const std::vector<LX_Physics::LX_Point>& points,
                 const LX_Physics::LX_Point& pos);

    LX_Physics::LX_Polygon& getPoly() noexcept;

    ~PolygonShape() = default;
};

#endif // ENTITY_H_INCLUDED
