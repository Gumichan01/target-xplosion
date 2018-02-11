
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


#include "Boss01.hpp"
#include "../Bullet.hpp"
#include "../Player.hpp"
#include "../BasicMissile.hpp"
#include "../../game/engine/Engine.hpp"
#include "../../pattern/BulletPattern.hpp"
#include "../../game/engine/AudioHandler.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>

using namespace AudioHandler;
using namespace LX_Physics;

namespace
{
const int BOSS01_SIDES = 2;
const int BOSS01_LBULLET_ID = 4;
const int BOSS01_RBULLET_ID = 8;
const int BOSS01_PBULLET_ID = 9;
const int BOSS01_SPRITE_DID = 3;

// Limits of the boss's action area
const int BOSS01_MIN_XPOS = 876;
const int BOSS01_MAX_XPOS = 896;
const int BOSS01_MIN_YPOS = 152;
const int BOSS01_MAX_YPOS = 160;

// Delays fot the Circle01Strat
const uint32_t BOSS01_WSHOT_DELAY = 500;    // delay between two shots
const uint32_t BOSS01_WSHOT_TDELAY = 2100;  // total delay

// Boss01Circle02Strat
const int BOSS01_XLIM = 128;
const int BOSS01_YLIM_UP = 71;
const int BOSS01_YLIM_DOWN = 300;
const int BOSS01_SCIRCLE_VEL = 2;
const int BOSS01_SCIRCLE_BVEL = 6;
const float BOSS01_KILL_VEL = -7.5f;

// These values are used in order to set the position of the missiles
const int BOSS01_XOFF = 90;
const int BOSS01_YOFF1 = 54;
const int BOSS01_YOFF2 = 370;

const uint32_t MOVE_DELAY = 9000;
const uint32_t TOTAL_MOVE_DELAY = MOVE_DELAY + 2000;
const uint32_t BOSS01_SCIRCLE_DELAY = 1000;
const uint32_t BOSS01_DELAY_NOISE = 640;

const int BOSS01_VMULT = 4;
const int BOSS01_BULLET_DIM = 24;

const int BOSS01_BCIRCLE_N = 4;
const int BOSS01_BCIRCLE_XOFF = 98;
const int BOSS01_BCIRCLE_YOFF[4] = {134, 174, 260, 302};
const size_t BOSS01_BCIRCLE_NUM = CIRCLE_BULLETS;


const std::vector<LX_Point> HPOINTS
{
    LX_Point(48,224), LX_Point(60,162),
    LX_Point(24,87), LX_Point(106,42), LX_Point(182,87), LX_Point(151,162),
    LX_Point(162,224), LX_Point(151,281), LX_Point(182,357), LX_Point(106,406),
    LX_Point(24,357), LX_Point(60,285)
};

// The half of health points of the boss
inline unsigned int halfLife(unsigned int n)
{
    return n / 2;
}

}


/* ------------------------
            Boss 01
   ------------------------ */
Boss01::Boss01(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy), bshield(false), scircle_time(0),
      circle01_time(0), shape(HPOINTS, LX_Point{x,y}), id_pos(0)
{
    id_strat = 1;   // Set the first strategy ID
    addStrategy(new Boss01PositionStrat(this));
}


// Shoot two lines of bullets around the boss
void Boss01::sideCircleShot()
{
    LX_AABB rect[BOSS01_SIDES];
    int sp_offset = static_cast<int>(speed.vy);

    rect[0] = {position.x + BOSS01_XOFF, position.y + BOSS01_YOFF1 + sp_offset,
               BOSS01_BULLET_DIM, BOSS01_BULLET_DIM
              };
    rect[1] = {position.x + BOSS01_XOFF, position.y + BOSS01_YOFF2 + sp_offset,
               BOSS01_BULLET_DIM, BOSS01_BULLET_DIM
              };

    std::array<LX_Vector2D, BOSS01_BCIRCLE_NUM> varray;
    BulletPattern::circlePattern(rect[0].x, rect[0].y, BOSS01_SCIRCLE_BVEL, varray);

    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, BOSS01_RBULLET_ID);

    for(LX_Vector2D& v: varray)
    {
        g->acceptEnemyMissile(new BasicMissile(attack_val, spr, rect[0], v));
        g->acceptEnemyMissile(new BasicMissile(attack_val, spr, rect[1], v));
    }
}


