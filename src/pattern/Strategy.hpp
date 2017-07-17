
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

#ifndef STRATEGY_H_INCLUDED
#define STRATEGY_H_INCLUDED

class Enemy;

namespace LX_Physics
{
struct LX_Point;
}

class Strategy
{
protected:

    Enemy *target;
    unsigned int reference_time;      // The reference time
    unsigned int cur_time;            // The current time

    virtual void setVelocity(int vx, int vy);

public:

    explicit Strategy(Enemy *newEnemy);

    virtual void proceed() = 0;
    virtual ~Strategy();

};

// Move and shoot
class BasicStrategy: public Strategy
{
    unsigned int delay_missile;       // The delay between two basic missiles shots

public:

    explicit BasicStrategy(Enemy *newEnemy);
    void proceed();

    ~BasicStrategy() = default;
};


// Just shoot!
class ShotStrategy: public Strategy
{
    unsigned int shot_delay;

public:

    explicit ShotStrategy(Enemy *newEnemy);

    void setShotDelay(unsigned int delay);
    void proceed();

    ~ShotStrategy() = default;
};


class MoveStrategy: public Strategy
{

public:

    explicit MoveStrategy(Enemy *newEnemy);
    virtual void proceed();

    virtual ~MoveStrategy() = default;
};


class UpDownMoveStrategy: public MoveStrategy
{
    int ylim_up;    // Y limit on the top of the screen
    int ylim_down;  // Y limit on the down of the screen
    float yvel_base;
    float yvel_cur;

public:

    explicit UpDownMoveStrategy(Enemy *newEnemy, int ylimu, int ylimd, int yvelb);
    virtual void proceed();

    virtual ~UpDownMoveStrategy() = default;
};

// Move according to a virtual path
class PseudoSinusMoveStrategy: public UpDownMoveStrategy
{
public:

    explicit PseudoSinusMoveStrategy(Enemy *newEnemy);
    void proceed();

    ~PseudoSinusMoveStrategy() = default;
};

class HeavisideStrat: public MoveStrategy
{
protected:

    const float HSTEP = 0.04f;
    int obj_speed;
    int transition;
    float alpha;

    void _proceed(float x, float y, const LX_Physics::LX_Point& p);

public:
    explicit HeavisideStrat(Enemy *newEnemy);
    virtual void proceed();

    virtual ~HeavisideStrat() = default;
};


class HeavisideReverseStrat: public HeavisideStrat
{

public:
    explicit HeavisideReverseStrat(Enemy *newEnemy);
    virtual void proceed();

    ~HeavisideReverseStrat() = default;
};


// Move and shoot! I do not mind how but do it!
class MoveAndShootStrategy: public Strategy
{
    Strategy *move;
    Strategy *shoot;

public:

    explicit MoveAndShootStrategy(Enemy *newEnemy);

    void proceed();
    void addMoveStrat(Strategy * m);
    void addShotStrat(Strategy * s);

    ~MoveAndShootStrategy();
};


class DeathStrategy: public Strategy
{
protected:

    unsigned int ref_time;            // Reference time of explosion
    unsigned int noise_ref_time;      // Reference time of explosion
    unsigned int xplosion_duration;   // Time of the complete boss explosion
    unsigned int noise_duration;      // Time of each single explosion noise

public:

    explicit DeathStrategy(Enemy *newEnemy, unsigned int explosion_delay,
                           unsigned int noise_delay);
    void proceed();

    ~DeathStrategy() = default;
};

class BossDeathStrategy: public DeathStrategy
{

public:

    explicit BossDeathStrategy(Enemy *newEnemy, unsigned int explosion_delay,
                               unsigned int noise_delay);
    void proceed();

    ~BossDeathStrategy() = default;
};


#endif // STRATEGY_H_INCLUDED
