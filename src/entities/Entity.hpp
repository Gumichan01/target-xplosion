

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


#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED


/**
*	@file Entity.h
*	@brief The Entity library
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/


#include <cstdio>
#include <string>
#include <cmath>

#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>

#include "../engine/LX_Hitbox.hpp"
#include "../engine/LX_Sound.hpp"
#include "../engine/LX_Chunk.hpp"
#include "../engine/LX_Mixer.hpp"
#include "../engine/LX_Vector2D.hpp"

#ifndef ZERO
#define ZERO 0
#endif


namespace LX_Mixer{

class LX_Chunk;

};

using namespace LX_Mixer;



class Entity
{

protected :

    SDL_Texture *graphic;
    LX_Chunk *sound;
    SDL_Rect position;
    bool still_alive;
    LX_Vector2D speed;


public:

    Entity(SDL_Texture *image, LX_Chunk *audio,
           int x, int y, int w, int h,int dX, int dY);

    Entity(SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp);

    virtual void move() = 0 ;
    virtual void die();
    bool isDead();

    void setX(int newX);
    void setY(int newY);
    void set_Xvel(int Xvel);
    void set_Yvel(int Yvel);

    SDL_Texture * getTexture();
    SDL_Rect * getPos();

    int get_Xvel();
    int get_Yvel();
    int getX();
    int getY();
    int getWidth();
    int getHeight();

    virtual ~Entity();

};


#endif // ENTITY_H_INCLUDED



