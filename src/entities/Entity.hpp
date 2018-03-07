
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
#include <LunatiX/LX_ImgRect.hpp>

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


class Entity
{

protected:

    LX_Graphics::LX_Sprite *graphic = nullptr;
    LX_Graphics::LX_ImgRect imgbox;   /// @todo imgbox, rename it -> box
    LX_Physics::LX_FloatingBox phybox;
    LX_Physics::LX_Vector2D speed;
    bool still_alive = true;

    LX_Physics::LX_FloatingBox fromRect(const LX_Graphics::LX_ImgRect& r) noexcept;

public:

    Entity() = default;

    Entity(LX_Graphics::LX_Sprite *image, const LX_Graphics::LX_ImgRect& rect,
           const LX_Physics::LX_Vector2D& sp);

    virtual void move() noexcept = 0;
    virtual void die() noexcept;
    virtual void draw() noexcept;
    bool isDead() const noexcept;

    virtual void setX(float nx) noexcept;
    virtual void setY(float ny) noexcept;
    void setXvel(float xvel) noexcept;
    void setYvel(float yvel) noexcept;

    Float getXvel() const noexcept;
    Float getYvel() const noexcept;
    virtual Float getX() const noexcept;
    virtual Float getY() const noexcept;
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

    PolygonShape(const std::vector<LX_Physics::LX_FloatPosition>& points,
                 const LX_Physics::LX_FloatPosition& pos);

    LX_Physics::LX_Polygon& getPoly() noexcept;

    ~PolygonShape() = default;
};

#endif // ENTITY_H_INCLUDED
