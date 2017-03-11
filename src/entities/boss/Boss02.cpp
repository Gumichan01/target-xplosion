
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
#include "../Bullet.hpp"
#include "../Rocket.hpp"
#include "../TreeMissile.hpp"
#include "../Player.hpp"
#include "../../game/Game.hpp"
#include "../../pattern/Strategy.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Polygon.hpp>
#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Log.hpp>   // remove it

#include <vector>
#include <algorithm>

using namespace LX_Physics;

namespace
{
const int GLOBAL_XOFFSET = 48;
const int GLOBAL_YOFFSET = 8;
const int GLOBAL_BOXWIDTH = 448;
const int GLOBAL_BOXHEIGHT = 256;   // or 248

const float BOSS02_MSTRAT1_XVEL = -4;
const float BOSS02_MSTRAT1_YVEL = 2;
const int BOSS02_MSTRAT1_BULLET_ID = 6;

const LX_Point BOSS02_MSTRAT1_BULLET_POS[] = {LX_Point(376, 137),
                                              LX_Point(342, 183),
                                              LX_Point(332, 105),
                                              LX_Point(294, 146)
                                             };

int index = -1;

const int BOSS02_MSTRAT1_BULLET_W = 16;
const int BOSS02_MSTRAT1_BULLET_H = 16;
const uint32_t BOSS02_MSTRAT1_BULLET_DELAY = 1000;

const uint32_t BOSS02_MSTRAT1_STOP_DELAY = 2000;
const float BOSS02_MSTRAT1_CIRC_STEP = 0.04f;
const int BOSS02_MSTRAT1_SPEED = 4;

const int BOSS02_MSTRAT2_YUP = 100;
const int BOSS02_MSTRAT2_YDOWN = 500;

const int BOSS02_MSTRAT3_BULLET_ID = 1;
const uint32_t BOSS02_MSTRAT3_BULLET_DELAY = 500;
const int BOSS02_MSTRAT3_ROCKET_XOFF = 80;
const int BOSS02_MSTRAT3_ROCKET_YOFF = 186;
const int BOSS02_MSTRAT3_ROCKET_WIDTH = 32;
const int BOSS02_MSTRAT3_ROCKET_HEIGHT = 12;
const int BOSS02_MSTRAT3_SPEED = -4;

const int BOSS02_MSTRAT4_BULLET_ID = 7;
const uint32_t BOSS02_MSTRAT4_BULLET_DELAY = 500;
const int BOSS02_MSTRAT4_BULLET_WIDTH = 28;
const int BOSS02_MSTRAT4_BULLET_HEIGHT = 28;
const int BOSS02_MSTRAT4_BULLET_XOFF = 174 - BOSS02_MSTRAT4_BULLET_WIDTH;
const int BOSS02_MSTRAT4_BULLET_YOFF = 19;
const int BOSS02_MSTRAT4_SPEED = -8;
const int BOSS02_MSTRAT44_SPEED = 8;
};

/// @todo (#1#) v0.5.0: Boss02 — implementation

Boss02::Boss02(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
               int x, int y, int w, int h, float vx, float vy)
    : Boss(hp, att, sh, image, audio, x, y, w, h, vx, vy),
    global_hitbox({0,0,0,0}), poly(nullptr), b1time(0)
{
    std::vector<LX_Physics::LX_Point> hpoints {LX_Point(7,147), LX_Point(243,67),
            LX_Point(174,47), LX_Point(174,19),LX_Point(300,8), LX_Point(380,8),
            LX_Point(405,64), LX_Point(464,88), LX_Point(494,160), LX_Point(464,218),
            LX_Point(432,248), LX_Point(370,246), LX_Point(360,260), LX_Point(282,260),
            LX_Point(248,220), LX_Point(108,220), LX_Point(108,184), LX_Point(238,184),
            LX_Point(216,162)
                                              };

    global_hitbox = {x + GLOBAL_XOFFSET, y + GLOBAL_YOFFSET,
                     GLOBAL_BOXWIDTH, GLOBAL_BOXHEIGHT
                    };
    addStrategy(new MoveStrategy(this));
    poly = new LX_Polygon();
    index = LX_Random::crand() %4;

    std::for_each(hpoints.begin(), hpoints.end(), [x,y](LX_Point& p)
    {
        p.x += x;
        p.y += y;
    });

    /// @todo (#5#) lib: update Lunatix — add every points in the polygon in one function (v0.11.0)
    std::for_each(hpoints.begin(), hpoints.end(), [this](const LX_Point& p)
    {
        poly->addPoint(p);
    });
}

