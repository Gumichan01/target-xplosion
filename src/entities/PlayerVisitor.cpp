
/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Luxon Jean-Pierre (Gumichan01)
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file PlayerVisitor.cpp
*	@brief The visitor of the player (implementation)
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "PlayerVisitor.hpp"

#include <LunatiX/LX_Hitbox.hpp>

PlayerVisitor::PlayerVisitor() : last_player_x(0), last_player_y(0) {}

void PlayerVisitor::visit(LX_Physics::LX_Point& last)
{
    last_player_x = last.x;
    last_player_y = last.y;
}

