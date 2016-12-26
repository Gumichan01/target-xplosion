
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
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "Boss02.hpp"
//#include "../Bullet.hpp"
#include "../BasicMissile.hpp"
//#include "../../game/Game.hpp"
#include "../../resources/ResourceManager.hpp"

//#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Log.hpp> // remove it when the tests are done

using namespace LX_Physics;

namespace
{

const int XLIM = 641;

const int DANGER_RAD = 285;
const int CORE_X = 320;
const int CORE_Y = 320;
const int CORE_RAD = 133;

// Sentinels
const int NB_SENTINELS = 8;
const int SENT_RAD = 32;

LX_Circle sentinel_hbox[NB_SENTINELS] = {LX_Circle(LX_Point(140,140), SENT_RAD),
                                         LX_Circle(LX_Point(320,68), SENT_RAD),
                                         LX_Circle(LX_Point(500,140), SENT_RAD),
                                         LX_Circle(LX_Point(572,320), SENT_RAD),
                                         LX_Circle(LX_Point(500,500), SENT_RAD),
                                         LX_Circle(LX_Point(320,552), SENT_RAD),
                                         LX_Circle(LX_Point(140,500), SENT_RAD),
                                         LX_Circle(LX_Point(68,320), SENT_RAD),
                                        };
/// Shot on target
// Shot wave duration
const uint32_t BOSS_DSHOT = 4000;
// Duration between two shot waves
const uint32_t BOSS_DSHOT_DELAY = 2000;
// Duration between each shot
const uint32_t BOSS_DBSHOT = 100;

/// Bullets
const uint32_t BOSS_BSHOT_DELAY = 500;


/// Reload
const int HP_RELOAD = 128;

}


Boss02::Boss02(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
               int x, int y, int w, int h, float vx, float vy)
    : Boss(hp, att, sh, image, audio, x, y, w, h, vx, vy),
      shield(true), shield_points(max_health_point),
      core_hbox(LX_Point(CORE_X,CORE_Y), CORE_RAD), asprite(nullptr),
      asprite_sh(nullptr), asprite_nosh(nullptr)
{
    addStrategy(new MoveStrategy(this));

    hitbox.radius = DANGER_RAD;
    hitbox.square_radius = DANGER_RAD * DANGER_RAD;
    moveCircleTo(core_hbox, position.x + core_hbox.center.x, position.y + core_hbox.center.y);

    for(int i = 0; i< NB_SENTINELS; i++)
    {
        moveCircleTo(sentinel_hbox[i], position.x + sentinel_hbox[i].center.x,
                     position.y + sentinel_hbox[i].center.y);
    }

    asprite = graphic;
    asprite_sh = ResourceManager::getInstance()->getResource(RC_ENEMY, 3);
    graphic = asprite_sh;
    /// @todo Sprite of the boss without sentinels

    LX_Log::log("BOSS");
    LX_Log::log("danger zone: (%d, %d) | %d", hitbox.center.x, hitbox.center.y, hitbox.radius);
    LX_Log::log("core zone: (%d, %d) | %d", core_hbox.center.x, core_hbox.center.y, core_hbox.radius);
}


void Boss02::shotOnTarget()
{
    LX_Log::log("SHOOT ON TARGET");
}

void Boss02::bullets()
{
    LX_Log::log("BULLETS");
}

void Boss02::mbullets()
{
    LX_Log::log("MEGA BULLETS");
}

void Boss02::reload()
{
    const unsigned int V = 512;

    if(shield_points > 0)
    {
        if(health_point + V > max_health_point)
            health_point = max_health_point;
        else
            health_point += V;
    }

    LX_Log::log("RELOADS");
    LX_Log::log("HP: %u", health_point);
    LX_Log::log("shield point: %u", shield_points);
}

void Boss02::fire()
{
    switch(id_strat)
    {
    case 2:
        bullets();
        break;

    case 3:
        bullets();
        mbullets();
        break;

    case 4:
        reload();
        break;

    default:
        shotOnTarget();
        break;
    }
}

