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

#include "../entities/Missile.hpp"

class Enemy;

#define DELAY_BASIC_ENEMY_MISSILE 1000
#define DELAY_ENEMY_MISSILE 750
#define DELAY_ENEMY_ROCKET 5000
#define DELAY_ENEMY_LASER 10000
#define DELAY_ENEMY_BOMB 4000


class Strategy
{
protected:

    Enemy *target;
    Uint32 reference_time;      // The reference time
    Uint32 cur_time;            // The current time

    virtual void setVelocity(int vx, int vy);

public :

    Strategy(Enemy *newEnemy);

    virtual void proceed(void) = 0;
    virtual ~Strategy();

};

// Move and shoot
class BasicStrategy: public Strategy
{
    Uint32 delay_missile;       // The delay between two basic missiles shots

public:

    BasicStrategy(Enemy *newEnemy);
    void proceed(void);

    ~BasicStrategy();
};

// Move and shoot! I do not mind how but do it!
class MoveAndShootStrategy: public Strategy
{
    Strategy *move;
    Strategy *shoot;

public:

    MoveAndShootStrategy(Enemy *newEnemy);

    void proceed(void);
    void addMoveStrat(Strategy * m);
    void addShotStrat(Strategy * s);

    ~MoveAndShootStrategy();
};


// Move according to a virtual path
class PseudoSinusMoveStrategy: public Strategy
{
    int vx, vy;     // X and Y velocity

public:

    PseudoSinusMoveStrategy(Enemy *newEnemy);
    void proceed(void);

    ~PseudoSinusMoveStrategy();
};

// Just shoot!
class ShotStrategy: public Strategy
{
    Uint32 shot_delay;

public:

    ShotStrategy(Enemy *newEnemy);

    void setShotDelay(Uint32 delay);
    void proceed(void);

    ~ShotStrategy();
};


class MoveStrategy: public Strategy
{

public:

    MoveStrategy(Enemy *newEnemy);
    void proceed(void);

    ~MoveStrategy();
};


class DeathStrategy: public Strategy
{
    Uint32 ref_time;            // Reference time of explosion
    Uint32 noise_ref_time;            // Reference time of explosion
    Uint32 xplosion_duration;   // Time of the complete boss explosion
    Uint32 noise_duration;      // Time of each single explosion noise

public:

    DeathStrategy(Enemy *newEnemy,Uint32 explosion_delay,Uint32 noise_delay);
    void proceed(void);

    ~DeathStrategy();
};

#endif // STRATEGY_H_INCLUDED

