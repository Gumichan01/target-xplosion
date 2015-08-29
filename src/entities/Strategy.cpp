
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

static const int SINUS_SPEED_X = -10;
static const int SINUS_DOWN_SPEED_Y = 7;
static const int SINUS_UP_SPEED_Y = -7;



Strategy_exception::Strategy_exception(std::string err)
{
    str_err = err;
}


const char * Strategy_exception::what() const throw()
{
    return str_err.c_str();
}


/* Strategy implementation */

Strategy::Strategy(Enemy *newEnemy)
{
    if(newEnemy == NULL)
    {
        throw Strategy_exception("Null strategy");
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
    target->set_Xvel(vx);
    target->set_Yvel(vy);
}


/*
    Basic_strategy implementation
    Shoot and move
*/


Basic_strategy::Basic_strategy(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    delay_missile = DELAY_ENEMY_MISSILE;
}


void Basic_strategy::proceed(void)
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

Sinus_move_strategy::Sinus_move_strategy(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    vx = SINUS_SPEED_X;
    vy = SINUS_DOWN_SPEED_Y;
}


Sinus_move_strategy::~Sinus_move_strategy()
{
    // Empty
}

void Sinus_move_strategy::proceed()
{
    if(target->getY() < SINUS_MIN_Y)
    {
        vy = SINUS_DOWN_SPEED_Y;
    }
    else if(target->getY() > SINUS_MAX_Y)
    {
        vy = SINUS_UP_SPEED_Y;
    }

    setVelocity(vx,vy);
    target->move();
}












