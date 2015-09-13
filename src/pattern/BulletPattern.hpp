#ifndef BULLETPATTERN_HPP_INCLUDED
#define BULLETPATTERN_HPP_INCLUDED


/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2015  Luxon Jean-Pierre
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
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file BulletPattern.hpp
*	@brief The bullet patterns file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

struct LX_Vector2D;


namespace BulletPattern
{

void shotOnTarget(const float pos_x, const float pos_y,const int vel,LX_Vector2D *v);

};


#endif // BULLETPATTERN_HPP_INCLUDED

