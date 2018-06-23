
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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

#include <LunatiX/utils/float.hpp>
#include <vector>

class Enemy;

namespace LX_Physics
{
struct LX_FloatPosition;
}

class Strategy
{

    Strategy( const Strategy& ) = delete;
    Strategy( const Strategy&& ) = delete;
    Strategy& operator =( const Strategy& ) = delete;
    Strategy& operator =( const Strategy&& ) = delete;

protected:

    Enemy * target;
    unsigned int reference_time;      // The reference time

    virtual void setVelocity( int vx, int vy ) noexcept;

public:

    explicit Strategy( Enemy * newEnemy );

    virtual void proceed() noexcept = 0;
    virtual ~Strategy() = default;

};


// Just shoot!
class ShotStrategy final: public Strategy
{

    ShotStrategy( const ShotStrategy& ) = delete;
    ShotStrategy( const ShotStrategy&& ) = delete;
    ShotStrategy& operator =( const ShotStrategy& ) = delete;
    ShotStrategy& operator =( const ShotStrategy&& ) = delete;

protected:

    unsigned int shot_delay;

public:

    explicit ShotStrategy( Enemy * newEnemy );

    void setShotDelay( unsigned int delay ) noexcept;
    void proceed() noexcept override;

    ~ShotStrategy() = default;
};


class MultiStrategy final: public Strategy
{
    std::vector<Strategy *> stvec;

    MultiStrategy( const MultiStrategy& ) = delete;
    MultiStrategy( const MultiStrategy&& ) = delete;
    MultiStrategy& operator =( const MultiStrategy& ) = delete;
    MultiStrategy& operator =( const MultiStrategy&& ) = delete;

public:

    explicit MultiStrategy( Enemy * newEnemy );

    void proceed() noexcept override;
    void addStrat( Strategy& s ) noexcept;
    void reset() noexcept;

    ~MultiStrategy();
};


class MoveStrategy: public Strategy
{

    MoveStrategy( const MoveStrategy& ) = delete;
    MoveStrategy( const MoveStrategy&& ) = delete;
    MoveStrategy& operator =( const MoveStrategy& ) = delete;
    MoveStrategy& operator =( const MoveStrategy&& ) = delete;

public:

    explicit MoveStrategy( Enemy * newEnemy );
    virtual void proceed() noexcept override;

    virtual ~MoveStrategy() = default;
};


class UpDownMoveStrategy: public MoveStrategy
{
    int ylim_up;    // Y limit on the top of the screen
    int ylim_down;  // Y limit on the down of the screen
    float yvel_base;
    float yvel_cur;

    UpDownMoveStrategy( const UpDownMoveStrategy& ) = delete;
    UpDownMoveStrategy( const UpDownMoveStrategy&& ) = delete;
    UpDownMoveStrategy& operator =( const UpDownMoveStrategy& ) = delete;
    UpDownMoveStrategy& operator =( const UpDownMoveStrategy&& ) = delete;

public:

    explicit UpDownMoveStrategy( Enemy * newEnemy, int ylimu, int ylimd, int yvelb );
    virtual void proceed() noexcept override;

    virtual ~UpDownMoveStrategy() = default;
};

// Move according to a virtual path
class PseudoSinusMoveStrategy final: public UpDownMoveStrategy
{

    PseudoSinusMoveStrategy( const PseudoSinusMoveStrategy& ) = delete;
    PseudoSinusMoveStrategy( const PseudoSinusMoveStrategy&& ) = delete;
    PseudoSinusMoveStrategy& operator =( const PseudoSinusMoveStrategy& ) = delete;
    PseudoSinusMoveStrategy& operator =( const PseudoSinusMoveStrategy&& ) = delete;

public:

    explicit PseudoSinusMoveStrategy( Enemy * newEnemy );
    void proceed() noexcept override;

    ~PseudoSinusMoveStrategy() = default;
};

class HeavisideStrat: public MoveStrategy
{

    HeavisideStrat( const HeavisideStrat& ) = delete;
    HeavisideStrat( const HeavisideStrat&& ) = delete;
    HeavisideStrat& operator =( const HeavisideStrat& ) = delete;
    HeavisideStrat& operator =( const HeavisideStrat&& ) = delete;

protected:

    const Float HSTEP = {0.04f};
    Float obj_speed;
    int transition;
    Float alpha;

    void _proceed( float x, float y, const LX_Physics::LX_FloatPosition& p ) noexcept;

public:
    explicit HeavisideStrat( Enemy * newEnemy );
    virtual void proceed() noexcept override;

    virtual ~HeavisideStrat() = default;
};


class HeavisideReverseStrat final: public HeavisideStrat
{

    HeavisideReverseStrat( const HeavisideReverseStrat& ) = delete;
    HeavisideReverseStrat( const HeavisideReverseStrat&& ) = delete;
    HeavisideReverseStrat& operator =( const HeavisideReverseStrat& ) = delete;
    HeavisideReverseStrat& operator =( const HeavisideReverseStrat&& ) = delete;

public:
    explicit HeavisideReverseStrat( Enemy * newEnemy );
    virtual void proceed() noexcept override;

    ~HeavisideReverseStrat() = default;
};


// Move and shoot! I do not mind how but do it!
class MoveAndShootStrategy final: public Strategy
{
    Strategy * move;
    Strategy * shoot;

    MoveAndShootStrategy( const MoveAndShootStrategy& ) = delete;
    MoveAndShootStrategy( const MoveAndShootStrategy&& ) = delete;
    MoveAndShootStrategy& operator =( const MoveAndShootStrategy& ) = delete;
    MoveAndShootStrategy& operator =( const MoveAndShootStrategy&& ) = delete;

public:

    explicit MoveAndShootStrategy( Enemy * newEnemy );

    void proceed() noexcept override;
    void addMoveStrat( Strategy * m ) noexcept;
    void addShotStrat( Strategy * s ) noexcept;

    ~MoveAndShootStrategy();
};


class DeathStrategy: public Strategy
{

    DeathStrategy( const DeathStrategy& ) = delete;
    DeathStrategy( const DeathStrategy&& ) = delete;
    DeathStrategy& operator =( const DeathStrategy& ) = delete;
    DeathStrategy& operator =( const DeathStrategy&& ) = delete;

protected:

    unsigned int ref_time;            // Reference time of explosion
    unsigned int noise_ref_time;      // Reference time of explosion
    unsigned int xplosion_duration;   // Time of the complete boss explosion
    unsigned int noise_duration;      // Time of each single explosion noise

public:

    explicit DeathStrategy( Enemy * newEnemy, unsigned int explosion_delay,
                            unsigned int noise_delay );
    void proceed() noexcept override;

    ~DeathStrategy() = default;
};

class BossDeathStrategy final: public DeathStrategy
{

    BossDeathStrategy( const BossDeathStrategy& ) = delete;
    BossDeathStrategy( const BossDeathStrategy&& ) = delete;
    BossDeathStrategy& operator =( const BossDeathStrategy& ) = delete;
    BossDeathStrategy& operator =( const BossDeathStrategy&& ) = delete;

public:

    explicit BossDeathStrategy( Enemy * newEnemy, unsigned int explosion_delay,
                                unsigned int noise_delay );
    void proceed() noexcept override;

    ~BossDeathStrategy() = default;
};


#endif // STRATEGY_H_INCLUDED
