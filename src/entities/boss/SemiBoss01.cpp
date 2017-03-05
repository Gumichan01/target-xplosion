
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


#include "SemiBoss01.hpp"
#include "../Bullet.hpp"
#include "../BasicMissile.hpp"

#include "../../game/Game.hpp"
#include "../../pattern/BulletPattern.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>

using namespace LX_Physics;


namespace
{
const int SEMIBOSS_BULLET_ID = 8;

const int NB_SHOTS = 2;

const int XMIN = 1000;
const int X_OFFSET =  XMIN + 16;
const int YMIN = 47;
const int YMAX = 500;
const int YMIN_OFFSET = YMIN + 24;
const int YMAX_OFFSET =  YMAX - 24;
const uint32_t DELAY_TO_SHOOT = 1000;

const int SHOT1_OFFSET = 72;
const int SHOT2_OFFSET = 140;
const int HOMING_SHOT_OFFSET = SHOT1_OFFSET + (SHOT2_OFFSET - SHOT1_OFFSET);
const int BULLETX_OFFSET = 108;
const int BULLET_VELOCITY = 12;
const int HOMING_BULLET_VELOCITY = -6;

};


SemiBoss01::SemiBoss01(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
                       int x, int y, int w, int h, float vx, float vy)
    : Boss(hp, att, sh, image, audio, x, y, w, h, vx, vy),
      shot_delay(DELAY_TO_SHOOT), begin_time(0), old_state(LIFE_OK),
      current_state(LIFE_OK), mvs(nullptr)
{
    hitbox.radius = 100;
    hitbox.square_radius = hitbox.radius*hitbox.radius;

    mvs = new MoveAndShootStrategy(this);
    ShotStrategy *s = new ShotStrategy(this);
    MoveStrategy *m = new MoveStrategy(this);

    s->setShotDelay(DELAY_TO_SHOOT);
    mvs->addMoveStrat(m);
    mvs->addShotStrat(s);
}

SemiBoss01::~SemiBoss01()
{
    delete mvs;
    mvs = nullptr;
}

bool SemiBoss01::canShoot() const
{
    /*
        If the boss is close to a specific X position and is going to the left,
        OR if the boss is close to a specific Y maximum/minimum position
        and is going to the bottom/top of the screen, then it cannot shoot
    */
    if((position.x > XMIN && position.x < X_OFFSET && speed.vx < 0)
            || (position.y < YMAX && position.y > YMAX_OFFSET && speed.vy > 0)
            || (position.y > YMIN && position.y < YMIN_OFFSET && speed.vy < 0))
    {
        return false;
    }

    return true;
}


void SemiBoss01::homingShot()
{
    const int SZ = 16;

    LX_Vector2D v;
    Game *g = Game::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_AABB rect = {position.x,(position.y + (position.w/2)), SZ, SZ};

    BulletPattern::shotOnPlayer(position.x, position.y + HOMING_SHOT_OFFSET,
                                HOMING_BULLET_VELOCITY, v);

    g->acceptEnemyMissile(new BasicMissile(attack_val,
                                           rc->getResource(RC_MISSILE, SEMIBOSS_BULLET_ID),
                                           nullptr, rect, v));
}


void SemiBoss01::strategy()
{
    if(!dying)
        mvs->proceed();
    else
        Enemy::strategy();
}


// Direct shot from the semi-boss
void SemiBoss01::fire()
{
    unsigned int one_quarter_hp = max_health_point/3;
    unsigned int one_eighth_hp = one_quarter_hp/2;
    static uint32_t r_time = 0;

    if((LX_Timer::getTicks() - r_time) > DELAY_TO_SHOOT)
    {
        shoot(BASIC_MISSILE_TYPE);
        r_time = LX_Timer::getTicks();
    }
    // Update the shot strategy if the stage of the boss has changed
    if(current_state != old_state)
    {
        ShotStrategy *s = new ShotStrategy(this);
        s->setShotDelay(shot_delay);
        mvs->addShotStrat(s);
        old_state = current_state;
    }

    if((LX_Timer::getTicks() - begin_time) > shot_delay)
    {
        // Shoot
        if(health_point > (max_health_point - one_quarter_hp))
        {
            shoot(BULLETV1_TYPE);
            begin_time = LX_Timer::getTicks();
        }
        else if(health_point > one_quarter_hp)
        {
            shot_delay = DELAY_TO_SHOOT/2;
            shoot(BULLETV2_TYPE);
            begin_time = LX_Timer::getTicks();
            current_state = LIFE_WARNING;
        }
        else if(health_point > one_eighth_hp)
        {
            shot_delay = DELAY_TO_SHOOT/4;
            shoot(BULLETV2_TYPE);
            begin_time = LX_Timer::getTicks();
            current_state = LIFE_CRITICAL;
        }
        else
        {
            shot_delay = DELAY_TO_SHOOT/8;
            shoot(BULLETV1_TYPE);
            shoot(BULLETV2_TYPE);
            begin_time = LX_Timer::getTicks();
            current_state = LIFE_CRITICAL;
        }
    }
}

// Circular shot
void SemiBoss01::shoot(const MISSILE_TYPE& m_type)
{
    LX_AABB rect[NB_SHOTS];
    Game *g = Game::getInstance();
    LX_Vector2D vel(speed.vx, speed.vy);
    const ResourceManager * rc = ResourceManager::getInstance();
    // If the boss cannot shoot according to its position
    // Do not shoot!
    if(!canShoot())
        return;

    if(m_type == BULLETV1_TYPE)
    {
        rect[0] = {position.x, position.y + SHOT1_OFFSET, 32, 32};
        rect[1] = {position.x, position.y + SHOT2_OFFSET, 32, 32};
    }
    else if(m_type == BULLETV2_TYPE)
    {
        rect[0] = {position.x + BULLETX_OFFSET, position.y + SHOT1_OFFSET, 32, 32};
        rect[1] = {position.x + BULLETX_OFFSET, position.y + SHOT2_OFFSET, 32, 32};
    }
    else if(m_type == BASIC_MISSILE_TYPE)
    {
        homingShot();
        return;
    }

    g->acceptEnemyMissile(new MegaBullet(attack_val,
                                         rc->getResource(RC_MISSILE, 4),
                                         nullptr, rect[0], vel, BULLET_VELOCITY));

    g->acceptEnemyMissile(new MegaBullet(attack_val,
                                         rc->getResource(RC_MISSILE, 4),
                                         nullptr, rect[1], vel, BULLET_VELOCITY));
}


void SemiBoss01::move()
{
    if(!dying)
    {
        const int SHOT_YVEL = 1;

        if(position.x < XMIN)
        {
            position.x = XMIN +1;
            speed.vx = 0;
            speed.vy = SHOT_YVEL;
        }

        if(position.y < YMIN)
            speed.vy = SHOT_YVEL;
        else if(position.y > YMAX)
            speed.vy = -SHOT_YVEL;
    }

    Enemy::move();
}


void SemiBoss01::die()
{
    if(!dying)
    {
        delete mvs;
        mvs = nullptr;
        const ResourceManager *rc = ResourceManager::getInstance();
        graphic = rc->getResource(RC_XPLOSION, 2);
        addStrategy(new DeathStrategy(this, DEFAULT_XPLOSION_DELAY,
                                      DEFAULT_NOISE_DELAY));
    }
    Boss::die();
}
