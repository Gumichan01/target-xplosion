
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
*	@file Strategy.cpp
*	@brief The strategies implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/


#include "Strategy.hpp"
#include "Enemy.hpp"

#include "../game/Game.hpp"


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

/* Basic_strategy implementation */


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

