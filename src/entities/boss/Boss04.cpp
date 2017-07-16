
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

#include "Boss04.hpp"
#include "../Player.hpp"
#include "../Bullet.hpp"
#include "../BasicMissile.hpp"
#include "../../pattern/BulletPattern.hpp"
#include "../../game/Scoring.hpp"
#include "../../game/engine/Hud.hpp"
#include "../../game/engine/Engine.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <typeinfo>
#include <cmath>

#define FL(x)  static_cast<float>(x)
#define UIL(x) static_cast<unsigned int>(x)

using namespace LX_Graphics;
using namespace LX_Physics;


namespace
{

const int BOSS04_SHID = 8;
const int BOSS04_NOSHID = 9;
const int BOSS04_XSHID = 6;
const int BOSS04_YBULLET_ID = 6;
const int BOSS04_RBULLET_ID = 8;
const int BOSS04_BBULLET_ID = 5;

const int BOSS04_XLIM = 641;
const int BOSS04_RAD = 285;
// Core hitbox
const int BOSS04_XCORE = 320;
const int BOSS04_YCORE = 320;
const int BOSS04_CRAD = 133;    // radius of the core hitbox

// Sentinels
const int BOSS04_SENTINELS = 8;
const int BOSS04_SRAD = 32;

// Bullets
const int BOSS04_BULLETS_DIM = 24;
const int BOSS04_BULLETS2_DIM = 16;

LX_Circle sentinel_hbox[BOSS04_SENTINELS] =
{
    LX_Circle(LX_Point(140,140), BOSS04_SRAD),
    LX_Circle(LX_Point(320,68), BOSS04_SRAD),
    LX_Circle(LX_Point(500,140), BOSS04_SRAD),
    LX_Circle(LX_Point(572,320), BOSS04_SRAD),
    LX_Circle(LX_Point(500,500), BOSS04_SRAD),
    LX_Circle(LX_Point(320,552), BOSS04_SRAD),
    LX_Circle(LX_Point(140,500), BOSS04_SRAD),
    LX_Circle(LX_Point(68,320), BOSS04_SRAD),
};

LX_Point sentinel_src[BOSS04_SENTINELS] =
{
    LX_Point(140,140), LX_Point(320,68),
    LX_Point(500,140), LX_Point(572,320),
    LX_Point(500,500), LX_Point(320,552),
    LX_Point(140,500), LX_Point(68,320),
};

LX_AABB rbullets[BOSS04_SENTINELS] =
{
    {284,237,BOSS04_BULLETS2_DIM,BOSS04_BULLETS2_DIM},
    {284,403,BOSS04_BULLETS2_DIM,BOSS04_BULLETS2_DIM},
    {342,237,BOSS04_BULLETS2_DIM,BOSS04_BULLETS2_DIM},
    {342,403,BOSS04_BULLETS2_DIM,BOSS04_BULLETS2_DIM},
    {237,284,BOSS04_BULLETS2_DIM,BOSS04_BULLETS2_DIM},
    {237,356,BOSS04_BULLETS2_DIM,BOSS04_BULLETS2_DIM},
    {389,284,BOSS04_BULLETS2_DIM,BOSS04_BULLETS2_DIM},
    {389,356,BOSS04_BULLETS2_DIM,BOSS04_BULLETS2_DIM}
};

FloatPosition bfpos[BOSS04_SENTINELS];

/// Shot on target
// Shot wave duration
const uint32_t BOSS04_DSHOT = 2000;
// Duration between two shot waves
const uint32_t BOSS04_DSHOT_DELAY = 2000;
// Duration between each shot
const uint32_t BOSS04_DBSHOT = 100;
// Bullet velocity
const int BOSS04_DSHOT_BVEL = -16;

/// Bullets
const uint32_t BOSS04_BSHOT_DELAY = 2000;
const float BOSS04_BSHOT_BVEL = -8.0f;
const int BOSS04_MBSHOT_BVEL = 10;

const int BOSS04_MBSHOT_OFFX = 312;
const int BOSS04_MBSHOT_OFFY = 311;

/// Reload

// Duration between each heal
const uint32_t BOSS04_DHEAL = 100;
const uint32_t BOSS04_DAMAGES_RATIO = 2;

/// Remove the sentinels
const uint32_t BOSS03_XSH_DELAY = 750;

/// Unleash
float alpha = 0.0f;
const float step = FL(BulletPattern::PI)/24.0f;
const float BOSS04_RF = 100.0f;
const int BOSS04_USHOT_BVEL = -4;

const uint32_t BOSS04_USHOT_NDELAY = 200;
const uint32_t BOSS04_USHOT_HDELAY = 100;
const uint32_t BOSS04_USHOT_XDELAY = 50;


Boss04 * getBoss04Cast(Enemy * target)
{
    Boss04 * boss = dynamic_cast<Boss04*>(target);

    if(boss == nullptr)
        throw std::bad_cast();

    return boss;
}

}


