
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

const Float BOSS03_DIV2 = {2.0f};
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

const Float BOSS03_BODY_SPIN_VEL = {6.0f};
const Float BOSS03_BODY_SPIN_STEP = BulletPattern::PI_F / Float{7.0f};

LX_Physics::LX_Vector2D boss03_ray_v = {-8.0f, FloatBox::FNIL};
const Float RAY_NORM = LX_Physics::vector_norm(boss03_ray_v);

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
const Float BOSS03_HEAD_RUN_VX = {-6.0f};
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
const Float BOSS03_HEAD_LIM1_VX = {-9.0f};
const Float BOSS03_HEAD_LIM2_VX = {-6.0f};
const Float BOSS03_HEAD_LIM3_VEL = {-8.0f};

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
const Float BOSS03_HEAD_CIRCLE_VEL = {7.0f};
const size_t BOSS03_HEAD_CIRCLE_N = BulletPattern::CIRCLE_BULLETS * 2;
const unsigned int BOSS03_HEAD_CIRCLE_DELAY = 1000;
const unsigned int BOSS03_HEAD_DCIRCLE_DELAY = 100;

const Float OURANOS_SPIN_VEL = {8.0f};
const unsigned int OURANOS_SPIN_DELAY = 100;
const Float OURANOS_STEP1 = BulletPattern::PI_F / Float{9.0f};
const Float OURANOS_STEP2 = BulletPattern::PI_F / Float{10.0f};

using LX_Physics::LX_FloatPosition;

const std::vector<LX_FloatPosition> BHPOINTS
{
    LX_FloatPosition{32,326}, LX_FloatPosition{191,166},
    LX_FloatPosition{256,166}, LX_FloatPosition{256,16},
    LX_FloatPosition{312,168}, LX_FloatPosition{341,168},
    LX_FloatPosition{341,64}, LX_FloatPosition{488,326},
    LX_FloatPosition{341,592}, LX_FloatPosition{341,480},
    LX_FloatPosition{312,478}, LX_FloatPosition{256,628},
    LX_FloatPosition{256,486}, LX_FloatPosition{191,486}
};

using LX_Physics::LX_FloatPosition;
const std::vector<LX_Physics::LX_FloatPosition> HHPOINTS
{
    LX_FloatPosition{16,16}, LX_FloatPosition{448,168},
    LX_FloatPosition{16,320}, LX_FloatPosition{90,168}
};

}

using namespace LX_Physics;

using namespace DynamicGameBalance;
using namespace BulletPattern;
using namespace AudioHandler;
using namespace AudioHandler;
using namespace LX_Physics;
using namespace FloatBox;



/** Boss03 */

Boss03::Boss03(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy)
    : Enemy(hp, att, sh, image, x, y, w, h, vx, vy), index(0)
{
    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *hsp = rc->getResource(RC_ENEMY, BOSS03_HEAD_ID);

    Boss03Body *body = new Boss03Body(hp / 2, att, sh, image, x, y, w, h, vx, vy);
    Boss03Head *head = new Boss03Head(hp / 2, att, sh, hsp, x + BOSS03_HEAD_XOFF,
                                      y + BOSS03_HEAD_YOFF, BOSS03_HEAD_W,
                                      BOSS03_HEAD_H, vx, vy);

    body->addObserver(*head);
    boss_parts[0] = body;
    boss_parts[1] = head;

    // We don't care about were it is.
    // The only thing that matters is where are the parts
    phybox.p = LX_FloatPosition{0.0f, FNIL};
    imgbox = {{0,0},0,0};
    speed *= FNIL;
}


void Boss03::draw() noexcept
{
    for(int i = static_cast<int>(BOSS03_PARTS) - 1; i >= 0; --i)
    {
        if(!boss_parts[i]->isDead())
            boss_parts[i]->draw();
    }
}


void Boss03::strategy() noexcept
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

void Boss03::collision(Missile *mi) noexcept
{
    if(!mi->isDead() && !mi->explosion())
        boss_parts[index]->collision(mi);
}

void Boss03::collision(Player *play) noexcept
{
    boss_parts[index]->collision(play);
}

void Boss03::die() noexcept
{
    still_alive = false;
}


