#ifndef ANGLE_HPP_INCLUDED
#define ANGLE_HPP_INCLUDED

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
*	@file Angle.hpp
*	@brief The angle
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <cmath>

const float SQRT_THREE = static_cast<float>(sqrt(3));
const float SQRT_TWO = static_cast<float>(sqrt(2));
const float HALF_OF_SQRT_THREE = SQRT_THREE/2.0f;
const float ONE_HALF = 1.0f/2.0f;
const float HALF_OF_SQRT_TWO = (sqrt(2))/2.0f;
const float PI12_COS = (SQRT_TWO * (SQRT_THREE + 1))/4.0f;
const float PI12_SIN = (SQRT_TWO * (SQRT_THREE - 1))/4.0f;


#endif // ANGLE_HPP_INCLUDED