Boss04::Boss04(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy),
      HEALTH_80(UIL(FL(max_health_point) * 0.8f)),
      HEALTH_55(UIL(FL(max_health_point) * 0.55f)),
      HEALTH_25(UIL(FL(max_health_point) * 0.25f)), shield(true),
      shield_points(max_health_point),
      core_hbox(LX_Point(BOSS04_XCORE,BOSS04_YCORE), BOSS04_CRAD), asprite(nullptr),
      asprite_sh(nullptr), asprite_x(nullptr), asprite_nosh(nullptr)
{
    addStrategy(new MoveStrategy(this));

    // reduce the hitbox + set the core hitbox
    hitbox.radius = BOSS04_RAD;
    hitbox.square_radius = BOSS04_RAD * BOSS04_RAD;
    moveCircleTo(core_hbox, position.x + core_hbox.center.x,
                 position.y + core_hbox.center.y);

    // set the hitbox of each sentinel
    for(int i = 0; i< BOSS04_SENTINELS; i++)
    {
        moveCircleTo(sentinel_hbox[i], position.x + sentinel_hbox[i].center.x,
                     position.y + sentinel_hbox[i].center.y);
        bfpos[i] = sentinel_hbox[i];
    }

    core_fpos = core_hbox;

    // graphics assets of the boss
    asprite = graphic;
    asprite_sh = ResourceManager::getInstance()->getResource(RC_ENEMY, BOSS04_SHID);
    graphic = asprite_sh;
    asprite_nosh = ResourceManager::getInstance()->getResource(RC_ENEMY, BOSS04_NOSHID);
    asprite_x = ResourceManager::getInstance()->getResource(RC_XPLOSION, BOSS04_XSHID);
}


void Boss04::shotOnTarget()
{
    LX_Vector2D bvel[BOSS04_SENTINELS];
    LX_AABB brect[BOSS04_SENTINELS];
    Engine *g = Engine::getInstance();
    LX_Sprite *bsp = ResourceManager::getInstance()->getResource(RC_MISSILE, BOSS04_YBULLET_ID);

    for(int i = 0; i < BOSS04_SENTINELS; i++)
    {
        BulletPattern::shotOnPlayer(FL(sentinel_src[i].x), FL(sentinel_src[i].y),
                                    BOSS04_DSHOT_BVEL, bvel[i]);

        brect[i] = {sentinel_src[i].x, sentinel_src[i].y,
                    BOSS04_BULLETS_DIM, BOSS04_BULLETS_DIM
                   };

        g->acceptEnemyMissile(new Bullet(attack_val, bsp, brect[i], bvel[i]));
    }
}

void Boss04::bullets()
{
    LX_Vector2D v(BOSS04_BSHOT_BVEL, 0.0f);
    Engine *g = Engine::getInstance();
    LX_Sprite *bsp = ResourceManager::getInstance()->getResource(RC_MISSILE, BOSS04_RBULLET_ID);

    for(int i = 0; i < BOSS04_SENTINELS; i++)
        g->acceptEnemyMissile(new Bullet(attack_val, bsp, rbullets[i], v));
}

void Boss04::mbullets()
{
    LX_Vector2D v;
    Engine *e = Engine::getInstance();
    LX_AABB mbrect = {position.x + BOSS04_MBSHOT_OFFX,
                      position.y + BOSS04_MBSHOT_OFFY,
                      BOSS04_BULLETS2_DIM, BOSS04_BULLETS2_DIM
                     };

    LX_Sprite *bsp = ResourceManager::getInstance()->getResource(RC_MISSILE, BOSS04_BBULLET_ID);
    e->acceptEnemyMissile(new MegaBullet(attack_val, bsp, mbrect, v, BOSS04_MBSHOT_BVEL));
}

