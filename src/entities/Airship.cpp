
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

#include "Airship.hpp"

#include "Player.hpp"
#include "Bullet.hpp"
#include "Bomb.hpp"
#include "../pattern/Strategy.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../game/engine/Engine.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Polygon.hpp>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Log.hpp>
#include <vector>

using namespace LX_Physics;

namespace
{
const int AIRSHIP_WIDTH = 250;
const int AIRSHIP_HEIGHT = 100;

const int AIRSHIP_FRONT_YPOS = 255;
const int AIRSHIP_BOTTOM_YPOS = 261;

// Bomb

const int AIRSHIP_BOMB_ID = 2;
const int AIRSHIP_BOMB_XOFF = 124;
const int AIRSHIP_BOMB_YOFF = 76;
const int AIRSHIP_BOMB_DIM = 16;
const int AIRSHIP_BOMB_VEL = 10;
const int AIRSHIP_BOMB_NUM = CIRCLE_BULLETS/2;
const uint32_t AIRSHIP_BOMB_DELAY = 500;
}


Airship::Airship(unsigned int hp, unsigned int att, unsigned int sh,
                 LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                 float vx, float vy)
    : Enemy(hp, att, sh, image, x, y, w, h, vx, vy), strat(0),
    main_hitbox({position.x, position.y, AIRSHIP_WIDTH, AIRSHIP_HEIGHT})
{
    std::vector<LX_Point> hpoints {LX_Point(12,38), LX_Point(24,18),
                                   LX_Point(120,6), LX_Point(222,18),LX_Point(248,38), LX_Point(222,64),
                                   LX_Point(184,70), LX_Point(156,96), LX_Point(61,96), LX_Point(45,68),
                                   LX_Point(24,58)
                                  };

    poly_hitbox = new LX_Polygon();
    MoveAndShootStrategy *mvs = new MoveAndShootStrategy(this);
    mvs->addMoveStrat(new MoveStrategy(this));
    addStrategy(mvs);

    std::for_each(hpoints.begin(), hpoints.end(), [x,y](LX_Point& p)
    {
        p.x += x;
        p.y += y;
    });

    poly_hitbox->addPoints(hpoints.begin(), hpoints.end());
}

void Airship::move()
{
    moveRect(main_hitbox,speed);
    movePoly(*poly_hitbox, speed);
    Enemy::move();
}

void Airship::draw()
{
    if(dying)
    {
        const int N = 9;
        LX_AABB box[N] = {{24,32,64,64}, {64,10,64,64}, {48,64,64,64},
            {64,80,64,64}, {130,76,64,64}, {110,8,64,64}, {91,51,64,64},
            {174,24,64,64}, {226,32,64,64}
        };

        for(int i = 0; i < N; i++)
        {
            box[i].x += position.x;
            box[i].y += position.y;
            graphic->draw(&box[i]);
        }
    }
    else
        Enemy::draw();
}

void Airship::collision(Missile *mi)
{
    if(!mi->isDead() && mi->getX() <= (position.x + position.w) && !dying)
    {
        if(LX_Physics::collisionRect(main_hitbox, *mi->getHitbox()))
        {
            if(LX_Physics::collisionRectPoly(*mi->getHitbox(), *poly_hitbox))
            {
                if(destroyable) reaction(mi);
                mi->die();
            }
        }
    }
}

void Airship::collision(Player *play)
{
    if(play->getX() <= (position.x + position.w) && !dying)
    {
        if(LX_Physics::collisionCircleRect(*play->getHitbox(), main_hitbox))
        {
            if(LX_Physics::collisionCirclePoly(*play->getHitbox(), *poly_hitbox))
            {
                play->die();
            }
        }
    }
}

/// Strategy

void Airship::prepare()
{
    if(position.y < AIRSHIP_FRONT_YPOS)
    {
        strat = 1;
        ShotStrategy *shot = new ShotStrategy(this);
        shot->setShotDelay(AIRSHIP_BOMB_DELAY);
        getMVSStrat()->addShotStrat(shot);
    }
    else if(position.y >= AIRSHIP_BOTTOM_YPOS)
    {
        strat = 2;
        speed.vy = speed.vx / 2.0f;
        /// @todo front shot + move to the top
    }
    else
    {
        strat = 3;
        /// @todo front shot + spinner ullets
    }
}

void Airship::strategy()
{
    switch(strat)
    {
    case 0:
        prepare();
        break;

    default:
        break;
    }

    Enemy::strategy();
}


/// Fire

void Airship::bomb()
{
    LX_AABB bpos({position.x + AIRSHIP_BOMB_XOFF, position.y + AIRSHIP_BOMB_YOFF,
                  AIRSHIP_BOMB_DIM, AIRSHIP_BOMB_DIM
                 });

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, AIRSHIP_BOMB_ID);

    Engine *g = Engine::getInstance();
    std::array<LX_Vector2D, AIRSHIP_BOMB_NUM> varray;
    BulletPattern::circlePattern(bpos.x, bpos.y, AIRSHIP_BOMB_VEL, varray);

    for(auto it = varray.begin(); it != varray.begin() + (varray.size()/2) +1; ++it)
    {
        g->acceptEnemyMissile(new EnemyBomb(attack_val, spr, bpos, *it));
    }
}

void Airship::fire()
{
    switch(strat)
    {
    case 1:
        bomb();
        break;

    default:
        break;
    }
}

///


void Airship::die()
{
    if(!dying)
    {
        if((position.x + position.w) > 0)
            Engine::getInstance()->screenCancel();
    }

    Enemy::die();
}

Airship::~Airship()
{
    delete poly_hitbox;
}
