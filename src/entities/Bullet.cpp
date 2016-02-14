
/*
*	Target_Xplosion - The classic shoot'em up video game
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

#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_FileBuffer.hpp>
#include <LunatiX/LX_Hitbox.hpp>

#include "Bullet.hpp"

#include "../game/Game.hpp"
#include "../xml/XMLReader.hpp"
#include "../pattern/BulletPattern.hpp"

using namespace LX_FileIO;

static const Uint32 LIMIT = 1000;
static const Uint32 DELAY_MBTIME = 500;

static LX_FileBuffer *bulletBuffer;
static LX_FileBuffer *redBulletBuffer;


Bullet::Bullet(unsigned int pow, SDL_Texture *image,
               LX_Mixer::LX_Chunk *audio,
               SDL_Rect& rect, LX_Physics::LX_Vector2D& sp)
    : Missile(pow, 2, image, audio, rect, sp),bullet_time(SDL_GetTicks())
{
    // Empty
}


Bullet::~Bullet() {}


void Bullet::move()
{
    if(SDL_GetTicks() - bullet_time > LIMIT)
    {
        bullet_time = SDL_GetTicks();
    }

    Missile::move();
}


void Bullet::displayAdditionnalData()
{
    // Empty
}

// Staic functions that handle the bullet buffer
void Bullet::createBulletBuffer(void)
{
    TX_Asset *tx = TX_Asset::getInstance();
    const unsigned int BULLET_INDEX = 4;
    const unsigned int ENEMY_INDEX = 4;
    const std::string PL_MISSILES_FILE = tx->getPlayerMissilesFile(BULLET_INDEX);
    const std::string EN_MISSILES_FILES = tx->getEnemyMissilesFile(ENEMY_INDEX);

    bulletBuffer = new LX_FileBuffer(PL_MISSILES_FILE.c_str());
    redBulletBuffer = new LX_FileBuffer(EN_MISSILES_FILES.c_str());
}


void Bullet::destroyBulletBuffer(void)
{
    delete bulletBuffer;
    delete redBulletBuffer;
    bulletBuffer = nullptr;
    redBulletBuffer = nullptr;
}


LX_FileBuffer * Bullet::getLightBulletBuffer(void)
{
    return bulletBuffer;
}


LX_FileBuffer * Bullet::getRedBulletBuffer(void)
{
    return redBulletBuffer;
}


/* ------------------------------
    MegaMBullet implementation
   ------------------------------ */


MegaBullet::MegaBullet(unsigned int pow, SDL_Texture *image, LX_Mixer::LX_Chunk *audio,
                       SDL_Rect& rect, LX_Physics::LX_Vector2D& sp,int explosion_vel)
    : Missile(pow,2,image,audio,rect,sp), mbtime(SDL_GetTicks()),
    circle_vel(explosion_vel)
{
    // Empty
}


MegaBullet::~MegaBullet() {}    // Empty


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
    LX_Physics::LX_Vector2D v[CIRCLE_BULLETS];

    Game *g = Game::getInstance();
    rect = {position.x,position.y,24,24};

    BulletPattern::circlePattern(position.x + (position.w/2),
                                 position.y + (position.h/2),
                                 circle_vel,v);

    SDL_Texture *texture = nullptr;
    SDL_Surface *surface = LX_Graphics::loadSurfaceFromFileBuffer(bulletBuffer);

    for(int i = 0; i < CIRCLE_BULLETS; i++)
    {
        texture = LX_Graphics::loadTextureFromSurface(surface);
        g->addEnemyMissile(new Bullet(power,texture,nullptr,rect,v[i]));
    }

    SDL_FreeSurface(surface);
}

