
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
*	@file Bullet.cpp
*	@brief The bullet implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL_Timer.h>

#include "Bullet.hpp"

static const int LIMIT = 1000;


Bullet::Bullet(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,
               int x, int y, int w, int h,int dX, int dY)
    : Missile(pow, 2, image, audio, x, y, w, h, dX, dY)
{
    btime = 0;
}


Bullet::Bullet(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,
               SDL_Rect *rect,LX_Vector2D *sp)
    : Missile(pow, 2, image, audio, rect, sp)
{
    btime = 0;
}


Bullet::~Bullet() {}


void Bullet::move()
{
    if(SDL_GetTicks() - btime > LIMIT)
    {
        if(speed.vx < -4)
            speed.vx +=1;
        else if(speed.vx > 4)
            speed.vx -=1;

        btime = SDL_GetTicks();
    }

    Missile::move();
}


void Bullet::displayAdditionnalData()
{
    // Empty
}


