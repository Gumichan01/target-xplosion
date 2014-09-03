

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2014  Luxon Jean-Pierre
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
*	@file Missile.cpp
*	@brief The Missile functions
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date August 1st, 2014
*
*
*/

#include "Missile.h"


unsigned int Missile::put_damages()
{
    return(power*multiplier);
}


void Missile::move()
{

    position.x += speed.speed_X;
    position.y += speed.speed_Y;

    missile_box.x += speed.speed_X;
    missile_box.y += speed.speed_Y;
}































