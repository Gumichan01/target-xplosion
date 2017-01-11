
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

#include "Boss02.hpp"
#include "../Player.hpp"
#include "../Bullet.hpp"
#include "../BasicMissile.hpp"
#include "../../pattern/BulletPattern.hpp"
#include "../../game/Game.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <cmath>

#define FL(x) static_cast<float>(x)

using namespace LX_Graphics;
using namespace LX_Physics;

/// @todo clean the code

namespace
{

const int BOSS_SHID = 3;
const int BOSS_NOSHID = 4;
const int BOSS_YBULLET_ID = 7;
const int BOSS_RBULLET_ID = 9;
const int BOSS_BBULLET_ID = 5;

const int XLIM = 641;
const int DANGER_RAD = 285;
const int CORE_X = 320;
const int CORE_Y = 320;
const int CORE_RAD = 133;

// Sentinels
const int NB_SENTINELS = 8;
const int SENT_RAD = 32;

// Bullets
const int BOSS_BULLETS_DIM = 24;
const int BOSS_BULLETS2_DIM = 16;

LX_Circle sentinel_hbox[NB_SENTINELS] = {LX_Circle(LX_Point(140,140), SENT_RAD),
                                         LX_Circle(LX_Point(320,68), SENT_RAD),
                                         LX_Circle(LX_Point(500,140), SENT_RAD),
                                         LX_Circle(LX_Point(572,320), SENT_RAD),
                                         LX_Circle(LX_Point(500,500), SENT_RAD),
                                         LX_Circle(LX_Point(320,552), SENT_RAD),
                                         LX_Circle(LX_Point(140,500), SENT_RAD),
                                         LX_Circle(LX_Point(68,320), SENT_RAD),
                                        };

LX_Point sentinel_src[NB_SENTINELS] = {LX_Point(140,140), LX_Point(320,68),
                                       LX_Point(500,140), LX_Point(572,320),
                                       LX_Point(500,500), LX_Point(320,552),
                                       LX_Point(140,500), LX_Point(68,320),
                                      };

LX_AABB rbullets[NB_SENTINELS] =
{
    {284,237,BOSS_BULLETS2_DIM,BOSS_BULLETS2_DIM},
    {284,403,BOSS_BULLETS2_DIM,BOSS_BULLETS2_DIM},
    {342,237,BOSS_BULLETS2_DIM,BOSS_BULLETS2_DIM},
    {342,403,BOSS_BULLETS2_DIM,BOSS_BULLETS2_DIM},
    {237,284,BOSS_BULLETS2_DIM,BOSS_BULLETS2_DIM},
    {237,356,BOSS_BULLETS2_DIM,BOSS_BULLETS2_DIM},
    {389,284,BOSS_BULLETS2_DIM,BOSS_BULLETS2_DIM},
    {389,356,BOSS_BULLETS2_DIM,BOSS_BULLETS2_DIM}
};

/// Shot on target
// Shot wave duration
const uint32_t BOSS_DSHOT = 2000;
// Duration between two shot waves
const uint32_t BOSS_DSHOT_DELAY = 2000;
// Duration between each shot
const uint32_t BOSS_DBSHOT = 100;
// Bullet velocity
const int BOSS_DSHOT_BVEL = -16;

/// Bullets
const uint32_t BOSS_BSHOT_DELAY = 2000;
const float BOSS_BSHOT_BVEL = -8.0f;
const int BOSS_MBSHOT_BVEL = 10;

const int BOSS_MBSHOT_OFFX = 312;
const int BOSS_MBSHOT_OFFY = 311;

/// Reload
const int SH_DAMAGE = 80;

/// Unleash
float alpha = 0.0f;
const float step = FL(BulletPattern::PI)/24.0f;
const float BOSS_RF = 100.0f;
const int BOSS_USHOT_BVEL = -4;

const uint32_t BOSS_USHOT_NDELAY = 200;
const uint32_t BOSS_USHOT_HDELAY = 100;
const uint32_t BOSS_USHOT_XDELAY = 50;

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

