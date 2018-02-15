
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

#include "../../game/Balance.hpp"
#include "../../asset/TX_Asset.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../game/engine/AudioHandler.hpp"
#include "../../resources/WinID.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>


namespace
{
unsigned int BOSS03_HEAD_ID = 10;

const int BOSS03_BBULLET_ID = 4;
const int BOSS03_RBULLET_ID = 8;
const int BOSS03_PBULLET_ID = 9;

const float BOSS03_DIV2 = 2.0f;
const unsigned int BOSS03_DIV4 = 4;

const unsigned int OURANOS_BXDELAY = 512;
//const unsigned int OURANOS_HXPLOSION_DELAY = 36;
const unsigned int OURANOS_HXDELAY = 640;

/* Body */

const int BOSS03_BODY_XID = 11;
const int BOSS03_BODY_X = 512;
const unsigned int BOSS03_BODY_RAY1_DELAY = 83;
const unsigned int BOSS03_BODY_RAY2_DELAY = 1000;
const int BOSS03_BODY_BULLET1_W = 48;
const int BOSS03_BODY_BULLET1_H = 24;
const int BOSS03_BODY_CBULLET_DIM = 24;
const int BOSS03_BODY_CIRCLE1_XOFF = 278;
const int BOSS03_BODY_CIRCLE1_YOFF = 470;
const int BOSS03_BODY_CIRCLE2_XOFF = 278;
const int BOSS03_BODY_CIRCLE2_YOFF = 158;

const int BOSS03_BODY_SPIN_VEL = 6;
const float BOSS03_BODY_SPIN_STEP = BulletPattern::PI_F / 7.0f;

LX_Physics::LX_Vector2D boss03_ray_v(-8.0f, 0.0f);

const unsigned int BOSS03_BODY_ROW1_DELAY = 100;
const unsigned int BOSS03_BODY_ROW2_DELAY = 1000;
const int BOSS03_BODY_ROW_DIM = 16;

const unsigned int BOSS03_BODY_WAVE_DELAY = 900;
const unsigned int BOSS03_BODY_CIRCLE_DELAY = 1000;


/* Head */

const int BOSS03_HEAD_XID = 12;

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
const unsigned int BOSS03_HEAD_PROPEL_DELAY = 50;
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

const unsigned int BOSS03_HEAD_LIM_DELAY = 100;
const unsigned int BOSS03_HEAD_LIM_WDELAY = 500;


// circle
const int BOSS03_HEAD_CBULLET_DIM = BOSS03_BODY_CBULLET_DIM;
const int BOSS03_HEAD_CIRCLE1_XOFF = 84;
const int BOSS03_HEAD_CIRCLE1_YOFF = 89;
const int BOSS03_HEAD_CIRCLE2_XOFF = 84;
const int BOSS03_HEAD_CIRCLE2_YOFF = 222;
const int BOSS03_HEAD_CIRCLE_VEL = 7;
const size_t BOSS03_HEAD_CIRCLE_N = BulletPattern::CIRCLE_BULLETS * 2;
const unsigned int BOSS03_HEAD_CIRCLE_DELAY = 1000;
const unsigned int BOSS03_HEAD_DCIRCLE_DELAY = 100;

const int OURANOS_SPIN_VEL = 8;
const unsigned int OURANOS_SPIN_DELAY = 100;
const float OURANOS_STEP1 = BulletPattern::PI_F/9.0f;
const float OURANOS_STEP2 = BulletPattern::PI_F/10.0f;

using LX_Physics::LX_Point;

const std::vector<LX_Point> BHPOINTS
{
    LX_Point(32,326), LX_Point(191,166),
    LX_Point(256,166), LX_Point(256,16),LX_Point(312,168), LX_Point(341,168),
    LX_Point(341,64), LX_Point(488,326), LX_Point(341,592), LX_Point(341,480),
    LX_Point(312,478), LX_Point(256,628), LX_Point(256,486), LX_Point(191,486),
};


const std::vector<LX_Physics::LX_Point> HHPOINTS
{
    LX_Point(16,16), LX_Point(448,168),
    LX_Point(16,320), LX_Point(90,168)
};

}

using namespace LX_Physics;

using namespace DynamicGameBalance;
using namespace BulletPattern;
using namespace AudioHandler;


/** Boss03 */

Boss03::Boss03(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy)
    : Enemy(hp, att, sh, image, x, y, w, h, vx, vy), index(0)
{
    const ResourceManager *rc = ResourceManager::getInstance();
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
    fpos = FloatPosition(0.0f,0.0f);
    position = {0,0,0,0};
    speed *= 0.0f;
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
    }
    else
        boss_parts[index]->strategy();
}

