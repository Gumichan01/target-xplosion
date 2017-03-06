
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
const int BOSS02_MSTRAT1_BULLET_ID = 5;

const LX_Point BOSS02_MSTRAT1_BULLET_POS[] = {LX_Point(376, 137),
                                            LX_Point(342, 183),
                                            LX_Point(332, 105),
                                            LX_Point(294, 146)
                                           };

int index = -1;

const int BOSS02_MSTRAT1_BULLET_W = 16;
const int BOSS02_MSTRAT1_BULLET_H = 16;
const uint32_t BOSS02_MSTRAT1_BULLET_DELAY = 100;
};

/// @todo (#1#) v0.5.0: Boss02 — implementation

Boss02::Boss02(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
               int x, int y, int w, int h, float vx, float vy)
    : Boss(hp, att, sh, image, audio, x, y, w, h, vx, vy),
    global_hitbox({x + GLOBAL_XOFFSET, y + GLOBAL_YOFFSET, GLOBAL_BOXWIDTH, GLOBAL_BOXHEIGHT}),
poly(nullptr)
{
    std::vector<LX_Physics::LX_Point> hpoints {LX_Point(7,147), LX_Point(243,67),
            LX_Point(174,47), LX_Point(174,19),LX_Point(300,8), LX_Point(380,8),
            LX_Point(494,160), LX_Point(370,246), LX_Point(360,260), LX_Point(282,260),
            LX_Point(248,220), LX_Point(108,220), LX_Point(108,184), LX_Point(238,184),
            LX_Point(216,162)
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

// private functions

void Boss02::bposition()
{
    const int xlim = Game::getInstance()->getXlim();

    if(position.x <= (xlim - (xlim / 3)))
    {
        id_strat = 1;
        speed.vx = 0.0f;
        speed.vy = 0.0f;
        MoveAndShootStrategy *mvs = new MoveAndShootStrategy(this);
        ShotStrategy *shot = new ShotStrategy(this);
        shot->setShotDelay(BOSS02_MSTRAT1_BULLET_DELAY);
        mvs->addShotStrat(shot);
        addStrategy(mvs);
    }
}

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

void Boss02::fire()
{
    switch(id_strat)
    {
    case 1:
        mesh();
        break;

    default:
        break;
    }
}

// public fonctions

void Boss02::strategy()
{
    if(id_strat == 0)
        bposition();

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
