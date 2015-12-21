
/*
*   Target_Xplosion - The classic shoot'em up video game
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
*	@file Tower.hpp
*	@brief The Towwer(s) implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Hitbox.hpp>

#include "Tower.hpp"
#include "Bullet.hpp"
#include "../game/Game.hpp"


static const Uint32 DELAY_TOWER = 500;


Tower1::Tower1(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Mixer::LX_Chunk *audio,
               Sint16 x, Sint16 y, Uint16 w, Uint16 h,float vx, float vy)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,vx,vy)
{
    strat = new Tower1Strat(this);
}


void Tower1::shoot()
{
    static const int BULLET_VEL = -7;
    static const int N = 9;

    Game *g = Game::getInstance();
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture = nullptr;
    SDL_Rect rect = {position.x,position.y+130,24,24};

    LX_Physics::LX_Vector2D velocity[] = {{BULLET_VEL,0},{BULLET_VEL,-1},{BULLET_VEL,1},
        {BULLET_VEL,-2},{BULLET_VEL,2},{BULLET_VEL,-3},{BULLET_VEL,3},
        {BULLET_VEL,-4},{BULLET_VEL,4}
    };

    surface = LX_Graphics::loadSurfaceFromFileBuffer(Bullet::getLightBulletBuffer());

    for(int i = 0; i < N; i++)
    {
        texture = LX_Graphics::loadTextureFromSurface(surface);
        g->addEnemyMissile(new Bullet(attack_val,texture,nullptr,
                                      rect,velocity[i]));
        texture = nullptr;
    }
    SDL_FreeSurface(surface);
}


Tower1::~Tower1() {}


Tower1Strat::Tower1Strat(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    reference_time = 0;
}


Tower1Strat::~Tower1Strat() {}


void Tower1Strat::proceed(void)
{
    if((SDL_GetTicks() - reference_time) > DELAY_TOWER)
    {
        fire(ROCKET_TYPE);
        reference_time = SDL_GetTicks();
    }
    target->move();
}


void Tower1Strat::fire(MISSILE_TYPE m_type)
{
    if(m_type == ROCKET_TYPE)
    {
        target->shoot();
    }
}

