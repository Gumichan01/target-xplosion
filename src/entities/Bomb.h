#ifndef BOMB_H_INCLUDED
#define BOMB_H_INCLUDED


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
*	@file Bomb.h
*	@brief The bomb class
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date August 28th, 2014
*
*
*
*/

#include "Missile.h"

#define BOMB_LIFETIME 60

class Bomb: public Missile
{

    unsigned int lifetime;
    bool explosion;

    public:

    Bomb(unsigned int pow, SDL_Surface *image, Mix_Chunk *audio,int x, int y, int w, int h,int dX, int dY)
        : Missile(pow, 4, image, audio, x, y, w, h, dX, dY)
    {
        lifetime = BOMB_LIFETIME;
        explosion = false;
    }

    Bomb(unsigned int pow, SDL_Surface *image, Mix_Chunk *audio,SDL_Rect *rect,Speed *sp)
        : Missile(pow, 4, image, audio, rect, sp)
    {
        lifetime = BOMB_LIFETIME;
        explosion = false;
    }

    void move();
    void die();


    ~Bomb(){}

};
















#endif // BOMB_H_INCLUDED













