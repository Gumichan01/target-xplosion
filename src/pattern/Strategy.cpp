
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
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/

#include "Strategy.hpp"
#include "../entities/Missile.hpp"
#include "../entities/Enemy.hpp"
#include "../game/Game.hpp"

#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Timer.hpp>


namespace
{
const int SINUS_MIN_Y = 77;
const int SINUS_MAX_Y = 650;
const unsigned int SHOT_DELAY = 1000;
const unsigned int DELAY_BASIC_ENEMY_MISSILE = 1000;
};


/** Strategy implementation */
Strategy::Strategy(Enemy *newEnemy)
    : target(newEnemy),
      reference_time(LX_Timer::getTicks()), cur_time(0)
{
    target = newEnemy;
    reference_time = LX_Timer::getTicks();
}


void Strategy::setVelocity(int vx, int vy)
{
    target->setXvel(vx);
    target->setYvel(vy);
}

Strategy::~Strategy()
{
    // Empty
}

/**
    BasicStrategy implementation
    Shoot and move
*/
BasicStrategy::BasicStrategy(Enemy *newEnemy)
    : Strategy(newEnemy), delay_missile(DELAY_BASIC_ENEMY_MISSILE)
{
    // Empty
}


void BasicStrategy::proceed(void)
{
    if(!target->isDead())
    {
        cur_time = LX_Timer::getTicks();

        if((cur_time - reference_time) >= delay_missile)
        {
            reference_time = cur_time;
            target->fire();
        }

        target->move();
    }
}


BasicStrategy::~BasicStrategy()
{
    // Empty
}

/** Sinus movement strategy */
PseudoSinusMoveStrategy::PseudoSinusMoveStrategy(Enemy *newEnemy)
    : Strategy(newEnemy), vx(newEnemy->getXvel()),vy(newEnemy->getYvel())
{
    // Empty
}


PseudoSinusMoveStrategy::~PseudoSinusMoveStrategy()
{
    // Empty
}

void PseudoSinusMoveStrategy::proceed()
{
    int tmp_vy = target->getYvel();

    if(target->getY() < SINUS_MIN_Y)
    {
        vy = ((tmp_vy < 0)? (-tmp_vy): tmp_vy);
    }
    else if(target->getY() > SINUS_MAX_Y)
    {
        vy = ((tmp_vy < 0)? tmp_vy: (-tmp_vy));
    }

    setVelocity(vx,vy);
    target->move();
}

/**
    Shot Strategy
    Shoot, shoot, shoot!
    That is all!
*/
ShotStrategy::ShotStrategy(Enemy *newEnemy)
    : Strategy(newEnemy),shot_delay(SHOT_DELAY)
{
    // Empty
}

ShotStrategy::~ShotStrategy()
{
    // Empty
}


void ShotStrategy::setShotDelay(unsigned int delay)
{
    shot_delay = delay;
}


void ShotStrategy::proceed()
{
    if((LX_Timer::getTicks() - reference_time) > shot_delay)
    {
        target->fire();
        reference_time = LX_Timer::getTicks();
    }
}

/**
    Move and shoot!
    That is all I want
*/
MoveAndShootStrategy::MoveAndShootStrategy(Enemy *newEnemy)
    : Strategy(newEnemy),move(nullptr),shoot(nullptr)
{
    // Empty
}


MoveAndShootStrategy::~MoveAndShootStrategy()
{
    delete move;
    delete shoot;
    move = nullptr;
    shoot = nullptr;
}


void MoveAndShootStrategy::addMoveStrat(Strategy *m)
{
    delete move;
    move = m;
}


void MoveAndShootStrategy::addShotStrat(Strategy *s)
{
    delete shoot;
    shoot = s;
}

void MoveAndShootStrategy::proceed()
{
    shoot->proceed();
    move->proceed();
}

/** Move */
MoveStrategy::MoveStrategy(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    // Empty
}


MoveStrategy::~MoveStrategy()
{
    // Empty
}

void MoveStrategy::proceed()
{
    target->move();
}

/// Do something when an enemy is dying
DeathStrategy::DeathStrategy(Enemy *newEnemy,uint32_t explosion_delay,
                             uint32_t noise_delay)
    : Strategy(newEnemy),ref_time(LX_Timer::getTicks()),noise_ref_time(LX_Timer::getTicks()),
      xplosion_duration(explosion_delay),noise_duration(noise_delay)
{

}


void DeathStrategy::proceed(void)
{
    Game *g = Game::getInstance();
    uint32_t ticks = LX_Timer::getTicks();
    uint32_t total_noise_duration = xplosion_duration - 1000;

    if((ticks - ref_time) > xplosion_duration)
        target->die();
    else
        target->move();

    ticks = LX_Timer::getTicks();

    if((ticks - ref_time) < total_noise_duration)
    {
        if((ticks - noise_ref_time) > noise_duration)
        {
            target->boom();
            noise_ref_time = ticks;
        }
    }

    if(g != nullptr)
        g->screenCancel();
}


DeathStrategy::~DeathStrategy()
{
    // Empty
}
