
/*
*	Target_Xplosion - A classic shoot'em up video game
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

#ifndef BULLETPATTERN_HPP_INCLUDED
#define BULLETPATTERN_HPP_INCLUDED

/// @todo Try to improve the shot-on-target pattern (trajectory issue)

namespace LX_Physics
{
struct LX_Vector2D;
};

// The number of bullets in the circle
const int CIRCLE_BULLETS = 25;

// The velocity of the bullets in the circle
const int CIRCLE_BULLETS_DEFAULT_VEL = -8;

namespace BulletPattern
{

void shotOnPlayer(const float shooter_x,const float shooter_y,
                  const int vel,LX_Physics::LX_Vector2D& v);

void shotOnTarget(const float shooter_x,const float shooter_y,
                  const float target_x,const float target_y,
                  const int vel,LX_Physics::LX_Vector2D& v);
void circlePattern(const float pos_x,const float pos_y,
                   const int vel,LX_Physics::LX_Vector2D v[]);

};

#endif // BULLETPATTERN_HPP_INCLUDED