void Boss01::shootToKill()
{
    LX_AABB rect[BOSS01_BCIRCLE_N];

    for(int i = 0; i < BOSS01_BCIRCLE_N; i++)
    {
        // X position and dimension
        rect[i].x = position.x + BOSS01_BCIRCLE_XOFF;
        rect[i].y = position.y + BOSS01_BCIRCLE_YOFF[i];
        rect[i].w = BOSS01_BULLET_DIM;
        rect[i].h = BOSS01_BULLET_DIM;
    }

    LX_Vector2D v;
    LX_Point p(position.x + position.w/2, position.y + position.h/2);
    BulletPattern::shotOnPlayer(p.x, p.y, BOSS01_KILL_VEL, v);

    Engine * g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *s = rc->getResource(RC_MISSILE, BOSS01_PBULLET_ID);

    for(LX_AABB& box : rect)
    {
        g->acceptEnemyMissile(new Bullet(attack_val, s, box, v));
    }
}

void Boss01::bulletCircleShot()
{
    LX_AABB rect[BOSS01_BCIRCLE_N];

    for(int i = 0; i < BOSS01_BCIRCLE_N; i++)
    {
        // X position and dimension
        rect[i].x = position.x + BOSS01_BCIRCLE_XOFF;
        rect[i].y = position.y + BOSS01_BCIRCLE_YOFF[i];
        rect[i].w = BOSS01_BULLET_DIM;
        rect[i].h = BOSS01_BULLET_DIM;
    }

    int j = id_pos++;
    std::array<LX_Vector2D, BOSS01_BCIRCLE_NUM> varray;
    BulletPattern::circlePattern(rect[j].x, rect[j].y, BOSS01_SCIRCLE_BVEL, varray);

    Engine * g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, BOSS01_LBULLET_ID);

    for(LX_Vector2D& v: varray)
    {
        g->acceptEnemyMissile(new Bullet(attack_val, spr, rect[j], v));
    }

    if(id_pos == BOSS01_BCIRCLE_N)
        id_pos = 0;
}

// Default shot, circle bullets
void Boss01::fire()
{
    switch(id_strat)
    {
    case 2:
        bulletCircleShot();
        break;

    case 3:
        sideCircleShot();
        break;

    case 4:
        sideCircleShot();
        shootToKill();
        break;

    default:
        break;
    }
}


void Boss01::bposition()
{
    if(position.x >= BOSS01_MIN_XPOS && position.x <= BOSS01_MAX_XPOS
            && position.y >= BOSS01_MIN_YPOS && position.y <= BOSS01_MAX_YPOS)
    {
        // Use the second strategy
        id_strat = 2;
        bshield = false;
        addStrategy(new Boss01Circle01Strat(this));
        circle01_time = LX_Timer::getTicks();
    }
}

void Boss01::circle01()
{
    if((LX_Timer::getTicks() - circle01_time) > BOSS01_WSHOT_TDELAY)
    {
        // Use the third strategy
        id_strat = (health_point < halfLife(max_health_point)) ? 4 : 3;
        speed *= 0.0f;
        addStrategy(new Boss01Circle02Strat(this));
        scircle_time = LX_Timer::getTicks();
    }
}

void Boss01::circle02()
{
    if((LX_Timer::getTicks() - scircle_time) > TOTAL_MOVE_DELAY)
    {
        // First strategy
        id_strat = 1;
        bshield = true;
        addStrategy(new Boss01PositionStrat(this));
    }
}


void Boss01::strategy()
{
    if(!dying)
    {
        switch(id_strat)
        {
        case 1:
            bposition();
            break;
        case 2:
            circle01();
            break;
        case 3:
        case 4:
            circle02();
            break;
        default:
            break;
        }
    }

    Boss::strategy();
}


void Boss01::move()
{
    movePoly(shape.getPoly(), speed);
    Enemy::move();
}

void Boss01::collision(Missile *mi)
{
    const LX_AABB& b = mi->getHitbox();

    // no shield + no dead missile + missile can hit + basic collision
    if(!mi->isDead() && !mi->explosion() && mustCheckCollision()
            && b.x <= (position.x + position.w) && collisionRect(position, b))
    {
        if(collisionRectPoly(b, shape.getPoly()))
        {
            if(destroyable && !bshield) reaction(mi);
            mi->die();
        }
    }
}

