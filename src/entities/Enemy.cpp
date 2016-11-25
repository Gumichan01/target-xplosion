
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "Enemy.hpp"

#include "BasicMissile.hpp"
#include "../game/Game.hpp"
#include "../game/Scoring.hpp"
#include "../entities/Player.hpp"
#include "../pattern/Strategy.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Sound.hpp>
#include <LunatiX/LX_Physics.hpp>

using namespace LX_Physics;


Enemy::Enemy(unsigned int hp, unsigned int att, unsigned int sh,
             LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
             int x, int y, int w, int h,float vx, float vy)
    : Character(hp,att,sh,image,audio, {x,y,w,h}, LX_Vector2D(vx,vy)),strat(nullptr) {}

Enemy::~Enemy()
{
    delete strat;
}

void Enemy::fire()
{
    LX_AABB pos_mis;
    LX_Vector2D sp_mis = LX_Vector2D(-MISSILE_SPEED,0);
    Game *g = Game::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();

    pos_mis.x = position.x - MISSILE_WIDTH;
    pos_mis.y = position.y + ((position.h - MISSILE_HEIGHT)/ 2);
    pos_mis.w = MISSILE_WIDTH;
    pos_mis.h = MISSILE_HEIGHT;

    g->acceptEnemyMissile(new BasicMissile(attack_val,
                                           rc->getResource(RC_MISSILE,PLAYER_MISSILES+1),
                                           nullptr,pos_mis,sp_mis));
}


void Enemy::move()
{
    moveRect(position,speed);
    moveCircle(hitbox,speed);
}

// use the strategy
void Enemy::strategy()
{
    if(strat != nullptr)
        strat->proceed();
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
            play->die();
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
    delete strat;
    strat = new_strat;
}


// delete the strategy
void Enemy::deleteStrategy()
{
    delete strat;
    strat = nullptr;
}

void Enemy::boom() {}

const LX_Physics::LX_Circle * Enemy::getHitbox()
{
    return &hitbox;
}

