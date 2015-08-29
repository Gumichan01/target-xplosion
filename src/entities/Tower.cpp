
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

#include "Tower.hpp"
#include "Bullet.hpp"
#include "../game/Game.hpp"

/// @todo Improve the bullet generation

static const int DELAY_TOWER = 500;


Tower1::Tower1(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Chunk *audio,
               Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,dX,dY)
{
    strat = new Tower1_Strat(this);
}


Tower1::Tower1(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Chunk *audio, SDL_Rect *rect,LX_Vector2D *sp)
    : Enemy(hp,att,sh,image,audio,rect,sp)
{
    strat = new Tower1_Strat(this);
}


Missile * Tower1::shoot(MISSILE_TYPE m_type)
{
    return NULL;
}


Tower1::~Tower1() {}


Tower1_Strat::Tower1_Strat(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    reference_time = 0;
}


Tower1_Strat::~Tower1_Strat() {}


void Tower1_Strat::proceed(void)
{
    if((SDL_GetTicks() - reference_time) > DELAY_TOWER)
    {
        fire(ROCKET_TYPE);
        reference_time = SDL_GetTicks();
    }
    target->move();
}


void Tower1_Strat::fire(MISSILE_TYPE m_type)
{
    static const int v = -11;
    static const int n = 9;

    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    SDL_Rect rect = {target->getX()+26,target->getY()+130,24,24};
    LX_Vector2D velocity[] = {{v,0},{v,-1},{v,1},{v,-2},
        {v,2},{v,-3},{v,3},{v,-4},{v,4}
    };

    Game *g = Game::getInstance();

    surface = LX_Graphics::loadSurface("image/light_bullet.png");

    for(int i = 0; i < n; i++)
    {
        texture = LX_Graphics::loadTextureFromSurface(surface,0);
        g->addEnemyMissile(new Bullet(target->getATT(),texture,
                                      NULL,&rect,&velocity[i]));
        texture = NULL;
    }

    SDL_FreeSurface(surface);
}




