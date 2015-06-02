

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


#ifndef ROCKET_H_INCLUDED
#define ROCKET_H_INCLUDED


/**
*	@file Rocket.h
*	@brief Rhe Rocket class
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Missile.hpp"


class Rocket : public Missile
{

public:

    Rocket(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,int x, int y, int w, int h,int dX, int dY)
        : Missile(pow, 2, image, audio, x, y, w, h, dX, dY)
    {
        //Nothing to do
    }

    Rocket(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp)
        : Missile(pow, 2, image, audio, rect, sp)
    {
        //Nothing to do
    }

    ~Rocket() {}
};


#endif // ROCKET_H_INCLUDED


