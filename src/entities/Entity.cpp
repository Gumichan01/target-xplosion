
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

#include "Entity.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Hitbox.hpp>
#include <algorithm>


/// Entity

Entity::Entity(LX_Graphics::LX_Sprite *image, const LX_Graphics::LX_ImgRect& rect,
               const LX_Physics::LX_Vector2D& sp)
    : graphic(image), position(rect), phybox(fromRect(rect)), speed(sp),
      still_alive(true) {}


LX_Physics::LX_FloatingBox Entity::fromRect(const LX_Graphics::LX_ImgRect& r) noexcept
{
    return LX_Physics::LX_FloatingBox{LX_Physics::LX_FloatPosition{static_cast<float>(r.p.x),
                                      static_cast<float>(r.p.y)},
                                      r.w, r.h};
}

void Entity::die() noexcept
{
    still_alive = false;
}

void Entity::draw() noexcept
{
    position = LX_Graphics::toImgRect(phybox);

    if(graphic != nullptr)
        graphic->draw(position);
}


bool Entity::isDead() const noexcept
{
    return !still_alive;
}

// Setters
void Entity::setX(float nx) noexcept
{
    phybox.fpoint.x = fbox(nx);
}


void Entity::setY(float ny) noexcept
{
    phybox.fpoint.y = fbox(ny);
}


void Entity::setXvel(float xvel) noexcept
{
    speed.vx = fbox(xvel);
}

void Entity::setYvel(float yvel) noexcept
{
    speed.vy = fbox(yvel);
}

// Getters
Float Entity::getXvel() const noexcept
{
    return speed.vx;
}

Float Entity::getYvel() const noexcept
{
    return speed.vy;
}

Float Entity::getX() const noexcept
{
    return phybox.fpoint.x;
}

Float Entity::getY() const noexcept
{
    return phybox.fpoint.y;
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

PolygonShape::PolygonShape(const std::vector<LX_Physics::LX_FloatPosition>& points,
                           const LX_Physics::LX_FloatPosition& pos) : polygon_hitbox()
{
    std::vector<LX_Physics::LX_FloatPosition> _points(points.begin(), points.end());

    std::for_each(_points.begin(), _points.end(), [pos](LX_Physics::LX_FloatPosition& p)
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
