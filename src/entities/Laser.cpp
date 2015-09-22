
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
*	@file Laser.cpp
*	@brief The laser implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL_timer.h>

#include "Laser.hpp"

static Uint32 laser_shot_delay = 32;
Uint32 Laser::laser_ref_time = 0;


Laser::Laser(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,int x, int y, int w, int h,int dX, int dY)
    : Missile(pow, 5, image, audio, x, y, w, h, dX, dY)
{
    initLaser();
}


Laser::Laser(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp)
    : Missile(pow, 5, image, audio, rect, sp)
{
    initLaser();
}


void Laser::initLaser(void)
{
    state = true;
    //laser_ref_time = SDL_GetTicks();

    sprite_rect[0] = {0,0,LASER_WIDTH,LASER_HEIGHT};
    sprite_rect[1] = {0,81,LASER_WIDTH,LASER_HEIGHT};
    sprite_rect[2] = {129,0,LASER_WIDTH,LASER_HEIGHT};
    sprite_rect[3] = {129,81,LASER_WIDTH,LASER_HEIGHT};
    sprite_rect[4] = {258,0,LASER_WIDTH,LASER_HEIGHT};
    sprite_rect[5] = {258,81,LASER_WIDTH,LASER_HEIGHT};
}


void Laser::move()
{
    if(state == false)
    {
        die();
    }
    else
    {
        state = false;
        Missile::move();
    }
}


void Laser::displayAdditionnalData()
{
    // Empty
}


SDL_Rect * Laser::getAreaToDisplay()
{
    Uint32 diff_time = SDL_GetTicks() - laser_ref_time;

    if(diff_time > (laser_shot_delay*6))
    {
        laser_ref_time = SDL_GetTicks();
        return &sprite_rect[0];
    }
    else if(diff_time > (laser_shot_delay*5))
    {
        return &sprite_rect[5];
    }
    else if(diff_time > (laser_shot_delay*4))
    {
        return &sprite_rect[4];
    }
    else if(diff_time > (laser_shot_delay*3))
    {
        return &sprite_rect[3];
    }
    else if(diff_time > (laser_shot_delay*2))
    {
        return &sprite_rect[2];
    }
    else if(diff_time > (laser_shot_delay))
    {
        return &sprite_rect[1];
    }
    else
        return &sprite_rect[0];
}

Laser::~Laser() {}

