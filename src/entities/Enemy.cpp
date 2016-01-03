
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
*	@file Enemy.cpp
*	@brief The enemy implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL_surface.h>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Physics.hpp>

#include "Enemy.hpp"
#include "BasicMissile.hpp"
#include "../game/Game.hpp"
#include "../xml/XMLReader.hpp"

#include <iostream>

using namespace LX_Graphics;
using namespace LX_Physics;

static SDL_Surface *enemyMissileSurface[ENEMY_MISSILES];


Enemy::Enemy(unsigned int hp, unsigned int att, unsigned int sh,
             SDL_Texture *image, LX_Mixer::LX_Chunk *audio,
             Sint16 x, Sint16 y, Uint16 w, Uint16 h,float vx, float vy)
    : Character(hp,att,sh,image, audio, x, y, w, h, vx, vy), strat(nullptr)
{
    // Empty
}



Enemy::~Enemy()
{
    delete strat;
}

void Enemy::shoot()
{
    SDL_Rect pos_mis;
    LX_Vector2D sp_mis = LX_Vector2D(-MISSILE_SPEED,0);
    Game *g = Game::getInstance();

    pos_mis.x = position.x - MISSILE_WIDTH;
    pos_mis.y = position.y + ((position.h - MISSILE_HEIGHT)/ 2);
    pos_mis.w = MISSILE_WIDTH;
    pos_mis.h = MISSILE_HEIGHT;

    g->addEnemyMissile(new BasicMissile(attack_val,
                             loadTextureFromSurface(getResources()[1]),
                             nullptr,pos_mis,sp_mis));
}


void Enemy::createMissileRessources()
{
    TX_Asset *asset = TX_Asset::getInstance();

    for(unsigned int i = 0; i < ENEMY_MISSILES; i++)
    {
        enemyMissileSurface[i] = loadSurface(asset->getEnemyMissilesFile(i));
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
        enemyMissileSurface[i] = nullptr;
    }
}

void Enemy::move(void)
{
    moveRect(position,speed);
    moveCircle(hitbox,speed);
}

// use the strategy
void Enemy::strategy(void)
{
    if(strat != nullptr)
        strat->proceed();
}


void Enemy::receiveDamages(unsigned int attacks)
{
    std::cout << "TOOK : " << (attacks - shield) << std::endl;
    Character::receiveDamages(attacks);
    std::cout << "new HEALTH : " << health_point << std::endl;
}


void Enemy::collision(Missile *mi)
{
    if(!mi->isDead() && mi->getX() <= (position.x + position.w))
    {
        if(LX_Physics::collisionCircleRect(hitbox,*mi->getHitbox()))
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
        if(LX_Physics::collisionCircle(*play->getHitbox(),hitbox))
        {
            play->die();
        }
    }
}


// Define how the enemy react when it has collision with the following target
void Enemy::reaction(Missile *target)
{
    Score *sc = Game::getInstance()->getScore();

    receiveDamages(target->hit());
    sc->notify(DAMAGE_SCORE);
}


// Add a new strategy deleting the old one
void Enemy::addStrategy(Strategy *new_strat)
{
    if(new_strat != nullptr)
    {
        delete strat;
        strat = new_strat;
    }
}


// delete the strategy
void Enemy::deleteStrategy()
{
    delete strat;
    strat = nullptr;
}

// Play the sound of explosion
void Enemy::boom()
{
    // Empty
}

const LX_Physics::LX_Circle * Enemy::getHitbox()
{
    return &hitbox;
}

