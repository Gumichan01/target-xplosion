
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
*	@file Enemy.cpp
*	@brief The enemy implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL_surface.h>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Physics.hpp>

#include "Enemy.hpp"
#include "../game/Game.hpp"
#include "../xml/XMLReader.hpp"

using namespace LX_Physics;

static SDL_Surface *enemyMissileSurface[ENEMY_MISSILES];


Enemy::Enemy(unsigned int hp, unsigned int att, unsigned int sh,
             SDL_Texture *image, LX_Chunk *audio,
             Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Character(hp,att,sh,image, audio, x, y, w, h, dX, dY)
{
    was_killed = false;
}


Enemy::Enemy(unsigned int hp, unsigned int att, unsigned int sh,
             SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp)
    : Character(hp,att,sh,image, audio, rect, sp)
{
    was_killed = false;
}


Enemy::~Enemy()
{
    delete strat;
}


void Enemy::createMissileRessources()
{
    const std::string * MISSILESFILES = TX_Asset::getInstance()->getEnemyMissilesFiles();

    memset(enemyMissileSurface,0,ENEMY_MISSILES);

    for(int i = 0; i< ENEMY_MISSILES; i++)
    {
        enemyMissileSurface[i] = LX_Graphics::loadSurface(MISSILESFILES[i]);
    }
}


SDL_Surface ** Enemy::getResources()
{
    return enemyMissileSurface;
}


void Enemy::destroyMissileRessources()
{
    for(int i = 0; i< ENEMY_MISSILES; i++)
    {
        SDL_FreeSurface(enemyMissileSurface[i]);
    }
}

void Enemy::move(void)
{
    moveRect(&position,&speed);
    moveCircle(&hitbox,&speed);
}

// use the strategy
void Enemy::strategy(void)
{
    if(strat != NULL)
        strat->proceed();
}


void Enemy::receiveDamages(unsigned int attacks)
{
    Character::receiveDamages(attacks);
}


void Enemy::collision(Missile *mi)
{
    if(mi->getX() <= (position.x + position.w))
    {
        if(LX_Physics::collisionCircleRect(&hitbox,mi->getHitbox()))
        {
            reaction(mi);
            mi->die();
        }
    }
}


void Enemy::collision(Player *play)
{
    if(play->getX() <= (position.x + position.w))
    {
        if(LX_Physics::collisionCircle(play->getHitbox(),&hitbox))
        {
            receiveDamages(play->getMaxHP());
            play->die();
        }
    }
}


// define how the enemy react when it has collision with the following target
void Enemy::reaction(Missile *target)
{
    Score *sc = Game::getInstance()->getScore();

    receiveDamages(target->hit());
    sc->notify(DAMAGE_SCORE);
}


// Add a new strategy deleting the old one
void Enemy::addStrategy(Strategy *new_strat)
{
    if(new_strat != NULL)
    {
        delete strat;
        strat = new_strat;
    }
}


// delete the strategy
void Enemy::deleteStrategy()
{
    delete strat;
    strat = NULL;
}



LX_Circle * Enemy::getHitbox()
{
    return &hitbox;
}

