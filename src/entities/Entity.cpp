
/*
*	Target_Xplosion - A classic shoot'em up video game
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
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file Entity.cpp
*	@brief The Entity file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Entity.hpp"

#include <LunatiX/LX_Sound.hpp>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Log.hpp>


Entity::Entity(LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
               int x, int y, int w, int h,float vx, float vy)
    : graphic(image),sound(audio),position({x,y,w,h}),
speed(LX_Physics::LX_Vector2D(vx,vy)), still_alive(true) {}

Entity::Entity(LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
               SDL_Rect& rect,LX_Physics::LX_Vector2D& sp)
    : Entity(image,audio,rect.x,rect.y,rect.w,rect.h,sp.vx,sp.vy) {}

Entity::~Entity() {}


void Entity::die()
{
    still_alive = false;
}

void Entity::draw()
{
    if(graphic != nullptr)
        graphic->draw(&position);
}

bool Entity::isDead()
{
    return still_alive == false;
}

// Setters
void Entity::setX(int newX)
{
    position.x = newX;
}


void Entity::setY(int newY)
{
    position.y = newY;
}


void Entity::setXvel(float xvel)
{
    speed.vx = xvel;
}


void Entity::setYvel(float yvel)
{
    speed.vy = yvel;
}


// Getters
int Entity::getXvel() const
{
    return static_cast<int>(speed.vx);
}

int Entity::getYvel() const
{
    return static_cast<int>(speed.vy);
}

int Entity::getX() const
{
    return position.x;
}

int Entity::getY() const
{
    return position.y;
}

int Entity::getWidth() const
{
    return position.w;
}

int Entity::getHeight() const
{
    return position.h;
}
