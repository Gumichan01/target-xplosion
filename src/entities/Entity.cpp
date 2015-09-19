
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
*	@file Entity.cpp
*	@brief The Entity file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <cstdio>
#include <string>
#include <cmath>

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include <LunatiX/LX_Chunk.hpp>

#include "Entity.hpp"



Entity::Entity(SDL_Texture *image, LX_Chunk *audio,int x, int y, int w, int h,int dX, int dY)
{
    graphic = image;    // assign the image
    sound = audio;      // assign the sound

    // assigne all features
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;

    still_alive = true;

    speed.vx = dX;
    speed.vy = dY;
}


Entity::Entity(SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp)
{
    graphic = image;    // assign the image
    sound = audio;      // assign the sound

    // assigne all features
    position.x = rect->x;
    position.y = rect->y;
    position.w = rect->w;
    position.h = rect->h;

    still_alive = true;

    speed.vx = sp->vx;
    speed.vy = sp->vy;
}



Entity::~Entity()
{
    delete sound;
    SDL_DestroyTexture(graphic);
}


void Entity::setX(int newX)
{
    position.x = newX;
}



void Entity::setY(int newY)
{
    position.y = newY;
}



void Entity::setXvel(int xvel)
{
    speed.vx = xvel;
}



void Entity::setYvel(int yvel)
{
    speed.vy = yvel;
}


SDL_Texture * Entity::getTexture()
{
    return graphic;
}


SDL_Rect * Entity::getPos()
{
    return &position;
}


void Entity::die()
{
    still_alive = false;
}


bool Entity::isDead()
{
    return still_alive == false ;
}


int Entity::getXvel()
{
    return speed.vx;
}


int Entity::getYvel()
{
    return speed.vy;
}


int Entity::getX()
{
    return position.x;
}


int Entity::getY()
{
    return position.y;
}


int Entity::getWidth()
{
    return position.w;
}


int Entity::getHeight()
{
    return position.h;
}

SDL_Rect * Entity::getAreaToDisplay()
{
    return NULL;
}

