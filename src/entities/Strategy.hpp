#ifndef STRATEGY_H_INCLUDED
#define STRATEGY_H_INCLUDED



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
*	@file Strategy.hpp
*	@brief The strategy class
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <iostream>

#include "Missile.hpp"

class Enemy;

#define DELAY_ENEMY_MISSILE 1000
#define DELAY_ENEMY_ROCKET 5000
#define DELAY_ENEMY_LASER 10000
#define DELAY_ENEMY_BOMB 4000


class Strategy_exception : public std::exception
{
public :

    std::string str_err;

    Strategy_exception(std::string err);

    const char * what() const throw();

    ~Strategy_exception() throw() {}
};


class Strategy
{
protected:

    Enemy *target;

    double reference_time;      // The reference time
    double cur_time;            // The current time

    virtual void setVelocity(int vx, int vy);
    virtual void fire(MISSILE_TYPE m_type);


public :

    Strategy(Enemy *newEnemy);

    virtual void proceed(void) = 0;

    virtual ~Strategy() {}

};


// Move and shoot
class Basic_strategy: public Strategy
{
    double delay_missile;       // The delay between two basic missiles shots

public:

    Basic_strategy(Enemy *newEnemy);

    void proceed(void);

    ~Basic_strategy() {}

};

// Move and shoot! I do not mind how but do it!
class Move_and_shoot_strategy: public Strategy
{
    Strategy *move;
    Strategy *shoot;

public:

    Move_and_shoot_strategy(Enemy *newEnemy);

    void proceed(void);

    void addMoveStrat(Strategy * m);
    void addShotStrat(Strategy * s);

    ~Move_and_shoot_strategy();
};


// Move according to a virtual path
class Sinus_move_strategy: public Strategy
{
    int vx;
    int vy;

public:

    Sinus_move_strategy(Enemy *newEnemy);

    void proceed(void);

    ~Sinus_move_strategy();
};

// Just shoot!
class Shot_strategy: public Strategy
{
    int shot_delay;

public:

    Shot_strategy(Enemy *newEnemy);

    void setShotDelay(unsigned int delay);

    void proceed(void);

    ~Shot_strategy();
};


#endif // STRATEGY_H_INCLUDED

