

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
*	@version 0.1
*	@date August 8th,2014
*
*
*
*/

#include "Enemy.hpp"
#include "../game/Game.hpp"

#include "Player.hpp"

void Enemy::move(void)
{
    position.x += speed.speed_X;
    position.y += speed.speed_Y;

    box.x += speed.speed_X;
    box.y += speed.speed_Y;
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
    if(LX_Physics::collision(&box,mi->get_hitbox()))
    {
        reaction(mi);
        mi->die();
    }
}

void Enemy::collision(Player *play)
{
    if(LX_Physics::collision(play->get_hitbox(),&box))
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
















