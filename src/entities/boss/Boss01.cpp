
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
#include "../../game/engine/AudioHandler.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Polygon.hpp>
#include <LunatiX/LX_Timer.hpp>

using namespace AudioHandler;
using namespace LX_Physics;

namespace
{
const int WALL_MISSILES = 4;
const int NB_ROW = 2;
const int BOSS01_LBULLET_ID = 4;
const int BOSS01_BULLET_ID = 8;
const int BOSS01_SPRITE_DID = 3;

// Limits of the boss's action area
const int BOSS01_MIN_XPOS = 876;
const int BOSS01_MAX_XPOS = 896;
const int BOSS01_MIN_YPOS = 152;
const int BOSS01_MAX_YPOS = 160;

// Delays fot the Wall strategy
const uint32_t TIME_BETWEEN_WALL_SHOTS = 250;
const uint32_t WALL_SHOTS_TOTAL_DELAY = 2000;

// Extreme limits when the boss use the Row Strategy
const int XLIM = 128;
const int YLIM_UP = 71;
const int YLIM_DOWN = 300;

// These values are used in order to set the position of the missiles
const int X_OFFSET = 74;
const int Y1_OFFSET = 1;
const int Y2_OFFSET = 432;

const uint32_t MOVE_DELAY = 8000;
const uint32_t TOTAL_MOVE_DELAY = MOVE_DELAY + 2000;
const uint32_t BOSS01_ROW_DELAY = 100;
const uint32_t BOSS01_SPRITE_DISPLAY_DELAY = 125;
const uint32_t BOSS01_DELAY_NOISE = BOSS01_SPRITE_DISPLAY_DELAY*5;

const int BOSS01_RVEL = 6;
const int BULLETS_VEL = 10;
const int BULLETS_DIM = 24;

// A specific RNG for the first boss
inline int randBoss01()
{
    return static_cast<int>((LX_Random::xorshiftRand() %3)+2);
}

// The half of health points of the boss
inline unsigned int halfLife(unsigned int n)
{
    return n/2;
}

}


/* ------------------------
            Boss 01
   ------------------------ */
Boss01::Boss01(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy), bshield(true), row_time(0),
      wall_time(0), hpoly(nullptr)
{
    id_strat = 1;   // Set the first strategy ID
    std::vector<LX_Physics::LX_Point> hpoints {LX_Point(108,16), LX_Point(130,22),
            LX_Point(204,112), LX_Point(204,177),LX_Point(170,223), LX_Point(204,270),
            LX_Point(204,336), LX_Point(130,425), LX_Point(108,432), LX_Point(81,425),
            LX_Point(7,336), LX_Point(7,270), LX_Point(41,223), LX_Point(7,177),
            LX_Point(7,112), LX_Point(81,22)
                                              };

    std::for_each(hpoints.begin(), hpoints.end(), [x,y](LX_Point& p)
    {
        p.x += x;
        p.y += y;
    });
    hpoly = new LX_Polygon();
    hpoly->addPoints(hpoints.begin(), hpoints.end());
    addStrategy(new Boss01PositionStrat(this));
}


void Boss01::bulletCirclesShot()
{
    LX_AABB rect[WALL_MISSILES];
    LX_Vector2D v = LX_Vector2D(0.0f, 0.0f);

    for(int i = 0; i < WALL_MISSILES; i++)
    {
        // X position and dimension
        rect[i].x = position.x +96;
        rect[i].w = BULLETS_DIM;
        rect[i].h = BULLETS_DIM;
    }

    rect[0].y = position.y + 115;
    rect[1].y = position.y + 150;
    rect[2].y = position.y + 275;
    rect[3].y = position.y + 310;

    Engine * g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, BOSS01_LBULLET_ID);

    for(int i = 0; i < WALL_MISSILES; i++)
    {

        g->acceptEnemyMissile(new MegaBullet(attack_val, spr, rect[i], v,
                                             BULLETS_VEL));
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
        addStrategy(new Boss01WallStrat(this));
        wall_time = LX_Timer::getTicks();
    }
}

void Boss01::wall()
{
    uint32_t delay = WALL_SHOTS_TOTAL_DELAY;

    if((LX_Timer::getTicks() - wall_time) > delay)
    {
        // Use the third strategy
        id_strat = 3;
        addStrategy(new Boss01RowStrat(this));
        row_time = LX_Timer::getTicks();
    }
}

void Boss01::row()
{
    uint32_t delay = TOTAL_MOVE_DELAY;

    if(health_point < halfLife(max_health_point))
        delay = TOTAL_MOVE_DELAY/2;

    if((LX_Timer::getTicks() - row_time) > delay)
    {
        // First strategy
        id_strat = 1;
        bshield = true;
        addStrategy(new Boss01PositionStrat(this));
    }
}

// Default shot, circle bullets
void Boss01::fire()
{
    if(id_strat == 3)
        rowShot();
    else if(id_strat == 2)
    {
        wallShot();
        bulletCirclesShot();
    }
}

