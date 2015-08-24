
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

#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_rect.h>

#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_FileBuffer.hpp>

#include "../game/Game.hpp"
#include "Bullet.hpp"

using namespace LX_FileIO;

static const Uint32 LIMIT = 500;
static const Uint32 DELAY_MBTIME = 500;
static const int CIRCLE_BULLETS = 28;

static LX_FileBuffer *bulletBuffer;


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



/* ------------------------------
    MegaMissile implementation
   ------------------------------ */


MegaBullet::MegaBullet(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,
                       int x, int y, int w, int h,int dX, int dY)
    : Missile(pow,2,image,audio,x,y,w,h,dX,dY)
{
    mbtime = SDL_GetTicks();
}


MegaBullet::MegaBullet(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,
                       SDL_Rect *rect,LX_Vector2D *sp)
    : Missile(pow,2,image,audio,rect,sp)
{
    mbtime = SDL_GetTicks();
}


MegaBullet::~MegaBullet() {}    // Empty


void MegaBullet::createBulletBuffer(void)
{
    bulletBuffer = new LX_FileBuffer("image/light_bullet.png");
}

void MegaBullet::destroyBulletBuffer(void)
{
    delete bulletBuffer;
    bulletBuffer = NULL;
}


void MegaBullet::move()
{
    if((SDL_GetTicks() - mbtime) > DELAY_MBTIME)
    {
        explosion();
        die();
    }
    else
        Missile::move();
}


void MegaBullet::displayAdditionnalData() {} // Empty


void MegaBullet::explosion(void)
{
    SDL_Rect rect;
    LX_Vector2D v[CIRCLE_BULLETS];

    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    Game *g = Game::getInstance();

    rect = {position.x,position.y,24,24};

    v[0] = {8,0};
    v[1] = {7,4};
    v[2] = {6,5};
    v[3] = {5,6};
    v[4] = {3,7};
    v[5] = {0,8};
    v[6] = {-3,7};
    v[7] = {-5,6};
    v[8] = {-6,5};
    v[9] = {-7,3};
    v[10] = {-8,0};
    v[11] = {-7,-4};
    v[12] = {-6,-5};
    v[13] = {-5,-6};
    v[14] = {-3,-7};
    v[15] = {0,-8};
    v[16] = {3,-7};
    v[17] = {5,-6};
    v[18] = {6,-5};
    v[19] = {7,-4};
    v[20] = {-8,1};
    v[21] = {-8,-1};
    v[22] = {-8,2};
    v[23] = {8,-2};
    v[24] = {8,1};
    v[25] = {8,-1};
    v[26] = {8,2};
    v[27] = {8,-2};

    surface = LX_Graphics::loadSurfaceFromFileBuffer(bulletBuffer);

    for(int i = 0; i < 28; i++)
    {
        texture = LX_Graphics::loadTextureFromSurface(surface,0);
        g->addEnemyMissile(new Bullet(power,texture,NULL,&rect,&v[i]));
    }

    SDL_FreeSurface(surface);
}