Float Boss03::getX() const noexcept
{
    return boss_parts[index]->getX();
}

Float Boss03::getY() const noexcept
{
    return boss_parts[index]->getY();
}

int Boss03::getWidth() const noexcept
{
    return boss_parts[index]->getWidth();
}

int Boss03::getHeight() const noexcept
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
      shape(BHPOINTS, LX_FloatPosition{fbox(x), fbox(y)})
{
    addStrategy(new MoveStrategy(this));
    BulletPattern::initialize_array(BOSS03_BODY_SPIN_VEL, BOSS03_BODY_SPIN_STEP, vspin1);
    BulletPattern::initialize_array(BOSS03_BODY_SPIN_VEL, BOSS03_BODY_SPIN_STEP, vspin2, true);
}

void Boss03Body::addObserver(Boss03Head& obs) noexcept
{
    observer = &obs;
}


/// strat01 — fire!

void Boss03Body::rayShot() noexcept
{
    LX_Graphics::LX_ImgRect rpos[5] =
    {
        {imgbox.p.x + 70, imgbox.p.y + 182, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {imgbox.p.x + 12, imgbox.p.y + 239, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {imgbox.p.x - 32, imgbox.p.y + 314, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {imgbox.p.x + 12, imgbox.p.y + 390, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {imgbox.p.x + 70, imgbox.p.y + 448, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
    };

    const ResourceManager * const rc = ResourceManager::getInstance();
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

void Boss03Body::circleShot() noexcept
{
    LX_Graphics::LX_ImgRect cpos[2] =
    {
        {
            imgbox.p.x + BOSS03_BODY_CIRCLE1_XOFF,
            imgbox.p.y + BOSS03_BODY_CIRCLE1_YOFF,
            BOSS03_BODY_CBULLET_DIM, BOSS03_BODY_CBULLET_DIM
        },
        {
            imgbox.p.x + BOSS03_BODY_CIRCLE2_XOFF,
            imgbox.p.y + BOSS03_BODY_CIRCLE2_YOFF,
            BOSS03_BODY_CBULLET_DIM, BOSS03_BODY_CBULLET_DIM
        }
    };

    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_RBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    for(size_t i = 0; i < vspin1.size(); ++i)
    {
        LX_Vector2D v1, v2;
        (*vspin1[i])(fbox(cpos[0].p.x), fbox(cpos[0].p.y), v1);
        (*vspin2[i])(fbox(cpos[1].p.x), fbox(cpos[1].p.y), v2);
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, cpos[0], v1)));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, cpos[1], v2)));
    }
}

/// strat two - fire!

void Boss03Body::rowShot() noexcept
{
    LX_Graphics::LX_ImgRect rpos[2] =
    {
        {imgbox.p.x + 70, imgbox.p.y + 182, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
        {imgbox.p.x + 70, imgbox.p.y + 448, BOSS03_BODY_BULLET1_W, BOSS03_BODY_BULLET1_H},
    };

    LX_Graphics::LX_ImgRect cpos[2] =
    {
        {
            imgbox.p.x + BOSS03_BODY_CIRCLE1_XOFF,
            imgbox.p.y + BOSS03_BODY_CIRCLE1_YOFF,
            BOSS03_BODY_CBULLET_DIM, BOSS03_BODY_CBULLET_DIM
        },
        {
            imgbox.p.x + BOSS03_BODY_CIRCLE2_XOFF,
            imgbox.p.y + BOSS03_BODY_CIRCLE2_YOFF,
            BOSS03_BODY_CBULLET_DIM, BOSS03_BODY_CBULLET_DIM
        }
    };

    std::array<LX_Vector2D, CIRCLE_BULLETS * 2> varr;
    BulletPattern::circlePattern(fbox(cpos[0].p.x), fbox(cpos[0].p.y),
                                 BOSS03_BODY_SPIN_VEL, varr);

    const ResourceManager * const rc = ResourceManager::getInstance();
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

void Boss03Body::dShot() noexcept
{
    LX_Graphics::LX_ImgRect pos[2] =
    {
        {imgbox.p.x + 48, imgbox.p.y + 239, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {imgbox.p.x + 48, imgbox.p.y + 390, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
    };

    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varr1;
    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varr2;

    const Float& V = apply_dgb(-RAY_NORM);
    BulletPattern::waveOnPlayer(fbox(pos[0].p.x), fbox(pos[0].p.y), V, varr1);
    BulletPattern::waveOnPlayer(fbox(pos[1].p.x), fbox(pos[1].p.y), V, varr2);

    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    for(size_t i = 0; i < BulletPattern::WAVE_SZ; ++i)
    {
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, pos[0], varr1[i])));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, pos[1], varr2[i])));
    }
}


