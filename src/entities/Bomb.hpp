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
*
*/

#include "Missile.hpp"

#define BOMB_LIFETIME 60
#define BOMB_XPLOSION_W 94
#define BOMB_XPLOSION_H 94

struct LX_Vector2D;

class Bomb: public Missile
{
    unsigned int lifetime;
    bool explosion;

    void initBomb(void);

public:

    Bomb(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,
         int x, int y, int w, int h,int dX, int dY);

    Bomb(unsigned int pow, SDL_Texture *image,
         LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp);

    void move();
    void die();


    ~Bomb() {}

};


#endif // BOMB_H_INCLUDED