void Boss03::collision(Missile *mi)
{
    if(!mi->isDead() && !mi->explosion())
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
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy), ray_id(0),
      shape(BHPOINTS, LX_Point{x, y})
{
    addStrategy(new MoveStrategy(this));
    BulletPattern::initialize_array(BOSS03_BODY_SPIN_VEL, BOSS03_BODY_SPIN_STEP, vspin1);
    BulletPattern::initialize_array(BOSS03_BODY_SPIN_VEL, BOSS03_BODY_SPIN_STEP, vspin2, true);
}

void Boss03Body::addObserver(Boss03Head& obs)
{
    observer = &obs;
}


/// strat01 — fire!

void Boss03Body::rayShot()
{
    LX_AABB rpos[5] =
    {
        {position.x + 70, position.y + 182, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {position.x + 12, position.y + 239, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {position.x - 32, position.y + 314, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {position.x + 12, position.y + 390, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {position.x + 70, position.y + 448, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
    };

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_BBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    switch(ray_id)
    {
    case 0:
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, rpos[2], boss03_ray_v)));
        break;

    case 1:
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, rpos[1], boss03_ray_v)));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, rpos[3], boss03_ray_v)));
        break;

    default:
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, rpos[0], boss03_ray_v)));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, rpos[4], boss03_ray_v)));
        break;
    }
}

void Boss03Body::circleShot()
{
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

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_RBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    for(size_t i = 0; i < vspin1.size(); ++i)
    {
        LX_Vector2D v1, v2;
        (*vspin1[i])(cpos[0].x, cpos[0].y, v1);
        (*vspin2[i])(cpos[1].x, cpos[1].y, v2);
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, cpos[0], v1)));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, cpos[1], v2)));
    }
}

/// strat two - fire!

void Boss03Body::rowShot()
{
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

    std::array<LX_Vector2D, CIRCLE_BULLETS * 2> varr;
    BulletPattern::circlePattern(cpos[0].x, cpos[0].y,BOSS03_BODY_SPIN_VEL, varr);

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp1 = rc->getResource(RC_MISSILE, BOSS03_BBULLET_ID);
    LX_Graphics::LX_Sprite *sp2 = rc->getResource(RC_MISSILE, BOSS03_RBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    for(size_t i = 0; i < varr.size()/2 + 1; ++i)
    {
        LX_Vector2D v = -varr[i];
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp2, cpos[0], varr[i])));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp2, cpos[1], v)));
    }

    hdl.pushEnemyMissile(*(new Bullet(attack_val, sp1, rpos[0], boss03_ray_v)));
    hdl.pushEnemyMissile(*(new Bullet(attack_val, sp1, rpos[1], boss03_ray_v)));
}

void Boss03Body::dShot()
{
    LX_AABB pos[2] =
    {
        {position.x + 48, position.y + 239, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x + 48, position.y + 390, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
    };

    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varr1;
    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varr2;

    float vel = apply_dgb(-vector_norm(boss03_ray_v));
    BulletPattern::waveOnPlayer(pos[0].x, pos[0].y, vel, varr1);
    BulletPattern::waveOnPlayer(pos[1].x, pos[1].y, vel, varr2);

    const ResourceManager *rc  = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    for(size_t i = 0; i < BulletPattern::WAVE_SZ; ++i)
    {
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, pos[0], varr1[i])));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, pos[1], varr2[i])));
    }
}


// strat 03 - fire!

