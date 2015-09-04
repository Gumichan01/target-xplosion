
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
*	@file Bomb.cpp
*	@brief The bomb class definition
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_render.h>

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_FileBuffer.hpp>

#include "Bomb.hpp"
#include "../game/Game.hpp"

using namespace LX_FileIO;

static const double ANIMATION_DELAY = 125;
static LX_FileBuffer *explosion_buffer;

Bomb::Bomb(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,int x, int y, int w, int h,int dX, int dY)
    : Missile(pow, 4, image, audio, x, y, w, h, dX, dY)
{
    initBomb();
}



Bomb::Bomb(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp)
    : Missile(pow, 4, image, audio, rect, sp)
{
    initBomb();
}


Bomb::~Bomb()
{
    // Empty
}


void Bomb::initBomb(void)
{
    lifetime = BOMB_LIFETIME;
    ref_time = SDL_GetTicks();
    explosion = false;

    if(sound != NULL)
        sound->volume(MIX_MAX_VOLUME/2);

    sprite[0] = {0,0,BOMB_XPLOSION_W,BOMB_XPLOSION_H};
    sprite[1] = {95,0,BOMB_XPLOSION_W,BOMB_XPLOSION_H};
    sprite[2] = {190,0,BOMB_XPLOSION_W,BOMB_XPLOSION_H};
    sprite[3] = {285,0,BOMB_XPLOSION_W,BOMB_XPLOSION_H};
    sprite[4] = {380,0,BOMB_XPLOSION_W,BOMB_XPLOSION_H};
    sprite[5] = {475,0,BOMB_XPLOSION_W,BOMB_XPLOSION_H};
    sprite[6] = {570,0,BOMB_XPLOSION_W,BOMB_XPLOSION_H};
}

void Bomb::createExplosionBuffer(void)
{
    explosion_buffer = new LX_FileBuffer("image/explosion_sp.png");
}

void Bomb::destroyExplosionBuffer(void)
{
    delete explosion_buffer;
    explosion_buffer = NULL;
}



void Bomb::move()
{
    //if the bomb has not more life time and have not been exploded
    if((SDL_GetTicks() - ref_time) > lifetime)
    {
        die();
    }
    else if(!explosion)
        Missile::move();
}


void Bomb::die()
{
    if(position.x <= (-(position.w)) || position.x > Game::game_Xlimit)
        Missile::die();

    // If no explosion occured
    if(!explosion)
    {
        SDL_Surface * tmp = LX_Graphics::loadSurfaceFromFileBuffer(explosion_buffer);

        SDL_DestroyTexture(graphic);
        graphic = LX_Graphics::loadTextureFromSurface(tmp,0);
        SDL_FreeSurface(tmp);

        position.x -= BOMB_WIDTH /2;
        position.y -= BOMB_HEIGHT /2;
        position.w = BOMB_WIDTH *2 + 1;
        position.h = BOMB_HEIGHT *2 + 1;
        explosion = true;

        ref_time = SDL_GetTicks();

        if(sound != NULL)
            sound->play();
    }
    else if((SDL_GetTicks() - ref_time) > lifetime)
        Missile::die();
}



SDL_Rect * Bomb::getAreaToDisplay()
{
    double time = SDL_GetTicks();

    if(explosion)
    {
        if((time-ref_time) > (ANIMATION_DELAY*6))
        {
            return &sprite[6];
        }
        else if((time-ref_time) > (ANIMATION_DELAY*5))
        {
            return &sprite[5];
        }
        else if((time-ref_time) > (ANIMATION_DELAY*4))
        {
            return &sprite[4];
        }
        else if((time-ref_time) > (ANIMATION_DELAY*3))
        {
            return &sprite[3];
        }
        else if((time-ref_time) > (ANIMATION_DELAY*2))
        {
            return &sprite[2];
        }
        else if((time-ref_time) > (ANIMATION_DELAY))
            return &sprite[1];
        else
            return &sprite[0];
    }
    else
        return NULL;
}

