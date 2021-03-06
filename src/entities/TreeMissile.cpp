
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

#include "TreeMissile.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/engine/EntityHandler.hpp"

#include <Lunatix/Time.hpp>

namespace
{
const unsigned int TREE_DELAY = 500;
}


TreeMissile::TreeMissile( unsigned int pow, lx::Graphics::Sprite * image,
                          lx::Graphics::ImgRect& rect, lx::Physics::Vector2D& sp )
    : Missile( MULTIPLIER, pow, image, rect, sp ), t( lx::Time::getTicks() ) {}


void TreeMissile::move() noexcept
{
    Missile::move();

    if ( ( lx::Time::getTicks() - t ) > TREE_DELAY && !Engine::outOfBound( phybox ) )
    {
        lx::Physics::Vector2D v{speed.vx, -speed.vy};
        EntityHandler& hdl = EntityHandler::getInstance();

        hdl.pushEnemyMissile( *( new BasicMissile( power, graphic, imgbox, v ) ) );
        t = lx::Time::getTicks();
    }
}
