
/*
*   Target_Xplosion - The classic shoot'em up video game
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
*	@file EnemyData.cpp
*	@brief The definition of constructors
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "EnemyData.hpp"

EnemyData::EnemyData() {}

EnemyData::EnemyData(const EnemyData& data)
    : type(data.type), hp(data.hp), att(data.att), sh(data.sh), time(data.time),
      y(data.y), w(data.w),h(data.h)
{
    // Empty
}

EnemyData& EnemyData::operator =(const EnemyData data)
{
    type = data.type;
    hp = data.hp;
    att = data.att;
    sh = data.sh;
    time = data.time;
    y = data.y;
    w = data.w;
    h = data.h;

    return *this;
}

