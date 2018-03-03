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

#include "PlayerVisitor.hpp"

#include <LunatiX/LX_Hitbox.hpp>


void PlayerVisitor::visit(LX_Physics::LX_FloatPosition& last) noexcept
{
    last_player_x = last.x;
    last_player_y = last.y;
}

Float PlayerVisitor::getLastX() const noexcept
{
    return last_player_x;
}

Float PlayerVisitor::getLastY() const noexcept
{
    return last_player_y;
}
