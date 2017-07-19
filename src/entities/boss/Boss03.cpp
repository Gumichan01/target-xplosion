
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


#include "Boss03.hpp"

#include "../Player.hpp"
#include "../Bullet.hpp"
#include "../../pattern/Strategy.hpp"
#include "../../pattern/BulletPattern.hpp"
#include "../../game/engine/Engine.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Polygon.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Log.hpp>

#include <array>

namespace
{
unsigned int BOSS03_HEAD_ID = 10;

const int BOSS03_BBULLET_ID = 4;
const int BOSS03_RBULLET_ID = 8;
const int BOSS03_PBULLET_ID = 9;


/* Body */

const int BOSS03_BODY_X = 512;
const uint32_t BOSS03_BODY_RAY1_DELAY = 50;
const uint32_t BOSS03_BODY_RAY2_DELAY = 1000;
const int BOSS03_BODY_BULLET1_W = 48;
const int BOSS03_BODY_BULLET1_H = 24;
const int BOSS03_BODY_CBULLET_DIM = 24;
const int BOSS03_BODY_CIRCLE1_XOFF = 278;
const int BOSS03_BODY_CIRCLE1_YOFF = 470;
const int BOSS03_BODY_CIRCLE2_XOFF = 278;
const int BOSS03_BODY_CIRCLE2_YOFF = 158;
const int BOSS03_BODY_CIRCLE_VEL = 6;

LX_Physics::LX_Vector2D boss03_ray_v(-8.0f, 0.0f);

const uint32_t BOSS03_BODY_ROW1_DELAY = 100;
const uint32_t BOSS03_BODY_ROW2_DELAY = 1000;
const int BOSS03_BODY_ROW_DIM = 16;

const uint32_t BOSS03_BODY_WAVE_DELAY = 1000;
const uint32_t BOSS03_BODY_CIRCLE_DELAY = 1000;
const int BOSS03_BODY_CIRCLE_DIM = BOSS03_BODY_ROW_DIM;

/* Head */

// Position of the HEAD
int BOSS03_HEAD_XOFF = 318;
int BOSS03_HEAD_YOFF = 158;
int BOSS03_HEAD_W = 461;
int BOSS03_HEAD_H = 336;

}

using namespace LX_Physics;

/** Boss03 */

Boss03::Boss03(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy), index(0)
{
    boss_parts[0] = new Boss03Body(hp, att, sh, image, x, y, w, h, vx, vy);

    /// @todo Boss03 — constructor — head of the boss
    //LX_Graphics:LX_Sprite hsprite = ResourceManager::getInstance()->getResource(RC_ENEMY, BOSS03_HEAD_ID);
    /*boss_parts[1] = new Boss03Head(hp/2, att, sh, hsprite, x + BOSS03_HEAD_XOFF,
                                   y + BOSS03_HEAD_YOFF, BOSS03_HEAD_W, BOSS03_HEAD_H, vx, vy);*/

    // We don't care about were it is.
    // The only thing that matters is where are the parts
    fpos = FloatPosition(0.0f,0.0f);
    position = {0,0,0,0};
    speed *= 0.0f;
    image = nullptr;
}


void Boss03::draw()
{
    for(int i = 0; i < BOSS03_PARTS; ++i)
        boss_parts[i]->draw();
}


void Boss03::strategy()
{
    boss_parts[index]->strategy();
}


void Boss03::move()
{
    for(int i = 0; i < BOSS03_PARTS; ++i)
        boss_parts[i]->move();
}

void Boss03::collision(Missile *mi)
{
    boss_parts[index]->collision(mi);
}

void Boss03::collision(Player *play)
{
    boss_parts[index]->collision(play);
}

void Boss03::die()
{
    /// @todo Boss03 — die()
    still_alive = false;
    //Enemy::die();
}


int Boss03::getX() const
{
    return boss_parts[index]->getX();
}

int Boss03::getY() const
{
    return boss_parts[index]->getY();
}

int Boss03::getWidth() const
{
    return boss_parts[index]->getWidth();
}

int Boss03::getHeight() const
{
    return boss_parts[index]->getHeight();
}


Boss03::~Boss03()
{
    for(int i = 0; i < BOSS03_PARTS; ++i)
        delete boss_parts[i];
}


/** Boss03 Body */

Boss03Body::Boss03Body(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                       float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy), ray_id(0)
{
    addStrategy(new MoveStrategy(this));

    std::vector<LX_Physics::LX_Point> hpoints {LX_Point(13,326), LX_Point(191,166),
            LX_Point(256,166), LX_Point(256,16),LX_Point(312,168), LX_Point(341,168),
            LX_Point(341,64), LX_Point(488,326), LX_Point(341,592), LX_Point(341,480),
            LX_Point(312,478), LX_Point(256,628), LX_Point(256,486), LX_Point(191,486),
                                              };

    poly = new LX_Polygon();

    std::for_each(hpoints.begin(), hpoints.end(), [x,y](LX_Point& p)
    {
        p.x += x;
        p.y += y;
    });

    poly->addPoints(hpoints.begin(), hpoints.end());
}


