
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
*	@file Missile.cpp
*	@brief The Missile functions
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Physics.hpp>

#include "Missile.hpp"
#include "../game/Rank.hpp"


using namespace LX_Physics;


Missile::Missile(unsigned int pow,unsigned int mul, SDL_Texture *image, LX_Chunk *audio,
                 Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Entity(image,audio,x, y, w, h,dX, dY)
{
    initMissile(pow,mul);
    missile_box = {x,y,w,h};
}


Missile::Missile(unsigned int pow,unsigned int mul, SDL_Texture *image,
                 LX_Chunk *audio, SDL_Rect& rect,LX_Vector2D& sp)
    : Entity(image, audio, rect, sp)
{
    initMissile(pow,mul);
    missile_box = {rect.x,rect.y,rect.w,rect.h};
}


void Missile::initMissile(int pow, int mul)
{
    short r = Rank::getRank();

    power = pow;
    multiplier = mul;

    if(r != NO_RANK)
        multiplier += r;
}


unsigned int Missile::hit()
{
    return(power*multiplier);
}


void Missile::move()
{
    moveRect(&position,&speed);
    moveRect(&missile_box,&speed);
}


SDL_Rect * Missile::getHitbox()
{
    return &missile_box;
}