void Boss04::reload()
{
    const unsigned int V = 512;

    if(shield_points > 0)
    {
        if(health_point + V > max_health_point)
            health_point = max_health_point;
        else
            health_point += V;

        hud->update();
    }
}

void Boss04::unleash()
{
    LX_Vector2D v;
    Engine *e = Engine::getInstance();

    const LX_Point p(position.x + BOSS04_MBSHOT_OFFX, position.y + BOSS04_MBSHOT_OFFY);
    LX_AABB mbrect = {p.x, p.y, BOSS04_BULLETS2_DIM, BOSS04_BULLETS2_DIM};
    LX_Sprite *bsp = ResourceManager::getInstance()->getResource(RC_MISSILE, BOSS04_BBULLET_ID);

    BulletPattern::shotOnTarget(p.x, p.y, FL(p.x) + cosf(alpha) * BOSS04_RF,
                                FL(p.y) - sinf(alpha) * BOSS04_RF,
                                BOSS04_USHOT_BVEL, v);

    if(alpha > FL(BulletPattern::PI) * 2.0f)
    {
        alpha = 0.0f;
        bullets();
    }

    alpha += step;
    e->acceptEnemyMissile(new MegaBullet(attack_val, bsp, mbrect, v, BOSS04_MBSHOT_BVEL));
}


void Boss04::stratPos()
{
    if(position.x < BOSS04_XLIM)
    {
        id_strat = 1;
        shield = false;
        graphic = asprite;
        Engine::getInstance()->screenCancel();
        addStrategy(new Boss04Shot(this));

        for(int i = 0; i < BOSS04_SENTINELS; i++)
        {
            movePointTo(sentinel_src[i], position.x + sentinel_src[i].x,
                        position.y + sentinel_src[i].y);

            moveRectTo(rbullets[i], position.x + rbullets[i].x,
                       position.y + rbullets[i].y);
        }
    }
}

void Boss04::stratReload()
{
    if(health_point == max_health_point || shield_points == 0)
    {
        shield = false;

        if(shield_points == 0)
        {
            id_strat = 5;
            graphic = asprite_x;
            addStrategy(new Boss04Break(this));
        }
        else if(health_point == max_health_point)
        {
            id_strat = 1;
            graphic = asprite;
            addStrategy(new Boss04Shot(this));
        }

        Engine::getInstance()->screenCancel();
    }
}

void Boss04::stratX()
{
    id_strat = 6;
    graphic = asprite_nosh;
    ShotStrategy * sht = new ShotStrategy(this);
    sht->setShotDelay(BOSS04_USHOT_NDELAY);

    if(health_point < HEALTH_55)
        sht->setShotDelay(BOSS04_USHOT_HDELAY);
    else if(health_point < HEALTH_25)
        sht->setShotDelay(BOSS04_USHOT_XDELAY);

    addStrategy(sht);
}

void Boss04::stratUnleash()
{
    static unsigned int prev_health = 0;

    if(health_point < HEALTH_80 && prev_health >= HEALTH_80)
    {
        Engine::getInstance()->screenCancel();
        ShotStrategy * sht = new ShotStrategy(this);
        sht->setShotDelay(BOSS04_USHOT_NDELAY);
        addStrategy(sht);
    }

    if(health_point < HEALTH_55 && prev_health >= HEALTH_55)
    {
        Engine::getInstance()->screenCancel();
        ShotStrategy * sht = new ShotStrategy(this);
        sht->setShotDelay(BOSS04_USHOT_HDELAY);
        addStrategy(sht);
    }

    if(health_point < HEALTH_25 && prev_health >= HEALTH_25)
    {
        Engine::getInstance()->screenCancel();
        ShotStrategy * sht = new ShotStrategy(this);
        sht->setShotDelay(BOSS04_USHOT_XDELAY);
        addStrategy(sht);
    }

    prev_health = health_point;
}


void Boss04::fire()
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

    case 6:
        unleash();
        break;

    default:
        shotOnTarget();
        break;
    }
}

