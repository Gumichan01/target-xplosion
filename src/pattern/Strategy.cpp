
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
*   website : https://gumichan01.github.io/
*   mail : luxon.jean.pierre@gmail.com
*/

#include "Strategy.hpp"
#include "../entities/Missile.hpp"
#include "../entities/Enemy.hpp"
#include "BulletPattern.hpp"
#include "../game/Game.hpp"

#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Timer.hpp>

#include <cmath>


namespace
{
const int SINUS_MIN_Y = 77;
const int SINUS_MAX_Y = 650;
const unsigned int SHOT_DELAY = 1000;
const unsigned int DELAY_BASIC_ENEMY_MISSILE = 1000;

// For HeavisideStrat
const int HVS_YMIN = 100;
const int HVS_YMAX = 600;
const int R = (HVS_YMAX - HVS_YMIN)/2;
const float R_F = (static_cast<float>(HVS_YMAX- HVS_YMIN))/2.0f;

const int HVS_X1 = 868;
const int HVS_X2 = 768;
const int HVS_X25A = 660;
const int HVS_X25B = 676;
const int HVS_X3 = 568;
const int HVS_X4 = 468;
const int HVS_Y1 = 100;
const int HVS_Y2 = 200;
const int HVS_Y3 = 300;
const int HVS_Y4 = 400;
const int HVS_Y5 = 500;
const int HVS_Y6 = 600;
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

Strategy::~Strategy() {}

/**
    BasicStrategy implementation
    Shoot and move
*/
BasicStrategy::BasicStrategy(Enemy *newEnemy)
    : Strategy(newEnemy), delay_missile(DELAY_BASIC_ENEMY_MISSILE) {}


void BasicStrategy::proceed()
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


/** Sinus movement strategy */
PseudoSinusMoveStrategy::PseudoSinusMoveStrategy(Enemy *newEnemy)
    : Strategy(newEnemy), vx(newEnemy->getXvel()),vy(newEnemy->getYvel()) {}


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
    : Strategy(newEnemy),shot_delay(SHOT_DELAY) {}


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

/** Move */
MoveStrategy::MoveStrategy(Enemy *newEnemy)
    : Strategy(newEnemy) {}


void MoveStrategy::proceed()
{
    target->move();
}


/**
    Special movement: Heaviside
    → See http://www.wikiwand.com/en/Heaviside_step_function
*/
HeavisideStrat::HeavisideStrat(Enemy *newEnemy)
    : MoveStrategy(newEnemy), obj_speed(0),transition(0),
      alpha(static_cast<float>(BulletPattern::PI)/2.0f)
{
    using namespace LX_Physics;
    target->setY(HVS_Y1);
    float v = vector_norm(LX_Vector2D(target->getXvel(),target->getYvel()));
    obj_speed = static_cast<int>(v);
}


void HeavisideStrat::proceed()
{
    using namespace LX_Physics;
    int x = target->getX();
    int y = target->getY();

    Game *g = Game::getInstance();
    int x_mid = g->getXlim()/2;
    int y_mid = HVS_YMIN + R;
    int last_transition;

    LX_Point ctrl_point1(x_mid + R, y_mid);
    LX_Point ctrl_point2(x_mid - R, y_mid);
    last_transition = transition;

    if(x <= ctrl_point2.x || x > ctrl_point1.x)
    {
        transition = 0;
        target->setXvel(-obj_speed);
        target->setYvel(0);
        MoveStrategy::proceed();
    }
    else if(x <= ctrl_point1.x && y <= y_mid)
        transition = 1;
    else if(x > ctrl_point2.x || y > y_mid)
        transition = 2;

    if(transition == 1)
    {
        LX_Vector2D v;
        BulletPattern::shotOnTarget(static_cast<float>(x), static_cast<float>(y),
                                    static_cast<float>(ctrl_point1.x) + cosf(alpha) * R_F,
                                    static_cast<float>(ctrl_point1.y) - sinf(alpha) * R_F,
                                    -obj_speed, v);

        target->setXvel(v.vx);
        target->setYvel(v.vy);
        MoveStrategy::proceed();
        alpha += 0.04f;
    }
    else if(transition == 2)
    {
        if(last_transition == 1)
            alpha = -0.04f;

        LX_Vector2D v;
        BulletPattern::shotOnTarget(static_cast<float>(x), static_cast<float>(y),
                                    static_cast<float>(ctrl_point2.x) + cosf(alpha) * R_F,
                                    static_cast<float>(ctrl_point2.y) - sinf(alpha) * R_F,
                                    -obj_speed, v);

        target->setXvel(v.vx);
        target->setYvel(v.vy);
        MoveStrategy::proceed();
        alpha -= 0.04f;
    }
}


/**
    Move and shoot!
    That is all I want
*/
MoveAndShootStrategy::MoveAndShootStrategy(Enemy *newEnemy)
    : Strategy(newEnemy),move(nullptr),shoot(nullptr) {}


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


/// Do something when an enemy is dying
DeathStrategy::DeathStrategy(Enemy *newEnemy,uint32_t explosion_delay,
                             uint32_t noise_delay)
    : Strategy(newEnemy),ref_time(LX_Timer::getTicks()),noise_ref_time(LX_Timer::getTicks()),
      xplosion_duration(explosion_delay),noise_duration(noise_delay) {}


void DeathStrategy::proceed()
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

