
/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2014  Luxon Jean-Pierre
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
*	@brief The enemy class
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Enemy.hpp"
#include "../game/Game.hpp"
#include "Player.hpp"
#include "../engine/LX_Physics.hpp"

using namespace LX_Physics;



Enemy::Enemy(unsigned int hp, unsigned int att, unsigned int sh,
             SDL_Texture *image, LX_Chunk *audio,
             Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Character(hp,att,sh,image, audio, x, y, w, h, dX, dY)
{
    box = {x,y,w,h};
    wasKilled = false;

}


Enemy::Enemy(unsigned int hp, unsigned int att, unsigned int sh,
             SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,Speed *sp)
    : Character(hp,att,sh,image, audio, rect, sp)
{
    box = {rect->x,rect->y,rect->w,rect->h};
    wasKilled = false;

}


void Enemy::move(void)
{
    position.x += speed.speed_X;
    position.y += speed.speed_Y;

    moveRect(&box,speed.speed_X,speed.speed_Y);
}

// use the strategy
void Enemy::strategy(void)
{
    strat->proceed();
}


void Enemy::receive_damages(unsigned int attacks)
{
    Character::receive_damages(attacks);
}


void Enemy::collision(Missile *mi)
{
    if(LX_Physics::collisionRect(&box,mi->get_hitbox()))
    {
        reaction(mi);
        mi->die();
    }
}


void Enemy::collision(Player *play)
{
    if(LX_Physics::collisionCircleRect(play->get_hitbox(),&box))
    {
        receive_damages(play->getMAX_HP());
        play->die();
    }
}


// define how the enemy react when it has collision with the following target
void Enemy::reaction(Missile *target)
{
    receive_damages(target->put_damages());
}


// Add a new strategy deleting the old one
void Enemy::addStrategy(Strategy *newStrat)
{
    if(newStrat != NULL)
    {
        delete strat;
        strat = newStrat;
    }
}


// delete the strategy
void Enemy::deleteStrategy()
{
    delete strat;
    strat = NULL;
}



LX_AABB * Enemy::get_hitbox()
{
    return &box;
}