    // reduce the hitbox + set the core hitbox
    hitbox.radius = DANGER_RAD;
    hitbox.square_radius = DANGER_RAD * DANGER_RAD;
    moveCircleTo(core_hbox, position.x + core_hbox.center.x,
                 position.y + core_hbox.center.y);

    // set the hitbox of each sentinel
    for(int i = 0; i< NB_SENTINELS; i++)
    {
        moveCircleTo(sentinel_hbox[i], position.x + sentinel_hbox[i].center.x,
                     position.y + sentinel_hbox[i].center.y);
    }

    // graphics assets of the boss
    asprite = graphic;
    asprite_sh = ResourceManager::getInstance()->getResource(RC_ENEMY, BOSS_SHID);
    graphic = asprite_sh;
    asprite_nosh = ResourceManager::getInstance()->getResource(RC_ENEMY, BOSS_NOSHID);
}


void Boss02::shotOnTarget()
{
    LX_Vector2D bvel[NB_SENTINELS];
    LX_AABB brect[NB_SENTINELS];
    Game *g = Game::getInstance();
    LX_Sprite *bsp = ResourceManager::getInstance()->getResource(RC_MISSILE, BOSS_YBULLET_ID);

    for(int i = 0; i < NB_SENTINELS; i++)
    {
        BulletPattern::shotOnPlayer(FL(sentinel_src[i].x), FL(sentinel_src[i].y),
                                    BOSS_DSHOT_BVEL, bvel[i]);

        brect[i] = {sentinel_src[i].x, sentinel_src[i].y,
                    BOSS_BULLETS_DIM, BOSS_BULLETS_DIM
                   };

        g->acceptEnemyMissile(new Bullet(attack_val, bsp, nullptr, brect[i],
                                         bvel[i]));
    }
}

void Boss02::bullets()
{
    LX_Vector2D v(BOSS_BSHOT_BVEL, 0.0f);
    Game *g = Game::getInstance();
    LX_Sprite *bsp = ResourceManager::getInstance()->getResource(RC_MISSILE, BOSS_RBULLET_ID);

    for(int i = 0; i < NB_SENTINELS; i++)
        g->acceptEnemyMissile(new Bullet(attack_val, bsp, nullptr, rbullets[i], v));
}

void Boss02::mbullets()
{
    LX_Vector2D v;
    LX_AABB mbrect = {position.x + BOSS_MBSHOT_OFFX,
                      position.y + BOSS_MBSHOT_OFFY,
                      BOSS_BULLETS2_DIM, BOSS_BULLETS2_DIM
                     };

    LX_Sprite *bsp = ResourceManager::getInstance()->getResource(RC_MISSILE, BOSS_BBULLET_ID);
    Game::getInstance()->acceptEnemyMissile(new MegaBullet(attack_val, bsp,
                                            nullptr, mbrect, v, BOSS_MBSHOT_BVEL));
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
}