// strat 03 - fire!

void Boss03Body::finalWave() noexcept
{
    const size_t N = 5;

    LX_Graphics::LX_ImgRect pos[N] =
    {
        {imgbox.p.x + 90, imgbox.p.y + 182, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {imgbox.p.x + 44, imgbox.p.y + 239, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {imgbox.p.x, imgbox.p.y + 314, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {imgbox.p.x + 44, imgbox.p.y + 390, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
        {imgbox.p.x + 90, imgbox.p.y + 448, BOSS03_BODY_ROW_DIM, BOSS03_BODY_ROW_DIM},
    };

    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varr[N];
    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    for(size_t i = 0; i < N; ++i)
    {
        BulletPattern::waveOnPlayer(fbox(pos[i].p.x), fbox(pos[i].p.y), -RAY_NORM,
                                    varr[i]);

        for(LX_Vector2D& v: varr[i])
            hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, pos[i], v)));
    }
}


void Boss03Body::strat0() noexcept
{
    if(imgbox.p.x <= BOSS03_BODY_X)
    {
        id_strat = 1;
        speed *= FNIL;
        addStrategy(new Boss03RayBullet(this));
    }

    observer->notify(Boss03_MSG::MOVE);
}

void Boss03Body::strat1Row() noexcept
{
    const unsigned int HEALTH_23 = max_health_point - max_health_point/3;

    if(health_point < HEALTH_23)
    {
        id_strat = 2;
        EntityHandler::getInstance().bulletCancel();
        addStrategy(new Boss03RowBullet(this));
    }
}
void Boss03Body::strat2Wave() noexcept
{
    const unsigned int HEALTH_THIRD = max_health_point/3;

    if(health_point < HEALTH_THIRD)
    {
        id_strat = 3;
        EntityHandler::getInstance().bulletCancel();
        addStrategy(new Boss03WaveBullet(this));
    }
}

void Boss03Body::strategy() noexcept
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


void Boss03Body::move() noexcept
{
    shape.getPoly().move(speed);
    Enemy::move();
}

void Boss03Body::collision(Missile *mi) noexcept
{
    if(!mustCheckCollision()) return;

    if(LX_Physics::collisionBox(mi->getHitbox(), phybox))
    {
        if(LX_Physics::collisionBoxPoly(mi->getHitbox(), shape.getPoly()))
        {
            reaction(mi);
            mi->die();
        }
    }
}

void Boss03Body::collision(Player *play) noexcept
{
    if(!mustCheckCollision())
        return;

    if(LX_Physics::collisionCircleBox(play->getHitbox(), phybox))
    {
        if(LX_Physics::collisionCirclePoly(play->getHitbox(), shape.getPoly()))
            play->die();
    }
}

void Boss03Body::die() noexcept
{
    if(!dying)
    {
        const ResourceManager * const rc = ResourceManager::getInstance();
        graphic = rc->getResource(RC_XPLOSION, BOSS03_BODY_XID);
        addStrategy(new BossDeathStrategy(this, DEFAULT_XPLOSION_DELAY,
                                          OURANOS_BXDELAY));
    }

    Boss::die();
    speed.vx *= fbox(3.0f);
    speed.vy = fbox(0.0f);
}

Boss03Body::~Boss03Body()
{
    BulletPattern::destroy_array(vspin1);
    BulletPattern::destroy_array(vspin2);
}


/** Boss03 Body strategies */

Boss03RayBullet::Boss03RayBullet(Boss03Body *b)
    : Strategy(b), ray_time(LX_Timer::getTicks()), body(b) {}


void Boss03RayBullet::proceed() noexcept
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

void Boss03RowBullet::proceed() noexcept
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

void Boss03WaveBullet::proceed() noexcept
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
      shape(HHPOINTS, LX_FloatPosition{fbox(x), fbox(y)}), head_stratb(nullptr),
      pattern_up1(OURANOS_SPIN_VEL, OURANOS_STEP1),
      pattern_up2(OURANOS_SPIN_VEL, OURANOS_STEP1, BulletPattern::PI_F / fbox(2.0f)),
      pattern_down1(OURANOS_SPIN_VEL, OURANOS_STEP2),
      pattern_down2(OURANOS_SPIN_VEL, OURANOS_STEP2, BulletPattern::PI_F / fbox(2.0f))
{
    destroyHitSprite();
    createHitSprite();
    addStrategy(new MoveStrategy(this));
    BulletPattern::initialize_array(BOSS03_HEAD_CIRCLE_VEL, OURANOS_STEP1, vspin, true);
}

void Boss03Head::createHitSprite()
{
    using LX_Graphics::LX_ImgRect;
    using LX_Graphics::LX_BufferedImage;
    const TX_Asset * const a = TX_Asset::getInstance();
    LX_Win::LX_Window& w = LX_Win::getWindowManager().getWindow(WinID::getWinID());

    const TX_Anima * const an = a->getEnemyAnimation(BOSS03_HEAD_ID);
    const LX_ImgRect& r = (an == nullptr ? LX_ImgRect{{0,0},0,0} : (an->v[0]));
    hit_sprite = LX_BufferedImage(graphic->getFileName()).convertNegative().
                 generateSprite(w, r);
}


void Boss03Head::notify(const Boss03_MSG& msg) noexcept
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


void Boss03Head::propelShot() noexcept
{

    LX_Graphics::LX_ImgRect pos =
    {
        imgbox.p.x + BOSS03_HEAD_PROPEL_XOFF, imgbox.p.y + BOSS03_HEAD_PROPEL_YOFF,
        BOSS03_HEAD_PROPEL_W, BOSS03_HEAD_PROPEL_H
    };

    LX_Vector2D vel{-speed.vx, FNIL};
    LX_Vector2D vel_up{-speed.vx, -BOSS03_HEAD_PROPEL_VY};
    LX_Vector2D vel_down{-speed.vx, BOSS03_HEAD_PROPEL_VY};

    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, sp, pos, vel_up)));
    hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, sp, pos, vel_down)));
    hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, sp, pos, vel)));
}


