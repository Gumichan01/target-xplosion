
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

const int SHIELD_XOFFSET = 16;
const int SHIELD_YOFFSET = 8;
const int SHIELD_WIDTH = 64;
const int SHIELD_HEIGHT = 254;

const int BOSS02_SPRITE_SHID = 3;
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

const uint32_t BOSS02_MSTRAT5_BULLET_DELAY = 100;
const float BOSS02_MSTRAT5_XVEL = -5;
const float BOSS02_MSTRAT5_YVEL = 2;

const int BOSS02_REFLECT_BULLET_ID = 8;
const int BOSS02_REFLECT_DIV = 6;
};

/// @todo (#1#) v0.5.0: Boss02 — implementation

Boss02::Boss02(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
               int x, int y, int w, int h, float vx, float vy)
    : Boss(hp, att, sh, image, audio, x, y, w, h, vx, vy),
    global_hitbox({0,0,0,0}), shield_hitbox({0,0,0,0}), poly(nullptr),
sh_sprite(nullptr), has_shield(false), b1time(0)
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
    shield_hitbox = {x + SHIELD_XOFFSET, y + SHIELD_YOFFSET,
                     SHIELD_WIDTH, SHIELD_HEIGHT
                    };

    addStrategy(new MoveStrategy(this));
    poly = new LX_Polygon();
    index = LX_Random::crand() %4;

    std::for_each(hpoints.begin(), hpoints.end(), [x,y](LX_Point& p)
    {
        p.x += x;
        p.y += y;
    });

    poly->addPoints(hpoints.begin(), hpoints.end());
    sh_sprite = ResourceManager::getInstance()->getResource(RC_ENEMY, BOSS02_SPRITE_SHID);
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

void Boss02::changeShotStrat(const uint32_t d)
{
    MoveAndShootStrategy *mvs = getMVSStrat();
    ShotStrategy *shot = new ShotStrategy(this);
    shot->setShotDelay(d);
    mvs->addShotStrat(shot);
}


// boss position in strategy #0
void Boss02::prepareTheAttack()
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
void Boss02::engage()
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
void Boss02::meshAttack()
{
    const uint32_t HP_83PERCENT = static_cast<float>(max_health_point) * 0.83f;
    const uint32_t HP_34PERCENT = static_cast<float>(max_health_point) * 0.34f;

    if(health_point < HP_34PERCENT || (!has_shield && health_point < HP_83PERCENT))
    {
        id_strat = 3;
        changeShotStrat(BOSS02_MSTRAT3_BULLET_DELAY);
        Game::getInstance()->screenCancel();
    }

}

// boss position in strategy #3
void Boss02::targetAttack()
{
    const uint32_t HP_66PERCENT = static_cast<float>(max_health_point) * 0.66f;
    const uint32_t HP_16PERCENT = static_cast<float>(max_health_point) * 0.16f;

    if(health_point < HP_16PERCENT || (!has_shield && health_point < HP_66PERCENT))
    {
        id_strat = 4;
        changeShotStrat(BOSS02_MSTRAT4_BULLET_DELAY);
        Game::getInstance()->screenCancel();
    }

}

void Boss02::bulletAttack()
{
    const uint32_t HP_50PERCENT = static_cast<float>(max_health_point) * 0.50f;
    const uint32_t HP_10PERCENT = static_cast<float>(max_health_point) * 0.10f;

    if(health_point == 0)
    {
        /// @todo Define the death of the boss
        /// @todo death strategy
        die();
    }
    else if(health_point < HP_10PERCENT)
    {
        id_strat = 5;
        speed.vx = 0.0f;
        speed.vy = 0.0f;
        changeShotStrat(BOSS02_MSTRAT5_BULLET_DELAY);
        Game::getInstance()->screenCancel();
    }
    else if(!has_shield && health_point < HP_50PERCENT)
    {
        id_strat = 2;
        has_shield = true;
        graphic = sh_sprite;
        changeShotStrat(BOSS02_MSTRAT1_BULLET_DELAY);
        Game::getInstance()->screenCancel();
    }
}

/// Shot

void Boss02::mesh()
{
    Game *g = Game::getInstance();
    ResourceManager *rm = ResourceManager::getInstance();

    float vx, vy;
    vx = (has_shield ? BOSS02_MSTRAT5_XVEL : BOSS02_MSTRAT1_XVEL);
    vy = (has_shield ? BOSS02_MSTRAT5_YVEL : BOSS02_MSTRAT1_YVEL);
    LX_Vector2D v[] = {LX_Vector2D(vx, vy),LX_Vector2D(vx, -vy)};

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


void Boss02::reflect(Missile *m)
{
    /// @todo reflect
    BasicMissile *bm;

    bm = dynamic_cast<BasicMissile*>(m);

    if(bm != nullptr)
    {
        LX_Log::log("Basic missile — can reflect it");
        Game *g = Game::getInstance();
        ResourceManager *rs = ResourceManager::getInstance();
        LX_Graphics::LX_Sprite * s = rs->getResource(RC_MISSILE, BOSS02_REFLECT_BULLET_ID);
        LX_Vector2D v(-(m->getXvel()/BOSS02_REFLECT_DIV), m->getYvel());
        LX_AABB r;
        {
            const LX_AABB *tmp = m->getHitbox();
            r = {tmp->x, tmp->y, tmp->w, tmp->h};
        }

        g->acceptEnemyMissile(new Bullet(attack_val, s, nullptr, r,v));
    }
    else
    {
        LX_Log::log("Not a basic missile — cannot reflect it");
    }

    m->die();
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

    case 5:
        mesh();
        //target();
        break;
    default:
        break;
    }
}

/// public functions

void Boss02::strategy()

{
    switch(id_strat)
    {
    case 0:
        prepareTheAttack();
        break;

    case 1:
        engage();
        break;

    case 2:
        meshAttack();
        break;

    case 3:
        targetAttack();
        break;

    case 4:
        bulletAttack();
        break;

    default:
        break;
    }

    Enemy::strategy();
}


void Boss02::move()
{
    moveRect(global_hitbox,speed);
    moveRect(shield_hitbox,speed);
    movePoly(*poly, speed);
    Boss::move();
}


void Boss02::collision(Missile *mi)
{
    if(has_shield)
    {
        if(collisionRect(*(mi->getHitbox()), shield_hitbox))
        {
            reflect(mi);
            return;
        }
    }

    if(collisionRect(*(mi->getHitbox()), global_hitbox))
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
    if(has_shield)
    {
        if(collisionCircleRect(*(play->getHitbox()), shield_hitbox))
        {
            play->die();
            return;
        }
    }

    if(collisionCircleRect(*(play->getHitbox()), global_hitbox))
    {
        if(collisionCirclePoly(*(play->getHitbox()), *poly))
            play->die();
    }

    return;
}


void Boss02::die()
{
    Enemy::die();
}


Boss02::~Boss02()
{
    delete poly;
}
