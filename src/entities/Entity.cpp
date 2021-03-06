
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

#include "Entity.hpp"

#include <Lunatix/Graphics.hpp>
#include <Lunatix/Hitbox.hpp>
#include <algorithm>

using FloatBox::fbox;
using lx::Physics::toFloatingBox;

/// Entity

Entity::Entity( lx::Graphics::Sprite * image, const lx::Graphics::ImgRect& rect,
                const lx::Physics::Vector2D& sp )
    : graphic( image ), imgbox( rect ), phybox( toFloatingBox( rect ) ), speed( sp ),
      still_alive( true ) {}


void Entity::die() noexcept
{
    still_alive = false;
}

void Entity::draw() noexcept
{
    imgbox = lx::Graphics::toImgRect( phybox );

    if ( graphic != nullptr )
        graphic->draw( imgbox );
}


bool Entity::isDead() const noexcept
{
    return !still_alive;
}

// Setters
void Entity::setX( float nx ) noexcept
{
    phybox.p.x = fbox( nx );
}


void Entity::setY( float ny ) noexcept
{
    phybox.p.y = fbox( ny );
}


void Entity::setXvel( float xvel ) noexcept
{
    speed.vx = fbox( xvel );
}

void Entity::setYvel( float yvel ) noexcept
{
    speed.vy = fbox( yvel );
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
    return phybox.p.x;
}

Float Entity::getY() const noexcept
{
    return phybox.p.y;
}

int Entity::getWidth() const noexcept
{
    return imgbox.w;
}

int Entity::getHeight() const noexcept
{
    return imgbox.h;
}

/// PolygonShape

PolygonShape::PolygonShape( const std::vector<lx::Physics::FloatPosition>& points,
                            const lx::Physics::FloatPosition& pos ) : polygon_hitbox()
{
    std::vector<lx::Physics::FloatPosition> _points( points.begin(), points.end() );

    std::for_each( _points.begin(), _points.end(), [pos]( lx::Physics::FloatPosition & p )
    {
        p.x += pos.x;
        p.y += pos.y;
    } );

    polygon_hitbox.addPoints( _points.begin(), _points.end() );
}

lx::Physics::Polygon& PolygonShape::getPoly() noexcept
{
    return polygon_hitbox;
}
