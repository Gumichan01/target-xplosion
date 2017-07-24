
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

const float BOSS03_DIV2 = 2.0f;
const uint32_t BOSS03_DIV4 = 4;

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

const int BOSS03_HEAD_X = 819;
const int BOSS03_HEAD_XLOW = BOSS03_HEAD_X - 128;
const float BOSS03_HEAD_RUN_VX = -6.0f;
const float BOSS03_HEAD_RMULT = 1.5f;

// propel
const int BOSS03_HEAD_PROPEL_XOFF = 98;
const int BOSS03_HEAD_PROPEL_YOFF = 162;
const int BOSS03_HEAD_PROPEL_W = 48;
const int BOSS03_HEAD_PROPEL_H = 16;
const uint32_t BOSS03_HEAD_PROPEL_DELAY = 50;
const float BOSS03_HEAD_PROPEL_VY = -1.5f;

// blue area
const int BOSS03_HEAD_BLUE_XOFF = 84;
const int BOSS03_HEAD_BLUE_Y1OFF = 89;
const int BOSS03_HEAD_BLUE_Y2OFF = 222;


// lim
const int BOSS03_HEAD_LIM_XOFF = 24;
const int BOSS03_HEAD_LIM_Y1OFF = 36;
const int BOSS03_HEAD_LIM_Y2OFF = 288;
const int BOSS03_HEAD_LIM_W = 64;
const int BOSS03_HEAD_LIM_H = 24;
const float BOSS03_HEAD_LIM1_VX = -9.0f;
const float BOSS03_HEAD_LIM2_VX = -6.0f;
const float BOSS03_HEAD_LIM3_VEL = -8.0f;

const int BOSS03_HEAD_LIM_DIM = 16;
const int BOSS03_HEAD_LIM_YUP = 80;
const int BOSS03_HEAD_LIM_YDOWN = 436;
const int BOSS03_HEAD_LIM_BVEL = 2;

const uint32_t BOSS03_HEAD_LIM_DELAY = 100;
const uint32_t BOSS03_HEAD_LIM_WDELAY = 500;


// circle

const int BOSS03_HEAD_CBULLET_DIM = BOSS03_BODY_CBULLET_DIM;
const int BOSS03_HEAD_CIRCLE1_XOFF = 84;
const int BOSS03_HEAD_CIRCLE1_YOFF = 89;
const int BOSS03_HEAD_CIRCLE2_XOFF = 84;
const int BOSS03_HEAD_CIRCLE2_YOFF = 222;
const int BOSS03_HEAD_CIRCLE_VEL = 8;
const size_t BOSS03_HEAD_CIRCLE_N = CIRCLE_BULLETS * 2;
const uint32_t BOSS03_HEAD_CIRCLE_DELAY = 1000;
const uint32_t BOSS03_HEAD_DCIRCLE_DELAY = 100;

const int OURANOS_SPIN_VEL = 4;
const uint32_t OURANOS_SPIN_DELAY = 17;
const float OURANOS_STEP = BulletPattern::PI_F/24.0f;

}

using namespace LX_Physics;

/** Boss03 */

Boss03::Boss03(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy)
    : Enemy(hp, att, sh, image, x, y, w, h, vx, vy), index(0)
{
    ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *hsp = rc->getResource(RC_ENEMY, BOSS03_HEAD_ID);
    Boss03Body *body = new Boss03Body(hp/2, att, sh, image, x, y, w, h, vx, vy);
    Boss03Head *head = new Boss03Head(hp/2, att, sh, hsp, x + BOSS03_HEAD_XOFF,
                                      y + BOSS03_HEAD_YOFF, BOSS03_HEAD_W,
                                      BOSS03_HEAD_H, vx, vy);

    body->addObserver(*head);
    boss_parts[0] = body;
    boss_parts[1] = head;

    // We don't care about were it is.
    // The only thing that matters is where are the parts
    fpos = FloatPosition(0.0f,0.0f) ;
    position = {0,0,0,0};
    speed *= 0.0f;
    image = nullptr;
}


void Boss03::draw()
{
    for(int i = static_cast<int>(BOSS03_PARTS) - 1; i >= 0; --i)
    {
        if(!boss_parts[i]->isDead())
            boss_parts[i]->draw();
    }
}


