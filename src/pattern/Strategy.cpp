
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

#include "Strategy.hpp"
#include "BulletPattern.hpp"
#include "../entities/Enemy.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/engine/EntityHandler.hpp"

#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Timer.hpp>

#include <cmath>

#define iabs(x) (static_cast<int>(x > 0 ? x : -x))

namespace
{
const int SINUS_MIN_Y = 70;
const int SINUS_MAX_Y = 640;
const unsigned int SHOT_DELAY = 1000;
const unsigned int DELAY_BASIC_ENEMY_MISSILE = 1000;

// For HeavisideStrat
const int HVS_YMIN = 100;
const int HVS_YMAX = 600;
const int YMID = (HVS_YMAX - HVS_YMIN)/2;
const float YMID_F = (static_cast<float>(HVS_YMAX- HVS_YMIN))/2.0f;

const int HVS_Y1 = 100;
const int HVS_Y6 = 600;

// Up and down movement
const int YOFF = 50;
}

using namespace FloatBox;

/** Strategy implementation */
Strategy::Strategy(Enemy *newEnemy)
    : target(newEnemy), reference_time(LX_Timer::getTicks())
{
    target = newEnemy;
    reference_time = LX_Timer::getTicks();
}


void Strategy::setVelocity(int vx, int vy) noexcept
{
    target->setXvel(vx);
    target->setYvel(vy);
}


/** Sinus movement strategy */
PseudoSinusMoveStrategy::PseudoSinusMoveStrategy(Enemy *newEnemy)
    : UpDownMoveStrategy(newEnemy, SINUS_MIN_Y, SINUS_MAX_Y,
                         newEnemy->getYvel()) {}


void PseudoSinusMoveStrategy::proceed() noexcept
{
    UpDownMoveStrategy::proceed();
}

/**
    Shot Strategy
    Shoot, shoot, shoot!
    That is all!
*/
ShotStrategy::ShotStrategy(Enemy *newEnemy)
    : Strategy(newEnemy), shot_delay(SHOT_DELAY) {}


void ShotStrategy::setShotDelay(unsigned int delay) noexcept
{
    shot_delay = delay;
}


void ShotStrategy::proceed() noexcept
{
    if((LX_Timer::getTicks() - reference_time) > shot_delay)
    {
        target->fire();
        reference_time = LX_Timer::getTicks();
    }
}

/**
    Multiple strategy
*/
MultiStrategy::MultiStrategy(Enemy *newEnemy) : Strategy(newEnemy), stvec() {}

void MultiStrategy::proceed() noexcept
{
    for(Strategy* st: stvec)
        st->proceed();
}

void MultiStrategy::addStrat(Strategy& s) noexcept
{
    stvec.push_back(&s);
}

void MultiStrategy::reset() noexcept
{
    stvec.clear();
}

MultiStrategy::~MultiStrategy()
{
    reset();
}


/** Move */
MoveStrategy::MoveStrategy(Enemy *newEnemy)
    : Strategy(newEnemy) {}


void MoveStrategy::proceed() noexcept
{
    target->move();
}


/** Up and down movement */
UpDownMoveStrategy::UpDownMoveStrategy(Enemy *newEnemy, int ylimu, int ylimd, int yvelb)
    : MoveStrategy(newEnemy), ylim_up(ylimu), ylim_down(ylimd), yvel_base(yvelb),
      yvel_cur(0)
{
    target->setYvel(yvelb);
}


void UpDownMoveStrategy::proceed() noexcept
{
    int y = target->getY();
    yvel_cur = target->getYvel();

    if(y < ylim_up || y > ylim_down)
        target->setYvel(-yvel_cur);

    else if(y < ylim_up + YOFF || y > ylim_down - YOFF)
    {
        if(fabs(yvel_cur) >= fabs(yvel_base))
            target->setYvel(yvel_cur / 2.0f);
    }
    else
    {
        if(iabs(yvel_cur) < iabs(yvel_base))
            target->setYvel(yvel_cur * 2.0f);
    }

    MoveStrategy::proceed();
}



/**
    Special movement: Heaviside
    → See http://www.wikiwand.com/en/Heaviside_step_function
*/
HeavisideStrat::HeavisideStrat(Enemy *newEnemy)
    : MoveStrategy(newEnemy), obj_speed(
{
    FNIL
}), transition(0),
alpha(BulletPattern::PI_F / Float{2.0f})
{
    using namespace LX_Physics;
    target->setY(HVS_Y1);
    obj_speed = vector_norm(LX_Vector2D{target->getXvel(), target->getYvel()});
}

void HeavisideStrat::_proceed(float x, float y, const LX_Physics::LX_FloatPosition& p) noexcept
{
    using namespace LX_Physics;
    LX_Vector2D v;
    BulletPattern::shotOnTarget(fbox(x), fbox(y),
                                p.x + fbox(std::cos(alpha.v) * YMID_F),
                                p.y - fbox(std::sin(alpha.v) * YMID_F),
                                -obj_speed, v);

    target->setXvel(v.vx);
    target->setYvel(v.vy);
}


