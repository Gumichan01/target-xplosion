
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017  Luxon Jean-Pierre
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
#include "../game/engine/Engine.hpp"
#include "../game/Scoring.hpp"
#include "../entities/Player.hpp"
#include "../pattern/Strategy.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Chunk.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Log.hpp>

using namespace LX_Physics;

namespace
{
const int ENEMY_BMISSILE_ID = 0;
uint32_t ENEMY_EXPLOSION_DELAY = 1000;
}


Enemy::Enemy(unsigned int hp, unsigned int att, unsigned int sh,
             LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Chunk *audio,
             int x, int y, int w, int h, float vx, float vy)
    : Character(hp, att, sh, image, audio, {x, y, w, h}, LX_Vector2D(vx, vy)),
strat(nullptr)
{
    // An enemy that has no graphical repreesntation cannot exist
    if(graphic == nullptr)
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,"enemy - No graphical resource");
}


Enemy::~Enemy()
{
    delete strat;
}

void Enemy::fire()
{
    LX_AABB pos_mis;
    LX_Vector2D sp_mis = LX_Vector2D(-MISSILE_SPEED, 0);
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();

    pos_mis.x = position.x - MISSILE_WIDTH;
    pos_mis.y = position.y + ((position.h - MISSILE_HEIGHT)/ 2);
    pos_mis.w = MISSILE_WIDTH;
    pos_mis.h = MISSILE_HEIGHT;

    g->acceptEnemyMissile(new BasicMissile(attack_val,
                                           rc->getResource(RC_MISSILE, ENEMY_BMISSILE_ID),
                                           nullptr, pos_mis, sp_mis));
}


void Enemy::move()
{
    moveRect(position, speed);
    moveCircle(hitbox, speed);
}

// use the strategy
void Enemy::strategy()
{
    if(strat != nullptr)
        strat->proceed();
}


void Enemy::collision(Missile *mi)
{
    if(!mi->isDead() && mi->getX() <= (position.x + position.w) && !dying)
    {
        if(LX_Physics::collisionCircleRect(hitbox, *mi->getHitbox()))
        {
            reaction(mi);
            mi->die();
        }
    }
}


void Enemy::collision(Player *play)
{
    if(play->getX() <= (position.x + position.w) && !dying)
    {
        if(LX_Physics::collisionCircle(*play->getHitbox(), hitbox))
            play->die();
    }
}


// Define how the enemy react when it has collision with the following target
void Enemy::reaction(Missile *target)
{
    Score *sc = Engine::getInstance()->getScore();
    receiveDamages(target->hit());
    sc->notify(Scoring::DAMAGE_SCORE);
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

void Enemy::die()
{
    if(!dying)
    {
        dying = true;
        speed = LX_Vector2D(0.0f, 0.0f);
        addStrategy(new DeathStrategy(this, ENEMY_EXPLOSION_DELAY,
                                      ENEMY_EXPLOSION_DELAY));
    }
    else
    {
        // It is dead
        dying = false;
        Character::die();
    }
}

void Enemy::boom()
{
    if(sound != nullptr) sound->play();
}

const LX_Physics::LX_Circle * Enemy::getHitbox()
{
    return &hitbox;
}