void Boss03Head::prisonShot() noexcept
{
    const int N = 2;

    LX_Graphics::LX_ImgRect pos[N] =
    {
        {
            imgbox.p.x + BOSS03_HEAD_LIM_XOFF - BOSS03_HEAD_LIM_W / 2,
            imgbox.p.y + BOSS03_HEAD_LIM_Y1OFF, BOSS03_HEAD_LIM_W, BOSS03_HEAD_LIM_H
        },
        {
            imgbox.p.x + BOSS03_HEAD_LIM_XOFF - BOSS03_HEAD_LIM_W / 2,
            imgbox.p.y + BOSS03_HEAD_LIM_Y2OFF, BOSS03_HEAD_LIM_W, BOSS03_HEAD_LIM_H
        }
    };

    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    LX_Vector2D vel{BOSS03_HEAD_LIM1_VX, FNIL};
    hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, pos[0], vel)));
    hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, pos[1], vel)));
}

void Boss03Head::toPlayerShot01() noexcept
{
    const int M = 2;
    LX_Graphics::LX_ImgRect pos[M] =
    {
        {
            imgbox.p.x + BOSS03_HEAD_BLUE_XOFF,
            imgbox.p.y + BOSS03_HEAD_BLUE_Y1OFF,
            BOSS03_HEAD_LIM_DIM, BOSS03_HEAD_LIM_DIM
        },
        {
            imgbox.p.x + BOSS03_HEAD_BLUE_XOFF,
            imgbox.p.y + BOSS03_HEAD_BLUE_Y2OFF,
            BOSS03_HEAD_LIM_DIM, BOSS03_HEAD_LIM_DIM
        }
    };

    const LX_Physics::LX_FloatPosition P[M] =
    {
        LX_Physics::toFloatPosition(pos[0].p),
        LX_Physics::toFloatPosition(pos[1].p),
    };

    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varr1, varr2;
    BulletPattern::waveOnPlayer(P[0].x, P[0].y, BOSS03_HEAD_LIM3_VEL, varr1);
    BulletPattern::waveOnPlayer(P[1].x, P[1].y, BOSS03_HEAD_LIM3_VEL, varr2);

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *redsp = rc->getResource(RC_MISSILE, BOSS03_RBULLET_ID);

    generateGenericBulletCircles(pos[0], redsp, varr1.begin(), varr1.end());
    generateGenericBulletCircles(pos[1], redsp, varr2.begin(), varr2.end());

    LX_Vector2D vel{BOSS03_HEAD_LIM2_VX, FNIL};
    EntityHandler& hdl = EntityHandler::getInstance();

    LX_Graphics::LX_Sprite *bluesp = rc->getResource(RC_MISSILE, BOSS03_BBULLET_ID);
    hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, bluesp, pos[0], vel)));
    hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, bluesp, pos[1], vel)));

}