/// strat01 — fire!

void Boss03Body::rayShot()
{
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_BBULLET_ID);

    LX_AABB rpos[5] =
    {
        {position.x + 70, position.y + 182, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {position.x + 12, position.y + 239, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {position.x - 32, position.y + 314, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {position.x + 12, position.y + 390, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {position.x + 70, position.y + 448, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
    };

    switch(ray_id)
    {
    case 0:
        g->acceptEnemyMissile(new Bullet(attack_val, sp, rpos[2], boss03_ray_v));
        break;
    case 1:
        g->acceptEnemyMissile(new Bullet(attack_val, sp, rpos[1], boss03_ray_v));
        g->acceptEnemyMissile(new Bullet(attack_val, sp, rpos[3], boss03_ray_v));
        break;
    default:
        g->acceptEnemyMissile(new Bullet(attack_val, sp, rpos[0], boss03_ray_v));
        g->acceptEnemyMissile(new Bullet(attack_val, sp, rpos[4], boss03_ray_v));
        break;
    }
}

void Boss03Body::circleShot()
{
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_RBULLET_ID);

    LX_AABB cpos[2] =
    {
        {
            position.x + BOSS03_BODY_CIRCLE1_XOFF,
            position.y + BOSS03_BODY_CIRCLE1_YOFF,
            BOSS03_BODY_CBULLET_DIM, BOSS03_BODY_CBULLET_DIM
        },
        {
            position.x + BOSS03_BODY_CIRCLE2_XOFF,
            position.y + BOSS03_BODY_CIRCLE2_YOFF,
            BOSS03_BODY_CBULLET_DIM, BOSS03_BODY_CBULLET_DIM
        }
    };

    std::array<LX_Vector2D, CIRCLE_BULLETS> varr;
    BulletPattern::circlePattern(cpos[0].x, cpos[0].y,BOSS03_BODY_CIRCLE_VEL, varr);

    for(LX_Vector2D& v : varr)
    {
        g->acceptEnemyMissile(new Bullet(attack_val, sp, cpos[0], v));
        g->acceptEnemyMissile(new Bullet(attack_val, sp, cpos[1], v));
    }
}

/// strat two - fire!

void Boss03Body::rowShot()
{
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp1 = rc->getResource(RC_MISSILE, BOSS03_BBULLET_ID);
    LX_Graphics::LX_Sprite *sp2 = rc->getResource(RC_MISSILE, BOSS03_RBULLET_ID);

    LX_AABB rpos[2] =
    {
        {position.x + 70, position.y + 182, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {position.x + 70, position.y + 448, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
    };

    LX_AABB cpos[2] =
    {
        {
            position.x + BOSS03_BODY_CIRCLE1_XOFF,
            position.y + BOSS03_BODY_CIRCLE1_YOFF,
            BOSS03_BODY_CBULLET_DIM, BOSS03_BODY_CBULLET_DIM
        },
        {
            position.x + BOSS03_BODY_CIRCLE2_XOFF,
            position.y + BOSS03_BODY_CIRCLE2_YOFF,
            BOSS03_BODY_CBULLET_DIM, BOSS03_BODY_CBULLET_DIM
        }
    };

    LX_Vector2D v;
    std::array<LX_Vector2D, CIRCLE_BULLETS*2> varr;
    BulletPattern::circlePattern(cpos[0].x, cpos[0].y,BOSS03_BODY_CIRCLE_VEL, varr);

    for(size_t i = 0; i < varr.size()/2 + 1; ++i)
    {
        v = -varr[i];
        g->acceptEnemyMissile(new Bullet(attack_val, sp2, cpos[0], varr[i]));
        g->acceptEnemyMissile(new Bullet(attack_val, sp2, cpos[1], v));
    }

    g->acceptEnemyMissile(new Bullet(attack_val, sp1, rpos[0], boss03_ray_v));
    g->acceptEnemyMissile(new Bullet(attack_val, sp1, rpos[1], boss03_ray_v));
}

void Boss03Body::dShot()
{
    Engine *g = Engine::getInstance();
    const ResourceManager *rc  = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);

    LX_AABB pos[2] =
    {
        {position.x + 48, position.y + 239, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x + 48, position.y + 390, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
    };

    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varr1;
    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varr2;
    BulletPattern::waveOnPlayer(pos[0].x, pos[0].y, -vector_norm(boss03_ray_v), varr1);
    BulletPattern::waveOnPlayer(pos[1].x, pos[1].y, -vector_norm(boss03_ray_v), varr2);

    for(size_t i = 0; i < BulletPattern::WAVE_SZ; ++i)
    {
        g->acceptEnemyMissile(new Bullet(attack_val, sp, pos[0], varr1[i]));
        g->acceptEnemyMissile(new Bullet(attack_val, sp, pos[1], varr2[i]));
    }
}


// strat 03 - fire!

void Boss03Body::finalWave()
{
    const int N = 5;
    Engine *g = Engine::getInstance();
    const ResourceManager *rc  = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);

    LX_AABB pos[N] =
    {
        {position.x + 48, position.y + 182, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x + 44, position.y + 239, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x, position.y + 314, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x + 44, position.y + 390, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x + 48, position.y + 448, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
    };

    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varr[N];

    for(size_t i = 0; i < N; ++i)
    {
        BulletPattern::waveOnPlayer(pos[i].x, pos[i].y, -vector_norm(boss03_ray_v), varr[i]);

        for(LX_Vector2D& v: varr[i])
            g->acceptEnemyMissile(new Bullet(attack_val, sp, pos[i], v));
    }

    /*for(size_t i = 0; i < BulletPattern::WAVE_SZ; ++i)
    {
        g->acceptEnemyMissile(new Bullet(attack_val, sp, pos[0], varr1[i]));
        g->acceptEnemyMissile(new Bullet(attack_val, sp, pos[1], varr2[i]));
    }*/
}


void Boss03Body::strat0()
{
    if(position.x <= BOSS03_BODY_X)
    {
        id_strat = 1;
        speed *= 0.0f;
        addStrategy(new Boss03RayBullet(this));
    }
}

void Boss03Body::strat1Row()
{
    const uint32_t HEALTH_23 = max_health_point - max_health_point/3;

    if(health_point < HEALTH_23)
    {
        id_strat = 2;
        Engine::getInstance()->screenCancel();
        addStrategy(new Boss03RowBullet(this));
    }
}
void Boss03Body::strat2Wave()
{
    const uint32_t HEALTH_THIRD = max_health_point/3;

    if(health_point < HEALTH_THIRD)
    {
        id_strat = 3;
        Engine::getInstance()->screenCancel();
        addStrategy(new Boss03WaveBullet(this));
    }
}

void Boss03Body::strategy()
{
    switch(id_strat)
    {
    case 0:
        strat0();
        break;
    case 1:
        strat1Row();
        break;
    case 2:
        strat2Wave();
        break;
    default:
        break;
    }

    Boss::strategy();
}


void Boss03Body::move()
{
    poly->move(speed);
    Enemy::move();
}

void Boss03Body::collision(Missile *mi)
{
    if(LX_Physics::collisionRect(*(mi->getHitbox()), position))
    {
        if(LX_Physics::collisionRectPoly(*(mi->getHitbox()), *poly))
        {
            reaction(mi);
            mi->die();
        }
    }
}

void Boss03Body::collision(Player *play)
{
    if(!mustCheckCollision())
        return;

    if(LX_Physics::collisionCircleRect(*(play->getHitbox()), position))
    {
        if(LX_Physics::collisionCirclePoly(*(play->getHitbox()), *poly))
            play->die();
    }
}

void Boss03Body::die()
{
    /// @todo Boss03Body — die()
    Enemy::die();
}

Boss03Body::~Boss03Body()
{
    delete poly;
}

/** Boss03 Body strategies */

Boss03RayBullet::Boss03RayBullet(Boss03Body *b)
    : Strategy(b), ray_time(LX_Timer::getTicks()), body(b) {}


void Boss03RayBullet::proceed()
{
    if((LX_Timer::getTicks() - ray_time) > BOSS03_BODY_RAY1_DELAY)
    {
        body->rayShot();
        ray_time = LX_Timer::getTicks();
    }

    if((LX_Timer::getTicks() - reference_time) > BOSS03_BODY_RAY2_DELAY)
    {
        if(body->ray_id == 2)
            body->ray_id = 0;
        else
            body->ray_id += 1;

        body->circleShot();
        reference_time = LX_Timer::getTicks();
    }
}

/// Row

Boss03RowBullet::Boss03RowBullet(Boss03Body *b)
    : Boss03RayBullet(b), row_time(LX_Timer::getTicks()) {}

void Boss03RowBullet::proceed()
{
    if((LX_Timer::getTicks() - row_time) > BOSS03_BODY_ROW1_DELAY)
    {
        body->rowShot();
        row_time = LX_Timer::getTicks();
    }

    if((LX_Timer::getTicks() - reference_time) > BOSS03_BODY_ROW2_DELAY)
    {
        body->dShot();
        reference_time = LX_Timer::getTicks();
    }
}

/// Waves

Boss03WaveBullet::Boss03WaveBullet(Boss03Body *b)
    : Boss03RayBullet(b), wave_time(LX_Timer::getTicks()) {}

void Boss03WaveBullet::proceed()
{
    if((LX_Timer::getTicks() - wave_time) > BOSS03_BODY_WAVE_DELAY)
    {
        body->finalWave();
        wave_time = LX_Timer::getTicks();
    }

    if((LX_Timer::getTicks() - reference_time) > BOSS03_BODY_CIRCLE_DELAY)
    {
        body->circleShot();
        reference_time = LX_Timer::getTicks();
    }
}