void Boss04::strategy()
{
    Engine *g = Engine::getInstance();

    if(id_strat == 0)
        stratPos();
    else if(id_strat == 1)  // Shot on target
    {
        if(health_point < HEALTH_80)
        {
            id_strat = 2;
            g->screenCancel();
            addStrategy(new Boss04Shot2(this));
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
                addStrategy(new Boss04Reload(this));
            }
        }
    }
    else if(id_strat == 4)  // Shield
        stratReload();
    else if(id_strat == 6)  // Shield destroyed
        stratUnleash();

    Boss::strategy();
}

void Boss04::move()
{
    core_fpos += speed;
    core_fpos.toPixelUnit(core_hbox);

    for(int i = 0; i< BOSS04_SENTINELS; i++)
    {
        bfpos[i] += speed;
        bfpos[i].toPixelUnit(sentinel_hbox[i]);
    }

    Enemy::move();
}

void Boss04::collision(Missile *mi)
{
    const LX_AABB& box = *(mi->getHitbox());

    if(id_strat == 5)
        return;

    if(shield_points > 0)
    {
        if(collisionCircleRect(hitbox, box))
        {
            if(shield)
            {
                int hit = static_cast<int>(mi->hit() / BOSS04_DAMAGES_RATIO);
                int d = static_cast<int>(shield_points) - hit;
                shield_points = static_cast<uint32_t>(d < 0 ? 0 : d);
                mi->die();
            }

            if(collisionCircleRect(core_hbox, box))
            {
                if(destroyable) reaction(mi);
                mi->die();
            }
            else
            {
                for(int i = 0; i< BOSS04_SENTINELS; i++)
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
            if(destroyable) reaction(mi);
            mi->die();
        }
    }
}

void Boss04::collision(Player *play)
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

void Boss04::reaction(Missile *target)
{
    if(shield)
    {
        Score *sc = Engine::getInstance()->getScore();
        receiveDamages(target->hit()/ BOSS04_DAMAGES_RATIO);
        sc->notify(Scoring::DAMAGE_SCORE);
    }
    else
        Boss::reaction(target);
}

void Boss04::die()
{
    /// @todo (#9#) v0.6.x: destruction of the boss #X
    Enemy::die();
}

Boss04::~Boss04() {}


/* --------------------
    Boss04 strategies
   -------------------- */

Boss04Shot::Boss04Shot(Boss04 * nboss)
    : Strategy(nboss), BossStrategy(nboss), shot_t(0), wave_t(0), pause_t(0),
      shoot(true)
{
    shot_t = LX_Timer::getTicks();
    wave_t = LX_Timer::getTicks();
    pause_t = LX_Timer::getTicks();
}


void Boss04Shot::proceed()
{
    if(shoot)
    {
        if((LX_Timer::getTicks() - wave_t) < BOSS04_DSHOT)
        {
            if((LX_Timer::getTicks() - shot_t) > BOSS04_DBSHOT)
            {
                getBoss04Cast(target)->shotOnTarget();
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
        if((LX_Timer::getTicks() - pause_t) > BOSS04_DSHOT_DELAY)
        {
            shoot = true;
            wave_t = LX_Timer::getTicks();
            shot_t = LX_Timer::getTicks();
        }
    }
}

// Bullets
Boss04Shot2::Boss04Shot2(Boss04 * nboss)
    : Strategy(nboss), BossStrategy(nboss), bsstrat(nboss), bbstrat(nboss)

{
    bbstrat.setShotDelay(BOSS04_BSHOT_DELAY);
}

void Boss04Shot2::proceed()
{
    bsstrat.proceed();
    bbstrat.proceed();
}


Boss04Break::Boss04Break(Boss04 * nboss)
    : Strategy(nboss), BossStrategy(nboss),
      xtime(LX_Timer::getTicks()) {}

void Boss04Break::proceed()
{
    if((LX_Timer::getTicks() - xtime) > BOSS03_XSH_DELAY)
    {
        getBoss04Cast(target)->stratX();
    }
}


// Reload the life points of the boss
Boss04Reload::Boss04Reload(Boss04 * nboss)
    : Strategy(nboss), BossStrategy(nboss), t(LX_Timer::getTicks()) {}


void Boss04Reload::proceed()
{
    if((LX_Timer::getTicks() - t) > BOSS04_DHEAL)
    {
        target->fire();
        t = LX_Timer::getTicks();
    }
}
