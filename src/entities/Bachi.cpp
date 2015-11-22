
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
*	@file Bachi.cpp
*	@brief File that implements a bee (bachi in Japanese)
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL_render.h>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Hitbox.hpp>

#include "Bachi.hpp"
#include "Bullet.hpp"
#include "../game/Game.hpp"


static const int BACHI_BULLET_OFFSET_X = 8;
static const int BACHI_BULLET_OFFSET_Y = 16;
static const int BACHI_BULLET_SIZE = 16;

static const int BACHI_BULLET_VELOCITY = -10;
static const int BACHI_SHOT_DELAY = 400;


Bachi::Bachi(unsigned int hp, unsigned int att, unsigned int sh,
             SDL_Texture *image, LX_Mixer::LX_Chunk *audio,
             Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,dX,dY)
{
    initBachi();
}


Bachi::~Bachi()
{
    // Empty
}


void Bachi::initBachi()
{
    MoveAndShootStrategy *mvs = nullptr;
    ShotStrategy *st = nullptr;

    mvs = new MoveAndShootStrategy(this);
    st = new ShotStrategy(this);
    strat = mvs;

    st->setShotDelay(BACHI_SHOT_DELAY);
    mvs->addMoveStrat(new PseudoSinusMoveStrategy(this));
    mvs->addShotStrat(st);
}


Missile * Bachi::shoot(MISSILE_TYPE m_type)
{
    const int n = 3;
    LX_Physics::LX_Vector2D bullet_speed[] = {{BACHI_BULLET_VELOCITY,0},
        {BACHI_BULLET_VELOCITY,-1}, {BACHI_BULLET_VELOCITY,1}
    };

    SDL_Rect shot_area = {position.x + BACHI_BULLET_OFFSET_X,
                          position.y + BACHI_BULLET_OFFSET_Y,
                          BACHI_BULLET_SIZE, BACHI_BULLET_SIZE
                         };

    Game *g = Game::getInstance();
    SDL_Surface *bullet_surface = nullptr;

    bullet_surface = LX_Graphics::loadSurfaceFromFileBuffer(Bullet::getRedBulletBuffer());

    for(int i = 0; i < n; i++)
    {
        g->addEnemyMissile(new Bullet(attack_val,
                                      LX_Graphics::loadTextureFromSurface(bullet_surface),
                                      nullptr,shot_area,bullet_speed[i]));
    }

    SDL_FreeSurface(bullet_surface);

    // We do not need to return anything in this pattern
    return nullptr;
}


void Bachi::reaction(Missile *target)
{
    Enemy::reaction(target);

    if(was_killed)
    {
        Game::getInstance()->addItem(new Item(position.x,position.y));
    }
}