void Boss03Head::circleShot() noexcept
{
    const int M = 2;

    LX_Graphics::LX_ImgRect pos[M] =
    {
        {
            imgbox.p.x + BOSS03_HEAD_CIRCLE1_XOFF, imgbox.p.y + BOSS03_HEAD_CIRCLE1_YOFF,
            BOSS03_HEAD_CBULLET_DIM, BOSS03_HEAD_CBULLET_DIM
        },
        {
            imgbox.p.x + BOSS03_HEAD_CIRCLE2_XOFF, imgbox.p.y + BOSS03_HEAD_CIRCLE2_YOFF,
            BOSS03_HEAD_CBULLET_DIM, BOSS03_HEAD_CBULLET_DIM
        }
    };

    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *purplesp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    for(size_t i = 0; i < vspin.size(); ++i)
    {
        LX_Vector2D v;
        (*vspin[i])(fbox(pos[0].p.x), fbox(pos[0].p.y), v);
        hdl.pushEnemyMissile(*(new Bullet(attack_val, purplesp, pos[0], v)));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, purplesp, pos[1], v)));
    }
}


void Boss03Head::toPlayerShot02() noexcept
{
    const int M = 2;

    LX_Graphics::LX_ImgRect pos[M] =
    {
        {
            imgbox.p.x + BOSS03_HEAD_CIRCLE1_XOFF, imgbox.p.y + BOSS03_HEAD_CIRCLE1_YOFF,
            BOSS03_HEAD_CBULLET_DIM, BOSS03_HEAD_CBULLET_DIM
        },
        {
            imgbox.p.x + BOSS03_HEAD_CIRCLE2_XOFF, imgbox.p.y + BOSS03_HEAD_CIRCLE2_YOFF,
            BOSS03_HEAD_CBULLET_DIM, BOSS03_HEAD_CBULLET_DIM
        }
    };

    const LX_Physics::LX_FloatPosition P[M] =
    {
        LX_Physics::toFloatPosition(pos[0].p),
        LX_Physics::toFloatPosition(pos[1].p),
    };

    std::array<LX_Vector2D, BOSS03_HEAD_CIRCLE_N> varr1, varr2;
    BulletPattern::circlePattern(P[0].x, P[0].y, BOSS03_HEAD_CIRCLE_VEL, varr1);
    BulletPattern::circlePattern(P[1].x, P[1].y, BOSS03_HEAD_CIRCLE_VEL, varr2);

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *purplesp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);

    generateGenericBulletCircles(pos[0], purplesp, varr1.begin(), varr1.end(), true);
    generateGenericBulletCircles(pos[1], purplesp, varr2.begin(), varr2.end(), true);
}