void Boss03::strategy()
{
    if(boss_parts[index]->isDead())
    {
        if(index == BOSS03_PARTS-1)
            die();
        else
            index += 1;

        LX_Log::log("index %d", index);
    }
    else
        boss_parts[index]->strategy();
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
    still_alive = false;
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
    for(size_t i = 0; i < BOSS03_PARTS; ++i)
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

void Boss03Body::addObserver(Boss03Head& obs)
{
    observer = &obs;
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
    const size_t N = 5;
    Engine *g = Engine::getInstance();
    const ResourceManager *rc  = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);

    LX_AABB pos[N] =
    {
        {position.x + 90, position.y + 182, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x + 44, position.y + 239, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x, position.y + 314, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x + 44, position.y + 390, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x + 90, position.y + 448, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
    };

    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varr[N];

    for(size_t i = 0; i < N; ++i)
    {
        BulletPattern::waveOnPlayer(pos[i].x, pos[i].y, -vector_norm(boss03_ray_v), varr[i]);

        for(LX_Vector2D& v: varr[i])
            g->acceptEnemyMissile(new Bullet(attack_val, sp, pos[i], v));
    }
}


void Boss03Body::strat0()
{
    if(position.x <= BOSS03_BODY_X)
    {
        id_strat = 1;
        speed *= 0.0f;
        addStrategy(new Boss03RayBullet(this));
    }

    observer->notify(Boss03_MSG::MOVE);
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
    /// @todo (#1#) Boss03Body — die() — boss animation
    Engine::getInstance()->screenCancel();
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


/** Boss03 Head */

Boss03Head::Boss03Head(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                       float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy), poly(nullptr),
      mvs(nullptr), head_stratb(nullptr), pattern_up(OURANOS_SPIN_VEL, OURANOS_STEP),
      pattern_down(OURANOS_SPIN_VEL, OURANOS_STEP)
{
    addStrategy(new MoveStrategy(this));

    std::vector<LX_Physics::LX_Point> hpoints
    {
        LX_Point(16,16), LX_Point(448,168),
        LX_Point(16,320), LX_Point(90,168)
    };

    poly = new LX_Polygon();

    std::for_each(hpoints.begin(), hpoints.end(), [x,y](LX_Point& p)
    {
        p.x += x;
        p.y += y;
    });

    poly->addPoints(hpoints.begin(), hpoints.end());
}

void Boss03Head::notify(const Boss03_MSG& msg)
{
    switch(msg)
    {
    case Boss03_MSG::MOVE:
        strategy();
        break;

    case Boss03_MSG::DEATH:
        id_strat = 2;
    default:
        break;
    }
}



void Boss03Head::propelShot()
{
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);

    LX_AABB pos =
    {
        position.x + BOSS03_HEAD_PROPEL_XOFF, position.y + BOSS03_HEAD_PROPEL_YOFF,
        BOSS03_HEAD_PROPEL_W, BOSS03_HEAD_PROPEL_H
    };

    LX_Vector2D vel(-speed.vx, 0.0f);
    LX_Vector2D vel_up(-speed.vx, -BOSS03_HEAD_PROPEL_VY);
    LX_Vector2D vel_down(-speed.vx, BOSS03_HEAD_PROPEL_VY);

    g->acceptEnemyMissile(new LunaticBullet(attack_val, sp, pos, vel_up));
    g->acceptEnemyMissile(new LunaticBullet(attack_val, sp, pos, vel_down));
    g->acceptEnemyMissile(new LunaticBullet(attack_val, sp, pos, vel));
}


void Boss03Head::prisonShot()
{
    const int N = 2;
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);

    LX_AABB pos[N] =
    {
        {
            position.x + BOSS03_HEAD_LIM_XOFF - BOSS03_HEAD_LIM_W / 2,
            position.y + BOSS03_HEAD_LIM_Y1OFF, BOSS03_HEAD_LIM_W, BOSS03_HEAD_LIM_H
        },
        {
            position.x + BOSS03_HEAD_LIM_XOFF - BOSS03_HEAD_LIM_W / 2,
            position.y + BOSS03_HEAD_LIM_Y2OFF, BOSS03_HEAD_LIM_W, BOSS03_HEAD_LIM_H
        }
    };

    LX_Vector2D vel(BOSS03_HEAD_LIM1_VX, 0.0f);
    g->acceptEnemyMissile(new Bullet(attack_val, sp, pos[0], vel));
    g->acceptEnemyMissile(new Bullet(attack_val, sp, pos[1], vel));
}