// Shoot two lines of bullets around the boss
void Boss01::rowShot()
{
    LX_AABB rect[NB_ROW];

    int sp_offset = static_cast<int>(speed.vy);
    LX_Vector2D v = LX_Vector2D(-MISSILE_SPEED, 0.0f);
    LX_Vector2D v2 = LX_Vector2D((MISSILE_SPEED-(MISSILE_SPEED/4)), 0);

    rect[0] = {position.x + X_OFFSET, position.y + Y1_OFFSET + sp_offset,
               MISSILE_WIDTH, MISSILE_HEIGHT
              };
    rect[1] = {position.x + X_OFFSET, position.y + Y2_OFFSET + sp_offset,
               MISSILE_WIDTH, MISSILE_HEIGHT
              };

    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, BOSS01_BULLET_ID);

    for(int i = 0; i < NB_ROW; i++)
    {
        g->acceptEnemyMissile(new BasicMissile(attack_val, spr, rect[i], v));
        /*
            Launch missiles to the other side
            Change the X position of the other bullets
        */
        rect[i].x += MISSILE_WIDTH;
        g->acceptEnemyMissile(new BasicMissile(attack_val, spr, rect[i], v2));
    }
}

// Shoot four bullets at the same time
void Boss01::wallShot()
{
    const int N = WALL_MISSILES;
    LX_AABB rect[WALL_MISSILES];
    LX_Vector2D v = LX_Vector2D(-ROCKET_SPEED, 0);

    for(int i = 0; i < N; i++)
    {
        // X position and dimension
        rect[i].x = position.x +92;
        rect[i].w = 28;
        rect[i].h = 28;
    }

    // Y position of the bullets
    rect[0].y = position.y + 115;
    rect[1].y = position.y + 150;
    rect[2].y = position.y + 275;
    rect[3].y = position.y + 310;

    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, BOSS01_BULLET_ID);

    for(int j = 0; j < N; j++)
    {
        g->acceptEnemyMissile(new Bullet(attack_val,spr, rect[j], v));
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
            wall();
            break;
        case 3:
            row();
            break;
        default:
            break;
        }
    }

    Boss::strategy();
}


void Boss01::move()
{
    movePoly(*hpoly, speed);
    Enemy::move();
}

void Boss01::collision(Missile *mi)
{
    const LX_AABB& b = *mi->getHitbox();

    // no shield + no dead missile + missile can hit + basic collision
    if(!mi->isDead() && b.x <= (position.x + position.w)
            && collisionRect(position, b))
    {
        if(collisionRectPoly(b,*hpoly))
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

    const LX_Circle& b = *play->getHitbox();
    if(!play->isDead() && play->getX() <= (position.x + position.w)
            && collisionCircleRect(b, position))
    {
        if(collisionCirclePoly(b,*hpoly))
            play->die();
    }
}

void Boss01::die()
{
    if(!dying)
    {
        const ResourceManager *rc = ResourceManager::getInstance();
        graphic = rc->getResource(RC_XPLOSION, BOSS01_SPRITE_DID);
        Engine::getInstance()->stopBossMusic();
        AudioHDL::getInstance()->playVoiceMother();
        addStrategy(new BossDeathStrategy(this, DEFAULT_XPLOSION_DELAY,
                                          BOSS01_DELAY_NOISE));
    }

    Boss::die();
}


Boss01::~Boss01()
{
    delete hpoly;
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
Boss01WallStrat::Boss01WallStrat(Boss01 *newEnemy)
    : Strategy(newEnemy), BossStrategy(newEnemy), begin_wall(0), first(true) {}

Boss01WallStrat::~Boss01WallStrat() {}


void Boss01WallStrat::proceed()
{
    uint32_t delay = TIME_BETWEEN_WALL_SHOTS;
    uint32_t total_delay = WALL_SHOTS_TOTAL_DELAY;

    if(first == true)
    {
        begin_wall = LX_Timer::getTicks();
        first = false;
    }

    if(boss->getHP() < halfLife(boss->getMaxHP()))
        delay = TIME_BETWEEN_WALL_SHOTS/2;

    if(boss->getHP() < halfLife(halfLife(boss->getMaxHP())))
        delay = TIME_BETWEEN_WALL_SHOTS/4;

    // Shoot during 2 seconds
    if((LX_Timer::getTicks() - begin_wall) < total_delay)
    {
        static uint32_t wtime_tmp = 0;
        // Shoot every 250 ms
        if((LX_Timer::getTicks() - wtime_tmp) > delay)
        {
            target->fire();
            wtime_tmp = LX_Timer::getTicks();
        }
    }
}


/* Row */
Boss01RowStrat::Boss01RowStrat(Boss01 *newEnemy)
    : Strategy(newEnemy), BossStrategy(newEnemy), begin_row(0)
{
    boss->setYvel(randBoss01());
    boss->setXvel(0);
    first = 1;
}


Boss01RowStrat::~Boss01RowStrat() {}

void Boss01RowStrat::proceed()
{
    static uint32_t t = 0;
    int v = 1;
    uint32_t mv_delay = MOVE_DELAY;

    if(first == 1)
    {
        begin_row = LX_Timer::getTicks();
        first = 0;
    }

    // Row Shoot
    if((LX_Timer::getTicks() - t) > BOSS01_ROW_DELAY)
    {
        target->fire();
        t = LX_Timer::getTicks();
    }

    // The speed of the movement is greater
    if(boss->getHP() < halfLife(boss->getMaxHP()))
    {
        mv_delay = MOVE_DELAY/2;
        v += 1;
    }

    if((LX_Timer::getTicks() - begin_row) < mv_delay)
    {
        // Move faster
        if(boss->getY() < YLIM_UP)
        {
            boss->setYvel(randBoss01()*v);
        }
        else if(boss->getY() > YLIM_DOWN)
        {
            boss->setYvel(-randBoss01()*v);
        }
    }
    else
    {
        // Go to the left
        boss->setXvel(-v*BOSS01_RVEL);
        boss->setYvel(0);
    }

    // On the left of the screen
    if(boss->getX() < XLIM)
    {
        boss->setXvel(0);
    }

    boss->move();
}
