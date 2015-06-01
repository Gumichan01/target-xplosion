

/*
*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*
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





