void HeavisideStrat::proceed() noexcept
{
    using namespace LX_Physics;

    const Float& x = target->getX();
    const Float& y = target->getY();

    const Float X_MID = Engine::getMaxXlim() / fbox(2.0f);
    constexpr Float Y_MID = fbox(HVS_YMIN) + fbox(YMID_F);
    const LX_FloatPosition CTRL_P1{X_MID + YMID, Y_MID};
    const LX_FloatPosition CTRL_P2{X_MID - YMID, Y_MID};
    int last_transition = transition;

    if(x <= CTRL_P2.x || x > CTRL_P1.x)
    {
        transition = 0;
        target->setXvel(-obj_speed.v);
        target->setYvel(0.0f);
    }
    else if(x <= CTRL_P1.x && y <= Y_MID)
    {
        transition = 1;
        _proceed(x, y, CTRL_P1);
        alpha += HSTEP;
    }
    else if(x > CTRL_P2.x || y > Y_MID)
    {
        transition = 2;
        if(last_transition == 1)
            alpha = -HSTEP;

        _proceed(x, y, CTRL_P2);
        alpha -= HSTEP;
    }

    MoveStrategy::proceed();
}


HeavisideReverseStrat::HeavisideReverseStrat(Enemy *newEnemy)
    : HeavisideStrat(newEnemy)
{
    target->setY(HVS_Y6);
    alpha = -(BulletPattern::PI_F / fbox(2.0f));
}

// The algorithm is the same as HeavisideStrat::proceed(),
// the value of alpha has just been changed
void HeavisideReverseStrat::proceed() noexcept
{
    using namespace LX_Physics;
    const Float X = target->getX();
    const Float Y = target->getY();
    const Float X_MID = Engine::getMaxXlim() / fbox(2.0f);
    const Float Y_MID = fbox(HVS_YMIN) + fbox(YMID_F);
    const LX_FloatPosition CTRL_P1{X_MID + YMID, Y_MID};
    const LX_FloatPosition CTRL_P2{X_MID - YMID, Y_MID};
    int last_transition = transition;

    if(X <= CTRL_P2.x || X > CTRL_P1.x)
    {
        transition = 0;
        target->setXvel(-obj_speed);
        target->setYvel(0);
    }
    else if(X <= CTRL_P1.x && Y >= Y_MID)
    {
        transition = 1;
        _proceed(X, Y, CTRL_P1);
        alpha -= HSTEP;
    }
    else if(X > CTRL_P2.x || Y < Y_MID)
    {
        transition = 2;
        if(last_transition == 1)
            alpha = HSTEP;

        _proceed(X, Y, CTRL_P2);
        alpha += HSTEP;
    }

    MoveStrategy::proceed();
}


/**
    Move and shoot!
    That is all I want
*/
MoveAndShootStrategy::MoveAndShootStrategy(Enemy *newEnemy)
    : Strategy(newEnemy), move(nullptr), shoot(nullptr) {}


MoveAndShootStrategy::~MoveAndShootStrategy()
{
    delete move;
    delete shoot;
    move = nullptr;
    shoot = nullptr;
}


void MoveAndShootStrategy::addMoveStrat(Strategy *m) noexcept
{
    delete move;
    move = m;
}


void MoveAndShootStrategy::addShotStrat(Strategy *s) noexcept
{
    delete shoot;
    shoot = s;
}

void MoveAndShootStrategy::proceed() noexcept
{
    if(shoot != nullptr)
        shoot->proceed();
    if(move != nullptr)
        move->proceed();
}


/// Do something when an enemy is dying
DeathStrategy::DeathStrategy(Enemy *newEnemy, unsigned int explosion_delay,
                             unsigned int noise_delay)
    : Strategy(newEnemy), ref_time(LX_Timer::getTicks()),
      noise_ref_time(LX_Timer::getTicks()), xplosion_duration(explosion_delay),
      noise_duration(noise_delay) {}


void DeathStrategy::proceed() noexcept
{
    unsigned int ticks = LX_Timer::getTicks();

    if((ticks - ref_time) > xplosion_duration)
        target->die();
    else
    {
        target->move();

        if((ticks - noise_ref_time) > noise_duration)
        {
            target->boom();
            noise_ref_time = ticks;
        }
    }
}

BossDeathStrategy::BossDeathStrategy(Enemy *newEnemy, unsigned int explosion_delay,
                                     unsigned int noise_delay)
    : DeathStrategy(newEnemy, explosion_delay, noise_delay) {}


void BossDeathStrategy::proceed() noexcept
{
    DeathStrategy::proceed();
    EntityHandler::getInstance().bulletCancel();
}