void Boss03Head::toPlayerShot01()
{
    const int M = 2;
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *redsp = rc->getResource(RC_MISSILE, BOSS03_RBULLET_ID);
    LX_Graphics::LX_Sprite *bluesp = rc->getResource(RC_MISSILE, BOSS03_BBULLET_ID);

    LX_AABB pos[M] =
    {
        {
            position.x + BOSS03_HEAD_BLUE_XOFF, position.y + BOSS03_HEAD_BLUE_Y1OFF,
            BOSS03_HEAD_LIM_DIM, BOSS03_HEAD_LIM_DIM
        },
        {
            position.x + BOSS03_HEAD_BLUE_XOFF, position.y + BOSS03_HEAD_BLUE_Y2OFF,
            BOSS03_HEAD_LIM_DIM, BOSS03_HEAD_LIM_DIM
        }
    };

    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varr1, varr2;
    BulletPattern::waveOnPlayer(pos[0].x, pos[0].y, BOSS03_HEAD_LIM3_VEL, varr1);
    BulletPattern::waveOnPlayer(pos[1].x, pos[1].y, BOSS03_HEAD_LIM3_VEL, varr2);

    generateGenericBulletCircles(pos[0], redsp, varr1.begin(), varr1.end());
    generateGenericBulletCircles(pos[1], redsp, varr2.begin(), varr2.end());

    Engine *g = Engine::getInstance();
    LX_Vector2D vel(BOSS03_HEAD_LIM2_VX, 0.0f);
    g->acceptEnemyMissile(new LunaticBullet(attack_val, bluesp, pos[0], vel));
    g->acceptEnemyMissile(new LunaticBullet(attack_val, bluesp, pos[1], vel));

}

void Boss03Head::circleShot01()
{
    const int M = 2;
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *purplesp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);

    LX_AABB pos[M] =
    {
        {
            position.x + BOSS03_HEAD_CIRCLE1_XOFF, position.y + BOSS03_HEAD_CIRCLE1_YOFF,
            BOSS03_HEAD_CBULLET_DIM, BOSS03_HEAD_CBULLET_DIM
        },
        {
            position.x + BOSS03_HEAD_CIRCLE2_XOFF, position.y + BOSS03_HEAD_CIRCLE2_YOFF,
            BOSS03_HEAD_CBULLET_DIM, BOSS03_HEAD_CBULLET_DIM
        }
    };

    std::array<LX_Vector2D, BOSS03_HEAD_CIRCLE_N> varr1, varr2;
    BulletPattern::circlePattern(pos[0].x, pos[0].y, BOSS03_HEAD_CIRCLE_VEL, varr1);
    BulletPattern::circlePattern(pos[1].x, pos[1].y, BOSS03_HEAD_CIRCLE_VEL, varr2);

    generateGenericBulletCircles(pos[0], purplesp, varr1.begin(), varr1.end());
    generateGenericBulletCircles(pos[1], purplesp, varr2.begin(), varr2.end());
}


void Boss03Head::toPlayerShot02()
{
    const int M = 2;
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *purplesp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);

    LX_AABB pos[M] =
    {
        {
            position.x + BOSS03_HEAD_CIRCLE1_XOFF, position.y + BOSS03_HEAD_CIRCLE1_YOFF,
            BOSS03_HEAD_CBULLET_DIM, BOSS03_HEAD_CBULLET_DIM
        },
        {
            position.x + BOSS03_HEAD_CIRCLE2_XOFF, position.y + BOSS03_HEAD_CIRCLE2_YOFF,
            BOSS03_HEAD_CBULLET_DIM, BOSS03_HEAD_CBULLET_DIM
        }
    };

    std::array<LX_Vector2D, BOSS03_HEAD_CIRCLE_N> varr1, varr2;
    BulletPattern::circlePattern(pos[0].x, pos[0].y, BOSS03_HEAD_CIRCLE_VEL, varr1);
    BulletPattern::circlePattern(pos[1].x, pos[1].y, BOSS03_HEAD_CIRCLE_VEL, varr2);

    generateGenericBulletCircles(pos[0], purplesp, varr1.begin(), varr1.end(), true);
    generateGenericBulletCircles(pos[1], purplesp, varr2.begin(), varr2.end(), true);
}