/// private functions

MoveAndShootStrategy * Boss02::getMVSStrat()
{
    MoveAndShootStrategy *mvs = dynamic_cast<MoveAndShootStrategy*>(strat);

    if(mvs == nullptr)
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "RTTI — Cannot cast the current strategy");
    return mvs;
}

void Boss02::changeMoveStrat(const uint32_t d)
{
    MoveAndShootStrategy *mvs = getMVSStrat();
    ShotStrategy *shot = new ShotStrategy(this);
    shot->setShotDelay(d);
    mvs->addShotStrat(shot);
}


// boss position in strategy #0
void Boss02::b0position()
{
    const int xlim = Game::getInstance()->getXlim();

    if(position.x <= (xlim - (position.w)))
    {
        id_strat = 1;
        speed.vx = 0.0f;
        speed.vy = 0.0f;

        MoveAndShootStrategy *mvs = new MoveAndShootStrategy(this);
        ShotStrategy *shot = new ShotStrategy(this);
        MoveStrategy *mv = new MoveStrategy(this);
        shot->setShotDelay(BOSS02_MSTRAT1_BULLET_DELAY);
        mvs->addShotStrat(shot);
        mvs->addMoveStrat(mv);
        addStrategy(mvs);
        b1time = LX_Timer::getTicks();
    }
}

// boss position in strategy #1
void Boss02::b1position()
{
    if((LX_Timer::getTicks() - b1time) > BOSS02_MSTRAT1_STOP_DELAY)
    {
        MoveAndShootStrategy *mvs = getMVSStrat();
        mvs->addMoveStrat(new UpDownMoveStrategy(this, BOSS02_MSTRAT2_YUP,
                          BOSS02_MSTRAT2_YDOWN,
                          BOSS02_MSTRAT1_SPEED));
        speed.vx = 0.0f;
        speed.vy = BOSS02_MSTRAT1_SPEED;
        b1time = LX_Timer::getTicks();
        id_strat = 2;
    }
}

// boss position in strategy #2
void Boss02::b2position()
{
    const uint32_t HP_75PERCENT = static_cast<float>(max_health_point) * 0.83f;

    if(health_point < HP_75PERCENT)
    {
        id_strat = 3;
        changeMoveStrat(BOSS02_MSTRAT3_BULLET_DELAY);
        Game::getInstance()->screenCancel();
    }

}

// boss position in strategy #3
void Boss02::b3position()
{
    const uint32_t HP_50PERCENT = static_cast<float>(max_health_point) * 0.66f;

    if(health_point < HP_50PERCENT)
    {
        id_strat = 4;
        changeMoveStrat(BOSS02_MSTRAT4_BULLET_DELAY);
        Game::getInstance()->screenCancel();
    }

}

void Boss02::b4position()
{
    const uint32_t HP_50PERCENT = static_cast<float>(max_health_point) * 0.50f;

    if(health_point < HP_50PERCENT)
    {
        id_strat = 5;
        /// @todo repeat all of that with the sield
        //changeMoveStrat(BOSS02_MSTRAT4_BULLET_DELAY);
        Game::getInstance()->screenCancel();
    }

}

/// Shot