void Boss01::collision(Player *play)
{
    if(!mustCheckCollision())
        return;

    const LX_Circle& b = play->getHitbox();

    if(!play->isDead() && play->getX() <= (position.x + position.w)
            && collisionCircleRect(b, position))
    {
        if(collisionCirclePoly(b,shape.getPoly()))
            play->die();
    }
}

void Boss01::die()
{
    if(!dying)
    {
        const ResourceManager *rc = ResourceManager::getInstance();
        graphic = rc->getResource(RC_XPLOSION, BOSS01_SPRITE_DID);
        AudioHDL::getInstance()->stopBossMusic();
        AudioHDL::getInstance()->playVoiceMother();
        addStrategy(new BossDeathStrategy(this, DEFAULT_XPLOSION_DELAY,
                                          BOSS01_DELAY_NOISE));
    }

    Boss::die();
}


/* ------------------------
        Boss Strategy
   ------------------------ */

/* Position */
Boss01PositionStrat::Boss01PositionStrat(Boss01 * newEnemy)
    : Strategy(newEnemy), BossStrategy(newEnemy) {}


Boss01PositionStrat::~Boss01PositionStrat() {}


void Boss01PositionStrat::proceed()
{
    const float SPEED_X2 = 2.0f;
    const float SPEED_X3 = 3.0f;
    LX_Vector2D v(2.0f, 1.0f);

    if(boss->getHP() < halfLife(halfLife(boss->getMaxHP())))
        v *= SPEED_X3;

    else if(boss->getHP() < halfLife(boss->getMaxHP()))
        v *= SPEED_X2;

    // X position
    if(boss->getX() > BOSS01_MAX_XPOS)
        boss->setXvel(-v.vx);

    else if(boss->getX() < BOSS01_MIN_XPOS)
        boss->setXvel(v.vx);

    else
        boss->setXvel(0);

    // Y position
    if(boss->getY() > BOSS01_MAX_YPOS)
        boss->setYvel(-v.vy);

    else if(boss->getY() < BOSS01_MIN_YPOS)
        boss->setYvel(v.vy);

    else
        boss->setYvel(0);

    // Move normally
    boss->move();
}


/* Shoot */
Boss01Circle01Strat::Boss01Circle01Strat(Boss01 *newEnemy)
    : Strategy(newEnemy), BossStrategy(newEnemy), begin_circle01(0), first(true) {}

Boss01Circle01Strat::~Boss01Circle01Strat() {}


void Boss01Circle01Strat::proceed()
{
    uint32_t delay = BOSS01_WSHOT_DELAY;
    uint32_t total_delay = BOSS01_WSHOT_TDELAY;

    if(first)
    {
        begin_circle01 = LX_Timer::getTicks();
        first = false;
    }

    if(boss->getHP() < halfLife(boss->getMaxHP()))
        total_delay *= 2;

    if(boss->getHP() < halfLife(halfLife(boss->getMaxHP())))
    {
        total_delay *= 4;
        delay -= 100;
    }

    if((LX_Timer::getTicks() - begin_circle01) < total_delay)
    {
        static uint32_t wtime_tmp = 0;
        if((LX_Timer::getTicks() - wtime_tmp) > delay)
        {
            target->fire();
            wtime_tmp = LX_Timer::getTicks();
        }
    }
}


/* Row */
Boss01Circle02Strat::Boss01Circle02Strat(Boss01 *newEnemy)
    : Strategy(newEnemy), BossStrategy(newEnemy), first(true), begin_scircle(0),
      mv(new UpDownMoveStrategy(newEnemy, BOSS01_YLIM_UP, BOSS01_YLIM_DOWN, BOSS01_SCIRCLE_VEL)) {}


Boss01Circle02Strat::~Boss01Circle02Strat()
{
    delete mv;
}

void Boss01Circle02Strat::proceed()
{
    static uint32_t t = 0;

    if(first)
    {
        begin_scircle = LX_Timer::getTicks();
        first = false;
    }

    if((LX_Timer::getTicks() - begin_scircle) > MOVE_DELAY)
    {
        // Go to the left
        int v = 2;
        boss->setXvel(-v*BOSS01_VMULT);
        boss->setYvel(0);
    }
    else
    {
        if((LX_Timer::getTicks() - t) > BOSS01_SCIRCLE_DELAY)
        {
            target->fire();
            t = LX_Timer::getTicks();
        }
    }

    // On the left of the screen
    if(boss->getX() < BOSS01_XLIM)
    {
        boss->setXvel(0);
    }

    mv->proceed();
}