void Boss03Head::spinShot()
{
    const size_t OURANOS_N = 2;
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *purplesp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);

    LX_AABB pos[OURANOS_N] =
    {
        {
            position.x + BOSS03_HEAD_CIRCLE1_XOFF, position.y + BOSS03_HEAD_CIRCLE1_YOFF,
            BOSS03_HEAD_CBULLET_DIM, BOSS03_HEAD_CBULLET_DIM
        },
        {
            position.x + BOSS03_HEAD_CIRCLE2_XOFF, position.y + BOSS03_HEAD_CIRCLE2_YOFF,
            BOSS03_HEAD_CBULLET_DIM, BOSS03_HEAD_CBULLET_DIM
        }
    };

    std::array<LX_Vector2D, OURANOS_N> varray1, varray2;
    pattern_up(pos[0].x, pos[0].y, varray1);
    pattern_down(pos[1].x, pos[1].y, varray2);

    Engine *g = Engine::getInstance();

    for(LX_Vector2D& v: varray1)
    {
        LX_Vector2D rv = -v;
        g->acceptEnemyMissile(new Bullet(attack_val, purplesp, pos[0], v));
        g->acceptEnemyMissile(new Bullet(attack_val, purplesp, pos[0], rv));
    }

    for(LX_Vector2D& v: varray2)
    {
        LX_Vector2D rv = -v;
        g->acceptEnemyMissile(new Bullet(attack_val, purplesp, pos[1], v));
        g->acceptEnemyMissile(new Bullet(attack_val, purplesp, pos[1], rv));
    }

}


void Boss03Head::fire()
{
    switch(id_strat)
    {
    case 2:
        propelShot();
        break;

    case 3:
        prisonShot();
        break;

    case 4:
        circleShot01();
        break;

    case 5:
        toPlayerShot02();
        break;

    case 6:
        spinShot();
        break;

    default:
        break;
    }
}


void Boss03Head::moveStrat()
{
    if(position.x < BOSS03_BODY_X + BOSS03_HEAD_XOFF)
    {
        id_strat = 1;
        speed *= 0.0f;
        speed.vx = BOSS03_HEAD_RUN_VX;

        mvs = new MoveAndShootStrategy(this);
        mvs->addMoveStrat(new MoveStrategy(this));
        addStrategy(mvs);
    }
}


void Boss03Head::runToLeftStrat()
{
    if(position.x < 0)
    {
        id_strat = 2;
        speed.vx = -speed.vx;
        speed *= BOSS03_HEAD_RMULT;

        ShotStrategy *shot = new ShotStrategy(this);
        shot->setShotDelay(BOSS03_HEAD_PROPEL_DELAY);

        // I just need to update the MoveAndShootStrategy instance
        // because the strategy is already set
        // See moveStrat()
        mvs->addShotStrat(shot);
    }
}


