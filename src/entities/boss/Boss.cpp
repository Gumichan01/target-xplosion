
/*
*   Target_Xplosion - The classic shoot'em up video game
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
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file Boss.cpp
*	@brief The implementation of the Boss Interface
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Boss.hpp"
#include "../../game/Game.hpp"

using namespace LX_Physics;

Boss::Boss(unsigned int hp, unsigned int att, unsigned int sh,
           SDL_Texture *image, LX_Mixer::LX_Chunk *audio,
           int x, int y, int w, int h,float vx, float vy)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,vx,vy), dying(false),
      sprite_ref_time(0)
{
    // Empty
}


Boss::~Boss()
{
    // Empty
}


void Boss::reaction(Missile *target)
{
    if(!dying)
        Enemy::reaction(target);
}

// It is time to die
void Boss::die()
{
    Game *g = Game::getInstance();

    if((position.x + position.w) < 0)
        Entity::die();
    else
    {
        // The boss is dying
        if(!dying)
        {
            // The boss will die
            dying = true;
            g->screenCancel();
            g->stopBossMusic();
            speed = LX_Vector2D(XVEL_DIE,YVEL_DIE);
            sprite_ref_time = SDL_GetTicks();
            boom();
        }
        else
        {
            // It is dead
            // Give points to the player
            Entity::die();
            g->getScore()->notify(static_cast<int>(max_health_point)*2);
        }
    }
}

// Play the sound of explosion
void Boss::boom()
{
    if(sound != nullptr)
        sound->play();
}

// Boss strategy
BossStrategy::BossStrategy(Boss *newBoss)
    : Strategy(newBoss),boss(newBoss)
{
    // Empty
}

BossStrategy::~BossStrategy()
{
    // Empty
}