void Boss03Body::finalWave()
{
    const size_t N = 5;

    LX_AABB pos[N] =
    {
        {position.x + 90, position.y + 182, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x + 44, position.y + 239, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x, position.y + 314, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x + 44, position.y + 390, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {position.x + 90, position.y + 448, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
    };

    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varr[N];
    const ResourceManager *rc  = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    for(size_t i = 0; i < N; ++i)
    {
        BulletPattern::waveOnPlayer(pos[i].x, pos[i].y, -vector_norm(boss03_ray_v),
                                    varr[i]);

        for(LX_Vector2D& v: varr[i])
            hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, pos[i], v)));
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
    const unsigned int HEALTH_23 = max_health_point - max_health_point/3;

    if(health_point < HEALTH_23)
    {
        id_strat = 2;
        EntityHandler::getInstance().bulletCancel();
        addStrategy(new Boss03RowBullet(this));
    }
}
void Boss03Body::strat2Wave()
{
    const unsigned int HEALTH_THIRD = max_health_point/3;

    if(health_point < HEALTH_THIRD)
    {
        id_strat = 3;
        EntityHandler::getInstance().bulletCancel();
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
    shape.getPoly().move(speed);
    Enemy::move();
}

void Boss03Body::collision(Missile *mi)
{
    if(!mustCheckCollision()) return;

    if(LX_Physics::collisionRect(mi->getHitbox(), position))
    {
        if(LX_Physics::collisionRectPoly(mi->getHitbox(), shape.getPoly()))
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

    if(LX_Physics::collisionCircleRect(play->getHitbox(), position))
    {
        if(LX_Physics::collisionCirclePoly(play->getHitbox(), shape.getPoly()))
            play->die();
    }
}

void Boss03Body::die()
{
    if(!dying)
    {
        const ResourceManager *rc = ResourceManager::getInstance();
        graphic = rc->getResource(RC_XPLOSION, BOSS03_BODY_XID);
        addStrategy(new BossDeathStrategy(this, DEFAULT_XPLOSION_DELAY,
                                          OURANOS_BXDELAY));
    }

    Boss::die();
    speed.vx *= 3.0f;
    speed.vy = 0.0f;
}

Boss03Body::~Boss03Body()
{
    BulletPattern::destroy_array(vspin1);
    BulletPattern::destroy_array(vspin2);
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
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy),
      shape(HHPOINTS, LX_Point{x,y}), head_stratb(nullptr),
      pattern_up1(OURANOS_SPIN_VEL, OURANOS_STEP1),
      pattern_up2(OURANOS_SPIN_VEL, OURANOS_STEP1, BulletPattern::PI_F/2.0f),
      pattern_down1(OURANOS_SPIN_VEL, OURANOS_STEP2),
      pattern_down2(OURANOS_SPIN_VEL, OURANOS_STEP2, BulletPattern::PI_F/2.0f)
{
    destroyHitSprite();
    createHitSprite();
    addStrategy(new MoveStrategy(this));
    BulletPattern::initialize_array(BOSS03_HEAD_CIRCLE_VEL, OURANOS_STEP1, vspin, true);
}

void Boss03Head::createHitSprite()
{
    const TX_Asset *a = TX_Asset::getInstance();
    LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(WinID::getWinID());
    LX_Graphics::LX_BufferedImage bf(graphic->getFileName());
    bf.convertNegative();

    const TX_Anima *an = a->getEnemyAnimation(BOSS03_HEAD_ID);
    const LX_AABB * r = (an == nullptr ? nullptr : &(an->v[0]));
    hit_sprite = bf.generateSprite(*w, const_cast<LX_AABB *>(r));
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

    LX_AABB pos =
    {
        position.x + BOSS03_HEAD_PROPEL_XOFF, position.y + BOSS03_HEAD_PROPEL_YOFF,
        BOSS03_HEAD_PROPEL_W, BOSS03_HEAD_PROPEL_H
    };

    LX_Vector2D vel(-speed.vx, 0.0f);
    LX_Vector2D vel_up(-speed.vx, -BOSS03_HEAD_PROPEL_VY);
    LX_Vector2D vel_down(-speed.vx, BOSS03_HEAD_PROPEL_VY);

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, sp, pos, vel_up)));
    hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, sp, pos, vel_down)));
    hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, sp, pos, vel)));
}


void Boss03Head::prisonShot()
{
    const int N = 2;

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

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    LX_Vector2D vel{BOSS03_HEAD_LIM1_VX, 0.0f};
    hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, pos[0], vel)));
    hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, pos[1], vel)));
}

void Boss03Head::toPlayerShot01()
{
    const int M = 2;
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
    BulletPattern::waveOnPlayer(pos[0].x, pos[0].y, apply_dgb(BOSS03_HEAD_LIM3_VEL), varr1);
    BulletPattern::waveOnPlayer(pos[1].x, pos[1].y, apply_dgb(BOSS03_HEAD_LIM3_VEL), varr2);

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *redsp = rc->getResource(RC_MISSILE, BOSS03_RBULLET_ID);

    generateGenericBulletCircles(pos[0], redsp, varr1.begin(), varr1.end());
    generateGenericBulletCircles(pos[1], redsp, varr2.begin(), varr2.end());

    LX_Vector2D vel(BOSS03_HEAD_LIM2_VX, 0.0f);
    EntityHandler& hdl = EntityHandler::getInstance();

    LX_Graphics::LX_Sprite *bluesp = rc->getResource(RC_MISSILE, BOSS03_BBULLET_ID);
    hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, bluesp, pos[0], vel)));
    hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, bluesp, pos[1], vel)));

}

void Boss03Head::circleShot()
{
    const int M = 2;

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

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *purplesp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    for(size_t i = 0; i < vspin.size(); ++i)
    {
        LX_Vector2D v;
        (*vspin[i])(pos[0].x, pos[0].y, v);
        hdl.pushEnemyMissile(*(new Bullet(attack_val, purplesp, pos[0], v)));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, purplesp, pos[1], v)));
    }
}