void Boss03Head::runToRightStrat()
{
    static bool slow = false;

    if(speed.vx > 0.0f)
    {
        if(position.x > BOSS03_HEAD_X)
        {
            id_strat = 3;
            slow = false;
            speed *= 0.0f;

            // I don't want to replace a MoveAndShootStrategy instance by another one
            // So I just reuse it for the next boss pattern
            ShotStrategy * shot = new ShotStrategy(this);
            shot->setShotDelay(BOSS03_HEAD_LIM_DELAY);

            mvs->addShotStrat(shot);
            mvs->addMoveStrat(new UpDownMoveStrategy(this, BOSS03_HEAD_LIM_YUP,
                              BOSS03_HEAD_LIM_YDOWN, BOSS03_HEAD_LIM_BVEL));

            head_stratb = new Boss03HeadStratBase(this);

            // The Multiple strategy is necessary because I want
            //to combine two shotStrategy instance and a movement strategy
            MultiStrategy *multistrat = new MultiStrategy(this);
            multistrat->addStrat(*mvs);
            multistrat->addStrat(*head_stratb);

            /*
            *   If I use addStrategy(), then the previous value of strat
            *   will be deleted (freed). This previous vaue is mvs.
            *
            *   So, in order to avoid getting a dangling pointer and get a segmentation fault
            *   I have to set strat to NULL
            */
            strat = nullptr;
            addStrategy(multistrat);
        }
        else if(position.x > BOSS03_HEAD_XLOW && !slow)
        {
            speed.vx /= BOSS03_DIV2;
            slow = true;
        }
    }
}

void Boss03Head::prisonStrat()
{
    const uint32_t HEALTH_75 = max_health_point - max_health_point / BOSS03_DIV4;

    if(health_point < HEALTH_75)
    {
        id_strat = 4;
        Engine::getInstance()->screenCancel();
        ShotStrategy *s = new ShotStrategy(this);
        s->setShotDelay(BOSS03_HEAD_CIRCLE_DELAY);
        mvs->addShotStrat(s);
        addStrategy(mvs);
    }
}

void Boss03Head::circle01Strat()
{
    const uint32_t HEALTH_50 = max_health_point / BOSS03_DIV2;

    if(health_point < HEALTH_50)
    {
        id_strat = 5;
        Engine::getInstance()->screenCancel();
        ShotStrategy *s = new ShotStrategy(this);
        s->setShotDelay(BOSS03_HEAD_DCIRCLE_DELAY);
        mvs->addShotStrat(s);
    }
}


void Boss03Head::circle02Strat()
{
    const uint32_t HEALTH_25 = max_health_point / BOSS03_DIV4;

    if(health_point < HEALTH_25)
    {
        id_strat = 6;
        Engine::getInstance()->screenCancel();
        ShotStrategy *s = new ShotStrategy(this);
        s->setShotDelay(OURANOS_SPIN_DELAY);
        mvs->addShotStrat(s);
    }
}


void Boss03Head::strategy()
{
    switch(id_strat)
    {
    case 0:
        moveStrat();
        break;

    case 1:
        runToLeftStrat();
        break;

    case 2:
        runToRightStrat();
        break;

    case 3:
        prisonStrat();
        break;

    case 4:
        circle01Strat();
        break;

    case 5:
        circle02Strat();
        break;

    default:
        break;
    }

    Boss::strategy();
}

void Boss03Head::move()
{
    poly->move(speed);
    Enemy::move();
}

void Boss03Head::collision(Missile *mi)
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

void Boss03Head::collision(Player *play)
{
    if(!mustCheckCollision())
        return;

    if(LX_Physics::collisionCircleRect(*(play->getHitbox()), position))
    {
        if(LX_Physics::collisionCirclePoly(*(play->getHitbox()), *poly))
            play->die();
    }
}

void Boss03Head::die()
{
    /*
    *   strat was the pointer to the content of mvs
    *   So, when the boss dies, it calls addStrategy() and free the pointer
    *   in order to replace it with a DeathStrategy instance.
    *   → mvs become a dangling pointer
    *   That is why I assign NULL to strat to prevent a double free
    */
    strat = nullptr;
    Engine::getInstance()->screenCancel();
    Enemy::die();
}

Boss03Head::~Boss03Head()
{
    if(strat == nullptr || dynamic_cast<MoveAndShootStrategy*>(strat) == mvs)
        strat = nullptr;

    delete mvs;
    delete head_stratb;
    delete poly;
}


/** Boss03 Head strategies */

Boss03HeadStratBase::Boss03HeadStratBase(Boss03Head *b)
    : Strategy(b), head(b) {}

void Boss03HeadStratBase::proceed()
{
    if((LX_Timer::getTicks() - reference_time) > BOSS03_HEAD_LIM_WDELAY)
    {
        head->toPlayerShot01();
        reference_time = LX_Timer::getTicks();
    }
}