void Boss03Head::spinShot() noexcept
{
    static short count_lunatic = 0;
    const short LUNATIC_MAX = 3;
    const size_t OURANOS_N = 2;

    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *purplesp = rc->getResource(RC_MISSILE, BOSS03_PBULLET_ID);

    LX_Graphics::LX_ImgRect pos[OURANOS_N] =
    {
        {
            imgbox.p.x + BOSS03_HEAD_CIRCLE1_XOFF, imgbox.p.y + BOSS03_HEAD_CIRCLE1_YOFF,
            BOSS03_HEAD_CBULLET_DIM, BOSS03_HEAD_CBULLET_DIM
        },
        {
            imgbox.p.x + BOSS03_HEAD_CIRCLE2_XOFF, imgbox.p.y + BOSS03_HEAD_CIRCLE2_YOFF,
            BOSS03_HEAD_CBULLET_DIM, BOSS03_HEAD_CBULLET_DIM
        }
    };

    const int VSIZE = 8;
    LX_Vector2D vec[VSIZE];

    const LX_Physics::LX_FloatPosition P[OURANOS_N] =
    {
        LX_Physics::toFloatPosition(pos[0].p),
        LX_Physics::toFloatPosition(pos[1].p),
    };

    pattern_up1(P[0].x, P[0].y, vec[0]);
    pattern_up2(P[0].x, P[0].y, vec[1]);
    pattern_down1(P[1].x, P[1].y, vec[2]);
    pattern_down2(P[1].x, P[1].y, vec[3]);

    for(int i = VSIZE / 2; i < VSIZE; ++i)
    {
        vec[i] = -vec[i - (VSIZE / 2)];
    }

    int j = 0;
    EntityHandler& hdl = EntityHandler::getInstance();
    // Spin bullet
    for(LX_Graphics::LX_ImgRect& p : pos)
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
        LX_Vector2D vel{BOSS03_HEAD_LIM2_VX, FNIL};
        hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, purplesp, pos[0], vel)));
        hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, purplesp, pos[1], vel)));
        count_lunatic = 0;
    }
    else
        count_lunatic++;
}


void Boss03Head::fire() noexcept
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


void Boss03Head::moveStrat() noexcept
{
    if(imgbox.p.x < BOSS03_BODY_X + BOSS03_HEAD_XOFF)
    {
        id_strat = 1;
        speed *= FNIL;
        speed.vx = BOSS03_HEAD_RUN_VX;

        mvs->addMoveStrat(new MoveStrategy(this));
        addStrategy(mvs);
    }
}


void Boss03Head::runToLeftStrat() noexcept
{
    if(imgbox.p.x < 0)
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


void Boss03Head::runToRightStrat() noexcept
{
    if(speed.vx > fbox(0.0f) )
    {
        static bool slow = false;

        if(imgbox.p.x > BOSS03_HEAD_X)
        {
            id_strat = 3;
            slow = false;
            speed *= FNIL;

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
        else if(imgbox.p.x > BOSS03_HEAD_XLOW && !slow)
        {
            speed.vx /= BOSS03_DIV2;
            slow = true;
        }
    }
}

void Boss03Head::prisonStrat() noexcept
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

void Boss03Head::circle01Strat() noexcept
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


void Boss03Head::spinStrat() noexcept
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


void Boss03Head::strategy() noexcept
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

void Boss03Head::move() noexcept
{
    shape.getPoly().move(speed);
    Enemy::move();
}

void Boss03Head::collision(Missile *mi) noexcept
{
    if(!mustCheckCollision()) return;

    if(LX_Physics::collisionBox(mi->getHitbox(), phybox))
    {
        if(LX_Physics::collisionBoxPoly(mi->getHitbox(), shape.getPoly()))
        {
            reaction(mi);
            mi->die();
        }
    }
}

void Boss03Head::collision(Player *play) noexcept
{
    if(!mustCheckCollision())
        return;

    if(LX_Physics::collisionCircleBox(play->getHitbox(), phybox))
    {
        if(LX_Physics::collisionCirclePoly(play->getHitbox(), shape.getPoly()))
            play->die();
    }
}

void Boss03Head::die() noexcept
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

void Boss03HeadStratBase::proceed() noexcept
{
    if((LX_Timer::getTicks() - reference_time) > BOSS03_HEAD_LIM_WDELAY)
    {
        head->toPlayerShot01();
        reference_time = LX_Timer::getTicks();
    }
}