void Boss03Head::toPlayerShot02()
{
    const int M = 2;

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
    BulletPattern::circlePattern(pos[0].x, pos[0].y, apply_dgb(BOSS03_HEAD_CIRCLE_VEL), varr1);
    BulletPattern::circlePattern(pos[1].x, pos[1].y, apply_dgb(BOSS03_HEAD_CIRCLE_VEL), varr2);

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *purplesp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);

    generateGenericBulletCircles(pos[0], purplesp, varr1.begin(), varr1.end(), true);
    generateGenericBulletCircles(pos[1], purplesp, varr2.begin(), varr2.end(), true);
}


void Boss03Head::spinShot()
{
    static short count_lunatic = 0;
    const short LUNATIC_MAX = 3;
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

    const int VSIZE = 8;
    LX_Vector2D vec[VSIZE];

    pattern_up1(pos[0].x, pos[0].y, vec[0]);
    pattern_up2(pos[0].x, pos[0].y, vec[1]);
    pattern_down1(pos[1].x, pos[1].y, vec[2]);
    pattern_down2(pos[1].x, pos[1].y, vec[3]);

    for(int i = VSIZE / 2; i < VSIZE; ++i)
    {
        vec[i] = vec[i - (VSIZE / 2)];
    }

    int j = 0;
    EntityHandler& hdl = EntityHandler::getInstance();
    // Spin bullet
    for(LX_AABB& p : pos)
    {
        hdl.pushEnemyMissile(*(new Bullet(attack_val, purplesp, p, vec[j])));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, purplesp, p, vec[j + 1])));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, purplesp, p, vec[j + 4])));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, purplesp, p, vec[j + 5])));
        j += 2;
    }

    // Lunatic bullets
    if(count_lunatic == LUNATIC_MAX)
    {
        LX_Vector2D vel(BOSS03_HEAD_LIM2_VX, 0.0f);
        hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, purplesp, pos[0], vel)));
        hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, purplesp, pos[1], vel)));
        count_lunatic = 0;
    }
    else
        count_lunatic++;
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
        circleShot();
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
    if(speed.vx > 0.0f)
    {
        static bool slow = false;

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

            // strat was set to mvs , so I don't want to remove it
            addStrategy(multistrat, false);
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
    const unsigned int HEALTH_75 = max_health_point - max_health_point / BOSS03_DIV4;

    if(health_point < HEALTH_75)
    {
        id_strat = 4;
        EntityHandler::getInstance().bulletCancel();
        ShotStrategy *s = new ShotStrategy(this);
        s->setShotDelay(BOSS03_HEAD_CIRCLE_DELAY);
        mvs->addShotStrat(s);
        addStrategy(mvs);
    }
}

void Boss03Head::circle01Strat()
{
    const unsigned int HEALTH_50 = max_health_point / BOSS03_DIV2;

    if(health_point < HEALTH_50)
    {
        id_strat = 5;
        EntityHandler::getInstance().bulletCancel();
        ShotStrategy *s = new ShotStrategy(this);
        s->setShotDelay(BOSS03_HEAD_DCIRCLE_DELAY);
        mvs->addShotStrat(s);
    }
}


void Boss03Head::spinStrat()
{
    const unsigned int HEALTH_25 = max_health_point / BOSS03_DIV4;

    if(health_point < HEALTH_25)
    {
        id_strat = 6;
        EntityHandler::getInstance().bulletCancel();
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
        spinStrat();
        break;

    default:
        break;
    }

    Boss::strategy();
}

void Boss03Head::move()
{
    shape.getPoly().move(speed);
    Enemy::move();
}

void Boss03Head::collision(Missile *mi)
{
    if(!mustCheckCollision()) return;

    if(LX_Physics::collisionRect(mi->getHitbox(), position))
    {
        if(LX_Physics::collisionRectPoly(mi->getHitbox(), shape.getPoly()))
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

    if(LX_Physics::collisionCircleRect(play->getHitbox(), position))
    {
        if(LX_Physics::collisionCirclePoly(play->getHitbox(), shape.getPoly()))
            play->die();
    }
}

void Boss03Head::die()
{
    if(!dying)
    {
        const ResourceManager *rc = ResourceManager::getInstance();
        graphic = rc->getResource(RC_XPLOSION, BOSS03_HEAD_XID);

        AudioHDL::getInstance()->stopBossMusic();
        AudioHDL::getInstance()->playVoiceMother();

        /*
        *   strat was the pointer to the content of mvs
        *   So, when the boss dies, it calls addStrategy()
        *   in order to replace it with a DeathStrategy instance
        *   WITHOUT deleting the previous data
        */
        addStrategy(new BossDeathStrategy(this, DEFAULT_XPLOSION_DELAY,
                                          OURANOS_HXDELAY), false);
    }

    Boss::die();
}

Boss03Head::~Boss03Head()
{
    delete head_stratb;
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