void Boss02::mesh()
{
    Game *g = Game::getInstance();
    ResourceManager *rm = ResourceManager::getInstance();

    LX_Vector2D v[] = {LX_Vector2D(BOSS02_MSTRAT1_XVEL, BOSS02_MSTRAT1_YVEL),
                       LX_Vector2D(BOSS02_MSTRAT1_XVEL,-BOSS02_MSTRAT1_YVEL)
                      };
    LX_AABB b = {position.x + BOSS02_MSTRAT1_BULLET_POS[index].x,
                 position.y + BOSS02_MSTRAT1_BULLET_POS[index].y,
                 BOSS02_MSTRAT1_BULLET_W, BOSS02_MSTRAT1_BULLET_H
                };

    LX_Graphics::LX_Sprite *s = rm->getResource(RC_MISSILE, BOSS02_MSTRAT1_BULLET_ID);
    g->acceptEnemyMissile(new TreeMissile(attack_val, s, nullptr, b, v[0]));
    g->acceptEnemyMissile(new TreeMissile(attack_val, s, nullptr, b, v[1]));
}

void Boss02::target()
{
    Game *g = Game::getInstance();
    ResourceManager *rm = ResourceManager::getInstance();
    LX_Vector2D v(BOSS02_MSTRAT3_SPEED, 0);
    LX_AABB b = {position.x + BOSS02_MSTRAT3_ROCKET_XOFF,
                 position.y + BOSS02_MSTRAT3_ROCKET_YOFF,
                 BOSS02_MSTRAT3_ROCKET_WIDTH, BOSS02_MSTRAT3_ROCKET_HEIGHT
                };

    LX_Graphics::LX_Sprite *s = rm->getResource(RC_MISSILE, BOSS02_MSTRAT3_BULLET_ID);
    g->acceptEnemyMissile(new EnemyRocket(attack_val, s, nullptr, b, v));
}

void Boss02::danmaku()
{
    static int id = 0;
    Game *g = Game::getInstance();
    ResourceManager *rm = ResourceManager::getInstance();
    LX_Vector2D v(BOSS02_MSTRAT4_SPEED, speed.vy/2.0f);

    LX_AABB b[2] = {{
            position.x + BOSS02_MSTRAT4_BULLET_XOFF,
            position.y + BOSS02_MSTRAT4_BULLET_YOFF,
            BOSS02_MSTRAT4_BULLET_WIDTH, BOSS02_MSTRAT4_BULLET_HEIGHT
        }, {
            position.x + BOSS02_MSTRAT3_ROCKET_XOFF,
            position.y + BOSS02_MSTRAT3_ROCKET_YOFF,
            BOSS02_MSTRAT4_BULLET_WIDTH, BOSS02_MSTRAT4_BULLET_HEIGHT
        }
    };

    LX_Graphics::LX_Sprite *s = rm->getResource(RC_MISSILE, BOSS02_MSTRAT4_BULLET_ID);
    g->acceptEnemyMissile(new MegaBullet(attack_val, s, nullptr, b[id], v,
                                         BOSS02_MSTRAT44_SPEED));
    id = 1 - id;
}

void Boss02::fire()
{
    switch(id_strat)
    {
    case 1:
    case 2:
        mesh();
        break;

    case 3:
        target();
        break;

    case 4:
        danmaku();
        break;

    default:
        break;
    }
}

/// public fonctions

void Boss02::strategy()

{
    switch(id_strat)
    {
    case 0:
        b0position();
        break;

    case 1:
        b1position();
        break;

    case 2:
        b2position();
        break;

    case 3:
        b3position();
        break;

    case 4:
        b4position();
        break;

    default:
        break;
    }

    //LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"p(%d,%d)", position.x, position.y);
    //LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"v(%f,%f)", speed.vx, speed.vy);

    Enemy::strategy();
}


void Boss02::move()
{
    moveRect(global_hitbox,speed);
    movePoly(*poly, speed);
    Boss::move();
}


void Boss02::collision(Missile *mi)
{
    if(LX_Physics::collisionRect(*(mi->getHitbox()), global_hitbox))
    {
        if(collisionRectPoly(*(mi->getHitbox()), *poly))
        {
            reaction(mi);
            mi->die();
        }
    }
}

void Boss02::collision(Player *play)
{
    if(collisionCircleRect(*(play->getHitbox()), global_hitbox))
    {
        if(collisionCirclePoly(*(play->getHitbox()), *poly))
            play->die();
    }
}


Boss02::~Boss02()
{
    delete poly;
}