void Boss02::unleash()
{
    const LX_Point p(position.x + BOSS_MBSHOT_OFFX, position.y + BOSS_MBSHOT_OFFY);
    LX_Vector2D v;
    LX_AABB mbrect = {p.x, p.y, BOSS_BULLETS2_DIM, BOSS_BULLETS2_DIM};
    LX_Sprite *bsp = ResourceManager::getInstance()->getResource(RC_MISSILE, BOSS_BBULLET_ID);

    BulletPattern::shotOnTarget(p.x, p.y, FL(p.x) + cosf(alpha) * BOSS_RF,
                                FL(p.y) - sinf(alpha) * BOSS_RF,
                                BOSS_USHOT_BVEL, v);

    if(alpha > FL(BulletPattern::PI) * 2.0f)
    {
        alpha = 0.0f;
        bullets();
    }

    alpha += step;
    Game::getInstance()->acceptEnemyMissile(new MegaBullet(attack_val, bsp,
                                            nullptr, mbrect, v, BOSS_MBSHOT_BVEL));
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

    case 5:
        unleash();
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
    Game *g = Game::getInstance();

    if(id_strat == 0)
    {
        if(position.x < XLIM)
        {
            id_strat = 1;
            shield = false;
            graphic = asprite;
            g->screenCancel();
            addStrategy(new Boss02Shot(this));

            for(int i = 0; i < NB_SENTINELS; i++)
            {
                movePointTo(sentinel_src[i], position.x + sentinel_src[i].x,
                            position.y + sentinel_src[i].y);

                moveRectTo(rbullets[i], position.x + rbullets[i].x,
                           position.y + rbullets[i].y);
            }
        }
    }
    else if(id_strat == 1)  // Shot on target
    {
        if(health_point < HEALTH_80)
        {
            id_strat = 2;
            g->screenCancel();
            addStrategy(new Boss02Shot2(this));
        }
    }
    else if(id_strat == 2)  // Shot on target + Bullets
    {
        if(health_point < HEALTH_55)
        {
            id_strat = 3;
            g->screenCancel();
        }
    }
    else if(id_strat == 3)  // Shot on target + Bullets + Megabullets
    {
        if(health_point < HEALTH_25)
        {
            if(shield_points > 0)
            {
                id_strat = 4;
                shield = true;
                graphic = asprite_sh;
                g->screenCancel();
                addStrategy(new Boss02Reload(this));
            }
        }
    }
    else if(id_strat == 4)  // Shield
    {
        if(health_point == max_health_point || shield_points == 0)
        {
            shield = false;

            if(shield_points == 0)
            {
                id_strat = 5;
                graphic = asprite_nosh;
                ShotStrategy * sht = new ShotStrategy(this);
                sht->setShotDelay(BOSS_USHOT_NDELAY);

                if(health_point < HEALTH_55)
                    sht->setShotDelay(BOSS_USHOT_HDELAY);
                else if(health_point < HEALTH_25)
                    sht->setShotDelay(BOSS_USHOT_XDELAY);


                addStrategy(sht);
            }
            else if(health_point == max_health_point)
            {
                id_strat = 1;
                graphic = asprite;
                addStrategy(new Boss02Shot(this));
            }
            g->screenCancel();
        }
    }

    else if(id_strat == 5)  // Shield destroyed
    {
        static unsigned int prev_health = 0;

        if(health_point < HEALTH_80 && prev_health >= HEALTH_80)
        {
            g->screenCancel();
            ShotStrategy * sht = new ShotStrategy(this);
            sht->setShotDelay(BOSS_USHOT_NDELAY);
            addStrategy(sht);
        }

        if(health_point < HEALTH_55 && prev_health >= HEALTH_55)
        {
            g->screenCancel();
            ShotStrategy * sht = new ShotStrategy(this);
            sht->setShotDelay(BOSS_USHOT_HDELAY);
            addStrategy(sht);
        }

        if(health_point < HEALTH_25 && prev_health >= HEALTH_25)
        {
            g->screenCancel();
            ShotStrategy * sht = new ShotStrategy(this);
            sht->setShotDelay(BOSS_USHOT_XDELAY);
            addStrategy(sht);
        }

        prev_health = health_point;
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

    if(shield_points > 0)
    {
        if(collisionCircleRect(hitbox, box))
        {
            if(shield)
            {
                int d = static_cast<int>(shield_points) - SH_DAMAGE;
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
    else
    {
        if(collisionCircleRect(core_hbox, box))
        {
            reaction(mi);
            mi->die();
        }
    }
}

void Boss02::collision(Player *play)
{
    if(shield_points > 0)
        Enemy::collision(play);
    else
    {
        if(collisionCircle(core_hbox, *play->getHitbox()))
        {
            play->die();
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
                dynamic_cast<Boss02*>(target)->shotOnTarget();
                shot_t = LX_Timer::getTicks();
            }
        }
        else
        {
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
Boss02Shot2::Boss02Shot2(Boss02 * nboss)
    : Strategy(nboss), BossStrategy(nboss), bsstrat(nboss), bbstrat(nboss)

{
    bbstrat.setShotDelay(BOSS_BSHOT_DELAY);
}

void Boss02Shot2::proceed()
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