
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
*	@file Strategy.cpp
*	@brief The strategies implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL_timer.h>

#include "Strategy.hpp"
#include "Enemy.hpp"

#include "../game/Game.hpp"


static const int SINUS_MIN_Y = 77;
static const int SINUS_MAX_Y = 650;
static const int SHOT_DELAY = 1000;



StrategyException::StrategyException(std::string err)
{
    str_err = err;
}


const char * StrategyException::what() const throw()
{
    return str_err.c_str();
}


/* Strategy implementation */

Strategy::Strategy(Enemy *newEnemy)
{
    if(newEnemy == NULL)
    {
        throw StrategyException("Null strategy");
    }

    target = newEnemy;
    reference_time = SDL_GetTicks();
}

void Strategy::fire(MISSILE_TYPE m_type)
{
    // Empty
}


void Strategy::setVelocity(int vx, int vy)
{
    target->setXvel(vx);
    target->setYvel(vy);
}


/*
    BasicStrategy implementation
    Shoot and move
*/


BasicStrategy::BasicStrategy(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    delay_missile = DELAY_ENEMY_MISSILE;
}


void BasicStrategy::proceed(void)
{
    if(!target->isDead())
    {
        cur_time = SDL_GetTicks();

        if( cur_time - reference_time >= DELAY_ENEMY_MISSILE)
        {
            reference_time = cur_time;

            Game *tmp = Game::getInstance();
            tmp->addEnemyMissile(target->shoot(MISSILE_TYPE::BASIC_MISSILE_TYPE));
        }

        target->move();
    }
}


/* Sinus movement strategy */

PseudoSinusMoveStrategy::PseudoSinusMoveStrategy(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    vx = newEnemy->getXvel();
    vy = newEnemy->getYvel();
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
        vy = ((tmp_vy < 0)? (tmp_vy): -tmp_vy);
    }

    setVelocity(vx,vy);
    target->move();
}

/*
    Shot Strategy
    Shoot, shoot, shoot!
    No movement
*/

ShotStrategy::ShotStrategy(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    shot_delay = SHOT_DELAY;
    reference_time = SDL_GetTicks();
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
    Game *g = Game::getInstance();

    if( (SDL_GetTicks() - reference_time) > shot_delay)
    {
        g->addEnemyMissile(target->shoot());
        reference_time = SDL_GetTicks();
    }
}

// Move and shoot! That is all I want
MoveAndShootStrategy::MoveAndShootStrategy(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    move = NULL;
    shoot = NULL;
}


MoveAndShootStrategy::~MoveAndShootStrategy()
{
    delete move;
    delete shoot;
    move = NULL;
    shoot = NULL;
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
