void Boss02::strategy()
{
    const unsigned int HEALTH_80 = static_cast<float>(max_health_point) * 0.8f;
    const unsigned int HEALTH_55 = static_cast<float>(max_health_point) * 0.55f;
    const unsigned int HEALTH_25 = static_cast<float>(max_health_point) * 0.25f;

    if(id_strat == 0)
    {
        if(position.x < XLIM)
        {
            id_strat = 1;
            shield = false;
            graphic = asprite;
            addStrategy(new Boss02Shot(this));
        }
    }
    else if(id_strat == 1)
    {
        if(health_point < HEALTH_80)
        {
            id_strat = 2;
            addStrategy(new Boss02Shot80(this));
        }
    }
    else if(id_strat == 2)
    {
        if(health_point < HEALTH_55)
        {
            id_strat = 3;
            addStrategy(new Boss02Shot55(this));
        }
    }
    else if(id_strat == 3)
    {
        if(health_point < HEALTH_25)
        {
            if(shield_points > 0)
            {
                id_strat = 4;
                shield = true;
                graphic = asprite_sh;
                addStrategy(new Boss02Reload(this));
            }
        }
    }
    else if(id_strat == 4)
    {
        if(health_point == max_health_point || shield_points == 0)
        {
            id_strat = 1;
            shield = false;
            graphic = asprite;
            addStrategy(new Boss02Shot(this));
        }
    }

    strat->proceed();
}

void Boss02::move()
{
    moveCircle(core_hbox,speed);

    for(int i = 0; i< NB_SENTINELS; i++)
        moveCircle(sentinel_hbox[i],speed);

    Enemy::move();
}

void Boss02::collision(Missile *mi)
{
    const LX_AABB& box = *(mi->getHitbox());

    if(collisionCircleRect(hitbox, box))
    {
        if(shield)
        {
            int d = static_cast<int>(shield_points) - HP_RELOAD;
            shield_points = d <= 0 ? 0 : d;
            mi->die();
        }

        if(collisionCircleRect(core_hbox, box))
        {
            reaction(mi);
            mi->die();
        }
        else
        {
            for(int i = 0; i< NB_SENTINELS; i++)
            {
                if(collisionCircleRect(sentinel_hbox[i], box))
                {
                    mi->die();
                    break;
                }
            }
        }
    }
}

void Boss02::die()
{
    Enemy::die();
}

Boss02::~Boss02() {}


/* --------------------
    Boss02 strategies
   -------------------- */

Boss02Shot::Boss02Shot(Boss02 * nboss)
    : Strategy(nboss), BossStrategy(nboss), shot_t(0), wave_t(0), pause_t(0),
      shoot(true)
{
    shot_t = LX_Timer::getTicks();
    wave_t = LX_Timer::getTicks();
    pause_t = LX_Timer::getTicks();
}


void Boss02Shot::proceed()
{
    if(shoot)
    {
        if((LX_Timer::getTicks() - wave_t) < BOSS_DSHOT)
        {
            if((LX_Timer::getTicks() - shot_t) > BOSS_DBSHOT)
            {
                ((Boss02*)target)->shotOnTarget();
                shot_t = LX_Timer::getTicks();
            }
        }
        else
        {
            LX_Log::log("PAUSE");
            shoot = false;
            pause_t = LX_Timer::getTicks();
        }
    }
    else
    {
        if((LX_Timer::getTicks() - pause_t) > BOSS_DSHOT_DELAY)
        {
            shoot = true;
            wave_t = LX_Timer::getTicks();
            shot_t = LX_Timer::getTicks();
        }
    }
}

// Bullets
Boss02Bullet::Boss02Bullet(Boss02 * nboss)
    : Strategy(nboss), BossStrategy(nboss), shot_t(LX_Timer::getTicks()) {}

void Boss02Bullet::proceed()
{
    if((LX_Timer::getTicks() - shot_t) > BOSS_BSHOT_DELAY)
    {
        target->fire();
        shot_t = LX_Timer::getTicks();
    }
}

// Boss under 80% of its maximal health
Boss02Shot80::Boss02Shot80(Boss02 * nboss)
    : Strategy(nboss), BossStrategy(nboss), bsstrat(nboss), bbstrat(nboss) {}

void Boss02Shot80::proceed()
{
    bsstrat.proceed();
    bbstrat.proceed();
}


// Boss under 55% of its maximal health
Boss02Shot55::Boss02Shot55(Boss02 * nboss)
    : Strategy(nboss), BossStrategy(nboss), bsstrat(nboss), bbstrat(nboss) {}

void Boss02Shot55::proceed()
{
    bsstrat.proceed();
    bbstrat.proceed();
}


// Reload the life points of the boss
Boss02Reload::Boss02Reload(Boss02 * nboss)
    : Strategy(nboss), BossStrategy(nboss), t(LX_Timer::getTicks()) {}


void Boss02Reload::proceed()
{
    if((LX_Timer::getTicks() - t) > BOSS_DBSHOT)
    {
        target->fire();
        t = LX_Timer::getTicks();
    }
}
