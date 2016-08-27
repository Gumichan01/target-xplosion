

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


#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED


/**
*	@file Entity.hpp
*	@brief The Entity library
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <LunatiX/LX_Vector2D.hpp>
#include <SDL2/SDL_rect.h>

/// @todo [HIGH] An entity must display the image

namespace LX_Mixer
{
class LX_Sound;
};

namespace LX_Graphics
{
class LX_Image;
};


class Entity
{

protected :

    LX_Graphics::LX_Image *graphic;
    LX_Mixer::LX_Sound *sound;
    SDL_Rect position;
    LX_Physics::LX_Vector2D speed;
    bool still_alive;

public:

    Entity(LX_Graphics::LX_Image *image, LX_Mixer::LX_Sound *audio,
           int x, int y, int w, int h,float vx, float vy);

    Entity(LX_Graphics::LX_Image *image, LX_Mixer::LX_Sound *audio,
           SDL_Rect& rect,LX_Physics::LX_Vector2D& sp);

    virtual void move() = 0;
    virtual void die();
    bool isDead();

    void setX(int newX);
    void setY(int newY);
    void setXvel(float xvel);
    void setYvel(float yvel);

    /// @todo [HIGH] REMOVE getTexture()
    //LX_Graphics::LX_Image * getTexture();
    SDL_Rect * getPos();
    virtual SDL_Rect * getAreaToDisplay();

    int getXvel();
    int getYvel();
    int getX();
    int getY();
    int getWidth();
    int getHeight();

    virtual ~Entity();
};

#endif // ENTITY_H_INCLUDED

