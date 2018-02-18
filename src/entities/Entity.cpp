
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

#include "Entity.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Hitbox.hpp>
#include <algorithm>

/// Floating-point coordinates

FloatPosition::FloatPosition(): x(0.0f), y(0.0f) {}
FloatPosition::FloatPosition(float fx, float fy): x(fx), y(fy) {}
FloatPosition::FloatPosition(const FloatPosition& fp): x(fp.x), y(fp.y) {}
FloatPosition::FloatPosition(const LX_AABB& b): FloatPosition(b.x, b.y) {}

FloatPosition& FloatPosition::operator =(const FloatPosition& fp)
{
    x = fp.x;
    y = fp.y;
    return *this;
}

FloatPosition& FloatPosition::operator =(const LX_AABB& aabb)
{
    x = aabb.x;
    y = aabb.y;
    return *this;
}

FloatPosition& FloatPosition::operator =(const LX_Physics::LX_Circle& circle)
{
    x = circle.center.x;
    y = circle.center.y;
    return *this;
}


FloatPosition& FloatPosition::operator +=(const LX_Physics::LX_Vector2D& v)
{
    x += v.vx;
    y += v.vy;
    return *this;
}

void FloatPosition::toPixelUnit(LX_AABB& aabb)
{
    aabb.x = static_cast<int>(x);
    aabb.y = static_cast<int>(y);
}

void FloatPosition::toPixelUnit(LX_Physics::LX_Circle& circle)
{
    circle.center.x = static_cast<int>(x);
    circle.center.y = static_cast<int>(y);
}


/// Entity

Entity::Entity(LX_Graphics::LX_Sprite *image, const LX_AABB& rect,
               const LX_Physics::LX_Vector2D& sp)
    : graphic(image), position(rect), fpos(rect), speed(sp), still_alive(true) {}


void Entity::die() noexcept
{
    still_alive = false;
}

void Entity::draw() noexcept
{
    if(graphic != nullptr)
        graphic->draw(&position);
}


bool Entity::isDead() const noexcept
{
    return !still_alive;
}

// Setters
void Entity::setX(int newX) noexcept
{
    position.x = newX;
    fpos.y = newX;
}


void Entity::setY(int newY) noexcept
{
    position.y = newY;
    fpos.y = newY;
}


void Entity::setXvel(float xvel) noexcept
{
    speed.vx = xvel;
}

void Entity::setYvel(float yvel) noexcept
{
    speed.vy = yvel;
}

// Getters
float Entity::getXvel() const noexcept
{
    return speed.vx;
}

float Entity::getYvel() const noexcept
{
    return speed.vy;
}

int Entity::getX() const noexcept
{
    return position.x;
}

int Entity::getY() const noexcept
{
    return position.y;
}

int Entity::getWidth() const noexcept
{
    return position.w;
}

int Entity::getHeight() const noexcept
{
    return position.h;
}

/// PolygonShape

PolygonShape::PolygonShape(const std::vector<LX_Physics::LX_Point>& points,
                           const LX_Physics::LX_Point& pos) : polygon_hitbox()
{
    std::vector<LX_Physics::LX_Point> _points(points.begin(), points.end());

    std::for_each(_points.begin(), _points.end(), [pos](LX_Physics::LX_Point& p)
    {
        p.x += pos.x;
        p.y += pos.y;
    });

    polygon_hitbox.addPoints(_points.begin(), _points.end());
}

LX_Physics::LX_Polygon& PolygonShape::getPoly() noexcept
{
    return polygon_hitbox;
}
