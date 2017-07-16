
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

#include "Entity.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Hitbox.hpp>


/// Floating-point coordinates

FloatPosition::FloatPosition(): x(0.0f), y(0.0f) {}

FloatPosition::FloatPosition(float fx, float fy): x(fx), y(fy) {}

FloatPosition::FloatPosition(const FloatPosition& fp): FloatPosition(fp.x, fp.y) {}

FloatPosition::FloatPosition(const LX_AABB& b): FloatPosition(b.x, b.y) {}

FloatPosition& FloatPosition::operator =(const FloatPosition& fp)
{
    x = fp.x;
    y = fp.y;
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

Entity::Entity()
    : graphic(nullptr), position(), fpos(), speed(), still_alive(true) {}

Entity::Entity(LX_Graphics::LX_Sprite *image, const LX_AABB& rect,
               const LX_Physics::LX_Vector2D& sp)
    : graphic(image), position(rect), fpos(rect), speed(sp), still_alive(true) {}


void Entity::die()
{
    still_alive = false;
}

void Entity::draw()
{
    if(graphic != nullptr)
        graphic->draw(&position);
}


void Entity::visit(Enemy *) {}


bool Entity::isDead() const
{
    return still_alive == false;
}

// Setters
void Entity::setX(int newX)
{
    position.x = newX;
}


void Entity::setY(int newY)
{
    position.y = newY;
}


void Entity::setXvel(float xvel)
{
    speed.vx = xvel;
}


void Entity::setYvel(float yvel)
{
    speed.vy = yvel;
}

// Getters
int Entity::getXvel() const
{
    return static_cast<int>(speed.vx);
}

int Entity::getYvel() const
{
    return static_cast<int>(speed.vy);
}

int Entity::getX() const
{
    return position.x;
}

int Entity::getY() const
{
    return position.y;
}

int Entity::getWidth() const
{
    return position.w;
}

int Entity::getHeight() const
{
    return position.h;
}
