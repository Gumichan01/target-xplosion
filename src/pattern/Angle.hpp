#ifndef ANGLE_HPP_INCLUDED
#define ANGLE_HPP_INCLUDED

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
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file Angle.hpp
*	@brief The angle database
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <cmath>
#include <string>
#include <map>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
const float SQRT_THREE = static_cast<float>(sqrt(3));
const float SQRT_TWO = static_cast<float>(sqrt(2));

// Offset
const float OFFSET_CIRCLE = 1000;

// Angles

const std::string ZERO = "0";
const std::string PI = "PI";

// PI/12
const std::string PI12 = "PI12";
const float PI12_COS = (SQRT_TWO * (SQRT_THREE + 1))/4.0f;
const float PI12_SIN = (SQRT_TWO * (SQRT_THREE - 1))/4.0f;

// PI/6
const std::string PI6 = "PI6";
const float PI6_COS = SQRT_THREE/2.0f;
const float PI6_SIN = 1.0f/2.0f;

// PI/4
const std::string PI4 = "PI4";
const float PI4_COS = SQRT_TWO/2.0f;
const float PI4_SIN = SQRT_TWO/2.0f;

// PI/3
const std::string PI3 = "PI3";
const float PI3_COS = 1.0f/2.0f;
const float PI3_SIN = SQRT_THREE/2.0f;

// PI/2
const std::string PI2 = "PI2";
const float PI2_COS = 0.0f;
const float PI2_SIN = 1.0f;

typedef struct
{
    float cos;          /* Cosine */
    float sin;          /* Sine   */

} Angle;

// Associated array of angles
static std::map<std::string,Angle> angles = {
    {ZERO,{OFFSET_CIRCLE,0.0f}},
    {PI12,{PI12_COS*OFFSET_CIRCLE,PI12_SIN*OFFSET_CIRCLE}},
    {PI6,{PI6_COS*OFFSET_CIRCLE,PI6_SIN*OFFSET_CIRCLE}},
    {PI4,{PI4_COS*OFFSET_CIRCLE,PI4_SIN*OFFSET_CIRCLE}},
    {PI3,{PI3_COS*OFFSET_CIRCLE,PI3_SIN*OFFSET_CIRCLE}},
    {PI2,{PI2_COS*OFFSET_CIRCLE,PI2_SIN*OFFSET_CIRCLE}},
    {PI,{-OFFSET_CIRCLE,0.0f}}};

// A private structure used in the circle pattern
typedef struct TX_Point
{
    float x;
    float y;

}TX_Point;
#pragma clang diagnostic pop
#pragma clang diagnostic pop

#endif // ANGLE_HPP_INCLUDED

