
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
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Log.hpp>

namespace
{
unsigned int BOSS03_HEAD_ID = 10;

const int BOSS03_BBULLET_ID = 4;
const int BOSS03_RBULLET_ID = 8;


/* Body */


const int BOSS03_BODY_X = 512;
const uint32_t BOSS03_BODY_RAY1_DELAY = 50;
const uint32_t BOSS03_BODY_RAY2_DELAY = 1000;

LX_Physics::LX_Vector2D rayv(-8.0f, 0.0f);


/* Head */

// Position of the HEAD
int BOSS03_HEAD_XOFF = 318;
int BOSS03_HEAD_YOFF = 158;
int BOSS03_HEAD_W = 461;
int BOSS03_HEAD_H = 336;

}


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
    /// @todo Boss03Body — constructor
    addStrategy(new MoveStrategy(this));
}


// strat01 — fire

void Boss03Body::rayShot()
{
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *sp = rc->getResource(RC_MISSILE, BOSS03_BBULLET_ID);

    LX_AABB BOSS03_RAY_POS[5] =
    {
        {position.x + 70, position.y + 182, 48, 24},
        {position.x + 12, position.y + 239, 48, 24},
        {position.x -32,  position.y + 314, 48, 24},
        {position.x + 12, position.y + 390, 48, 24},
        {position.x + 70, position.y + 448, 48, 24},
    };

    switch(ray_id)
    {
    case 0:
        g->acceptEnemyMissile(new Bullet(attack_val, sp, BOSS03_RAY_POS[2], rayv));
        break;
    case 1:
        g->acceptEnemyMissile(new Bullet(attack_val, sp, BOSS03_RAY_POS[1], rayv));
        g->acceptEnemyMissile(new Bullet(attack_val, sp, BOSS03_RAY_POS[3], rayv));
        break;
    default:
        g->acceptEnemyMissile(new Bullet(attack_val, sp, BOSS03_RAY_POS[0], rayv));
        g->acceptEnemyMissile(new Bullet(attack_val, sp, BOSS03_RAY_POS[4], rayv));
        break;
    }
}

void Boss03Body::circleShot()
{

}

void Boss03Body::fire()
{
    /// @todo Boss03Body — fire()
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
    /*const HEALTH

    {
        id_strat = 2;
        speed *= 0.0f;
        /// @todo new strategy
    }*/
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
    default:
        break;
    }

    /// @todo Boss03Body — strategy()
    Enemy::strategy();
}


void Boss03Body::move()
{
    /// @todo Boss03Body — move()
    Enemy::move();
}

void Boss03Body::collision(Missile *mi)
{
    /// @todo Boss03Body — collision(missile)
}

void Boss03Body::collision(Player *play)
{
    /// @todo Boss03Body — collision(player)
}

void Boss03Body::die()
{
    /// @todo Boss03Body — die()
    Enemy::die();
}


/** Boss03 Body strategies */

Boss03RayBullet::Boss03RayBullet(Boss03Body *b)
    : Strategy(b), body(b), ray_time(LX_Timer::getTicks()) {}


void Boss03RayBullet::proceed()
{
    if((LX_Timer::getTicks() - ray_time) > BOSS03_BODY_RAY1_DELAY)
    {
        ///@todo ray bullets
        //LX_Log::log("ray bullets");
        body->rayShot();
        ray_time = LX_Timer::getTicks();
    }

    if((LX_Timer::getTicks() - reference_time) > BOSS03_BODY_RAY2_DELAY)
    {
        ///@todo circle bullets
        //LX_Log::log("circle of bullets");
        if(body->ray_id == 2)
            body->ray_id = 0;
        else
            body->ray_id += 1;

        body->circleShot();
        reference_time = LX_Timer::getTicks();
    }
}
