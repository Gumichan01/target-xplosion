
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

#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <Lunatix/Vector2D.hpp>
#include <Lunatix/Polygon.hpp>
#include <Lunatix/ImgRect.hpp>

#include <vector>


namespace lx
{

namespace Graphics
{
class Sprite;
}

namespace Physics
{
struct Circle;
}

}

class Enemy;


class Entity
{
    Entity( const Entity& ) = delete;
    Entity( const Entity&& ) = delete;
    Entity& operator =( const Entity& ) = delete;
    Entity& operator =( const Entity&& ) = delete;

protected:

    lx::Graphics::Sprite * graphic = nullptr;
    lx::Graphics::ImgRect imgbox = { { 0, 0} , 0, 0 };
    lx::Physics::FloatingBox phybox = { { FloatBox::FNIL, FloatBox::FNIL }, 0, 0 };
    lx::Physics::Vector2D speed = { FloatBox::FNIL, FloatBox::FNIL };
    bool still_alive = true;

public:

    Entity() = default;

    Entity( lx::Graphics::Sprite * image, const lx::Graphics::ImgRect& rect,
            const lx::Physics::Vector2D& sp );

    virtual void move() noexcept = 0;
    virtual void die() noexcept;
    virtual void draw() noexcept;
    bool isDead() const noexcept;

    virtual void setX( float nx ) noexcept;
    virtual void setY( float ny ) noexcept;
    void setXvel( float xvel ) noexcept;
    void setYvel( float yvel ) noexcept;

    Float getXvel() const noexcept;
    Float getYvel() const noexcept;
    virtual Float getX() const noexcept;
    virtual Float getY() const noexcept;
    virtual int getWidth() const noexcept;
    virtual int getHeight() const noexcept;

    virtual ~Entity() = default;
};


class PolygonShape final
{
    lx::Physics::Polygon polygon_hitbox;

    PolygonShape( const PolygonShape& ) = delete;
    PolygonShape& operator =( const PolygonShape& ) = delete;

public:

    PolygonShape( const std::vector<lx::Physics::FloatPosition>& points,
                  const lx::Physics::FloatPosition& pos );

    lx::Physics::Polygon& getPoly() noexcept;

    ~PolygonShape() = default;
};

#endif // ENTITY_H_INCLUDED
