
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

#define FLA(x)  static_cast<float>(x)

using namespace LX_Physics;

namespace
{
const int AIRSHIP_WIDTH = 250;
const int AIRSHIP_HEIGHT = 100;

const int AIRSHIP_FRONT_XPOS = 1000;
const int AIRSHIP_FRONT_YPOS = 255;
const int AIRSHIP_BOTTOM_YPOS = 261;
const float AIRSHIP_DIV = 4.0f;

// Bomb
const int AIRSHIP_BOMB_ID = 2;
const int AIRSHIP_BOMB_XOFF = 124;
const int AIRSHIP_BOMB_YOFF = 76;
const int AIRSHIP_BOMB_DIM = 16;
const int AIRSHIP_BOMB_VEL = 10;
const int AIRSHIP_BOMB_NUM = CIRCLE_BULLETS/2;
const uint32_t AIRSHIP_BOMB_DELAY = 500;

// Front shot
const int AIRSHIP_FSHOT_ID = 4;
const int AIRSHIP_FSHOT_XOFF = 16;
const int AIRSHIP_FSHOT_YOFF = 32;
const int AIRSHIP_FSHOT_W = 32;
const int AIRSHIP_FSHOT_H = 12;
const int AIRSHIP_FSHOT_VEL = 10;
const int AIRSHIP_FSHOT_NUM = CIRCLE_BULLETS *2;
const uint32_t AIRSHIP_FSHOT_DELAY = 500;

// Spin
const int AIRSHIP_SPIN_ID = 8; // next 6
const int AIRSHIP_SPIN_XOFF = 124;
const int AIRSHIP_SPIN_YOFF = 76;
const int AIRSHIP_SPIN_DIM = 24;
const int AIRSHIP_SPIN_VEL = 9;
const int AIRSHIP_SPIN_NUM = CIRCLE_BULLETS/2;
const uint32_t AIRSHIP_SPIN_DELAY = 100;
const float AIRSHIP_STEP = BulletPattern::PI_F/24.0f;
const float AIRSHIP_RF = 100.0f;
}


Airship::Airship(unsigned int hp, unsigned int att, unsigned int sh,
                 LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                 float vx, float vy)
    : Enemy(hp, att, sh, image, x, y, w, h, vx, vy), idstrat(0), alpha1(0.0f),
      alpha2(BulletPattern::PI_F),
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
    ShotStrategy *shot = nullptr;

    if(position.y < AIRSHIP_FRONT_YPOS)
    {
        idstrat = 1;
        shot = new ShotStrategy(this);
        shot->setShotDelay(AIRSHIP_BOMB_DELAY);
    }
    else if(position.y > AIRSHIP_BOTTOM_YPOS)
    {
        idstrat = 2;
        shot = new ShotStrategy(this);
        shot->setShotDelay(AIRSHIP_FSHOT_DELAY);
    }
    else
        idstrat = 3;

    getMVSStrat()->addShotStrat(shot);
}

void Airship::aposition()
{
    if(position.x <= AIRSHIP_FRONT_XPOS)
    {
        idstrat = 4;
        speed *= 0.0f;
        ShotStrategy *shot = new ShotStrategy(this);
        shot->setShotDelay(AIRSHIP_SPIN_DELAY);
        getMVSStrat()->addShotStrat(shot);
    }
}

void Airship::strategy()
{
    switch(idstrat)
    {
    case 0:
        prepare();
        break;

    case 3:
        aposition();
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

void Airship::frontShot()
{
    LX_AABB fpos({position.x + AIRSHIP_FSHOT_XOFF, position.y + AIRSHIP_FSHOT_YOFF,
                  AIRSHIP_FSHOT_W, AIRSHIP_FSHOT_H
                 });

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, AIRSHIP_FSHOT_ID);

    Engine *g = Engine::getInstance();
    std::array<LX_Vector2D, AIRSHIP_FSHOT_NUM> varray;
    BulletPattern::circlePattern(fpos.x, fpos.y, AIRSHIP_FSHOT_VEL, varray);

    const auto _beg = varray.begin() + varray.size() - varray.size() / 4;
    const auto _end = varray.begin() + varray.size() / 4;

    for(auto it = _beg; it != _end; ++it)
    {
        if(it == varray.end())
            it = varray.begin();

        g->acceptEnemyMissile(new Bullet(attack_val, spr, fpos, *it));
    }
}

void Airship::doubleSpinShot()
{
    LX_Vector2D v1, v2;

    using namespace LX_Graphics;
    const LX_Point p(position.x + AIRSHIP_SPIN_XOFF, position.y + AIRSHIP_SPIN_YOFF);
    LX_AABB mbrect = {p.x, p.y, AIRSHIP_SPIN_DIM, AIRSHIP_SPIN_DIM};
    LX_Sprite *bsp = ResourceManager::getInstance()->getResource(RC_MISSILE, AIRSHIP_SPIN_ID);

    BulletPattern::shotOnTarget(p.x, p.y, FLA(p.x) + cosf(alpha1) * AIRSHIP_RF,
                                FLA(p.y) - sinf(alpha1) * AIRSHIP_RF,
                                AIRSHIP_SPIN_VEL, v1);

    BulletPattern::shotOnTarget(p.x, p.y, FLA(p.x) + cosf(alpha2) * AIRSHIP_RF,
                                FLA(p.y) - sinf(alpha2) * AIRSHIP_RF,
                                AIRSHIP_SPIN_VEL, v2);

    if(alpha1 == BulletPattern::PI_F * 2.0f)
        alpha1 = 0.0f;
    else
        alpha1 += AIRSHIP_STEP;

    if(alpha2 == BulletPattern::PI_F * 2.0f)
        alpha2 = 0.0f;
    else
        alpha2 += AIRSHIP_STEP;

    Engine *e = Engine::getInstance();
    e->acceptEnemyMissile(new SpinBullet(attack_val, bsp, mbrect, v1));
    e->acceptEnemyMissile(new SpinBullet(attack_val, bsp, mbrect, v2));
}

void Airship::fire()
{
    switch(idstrat)
    {
    case 1:
        bomb();
        break;

    case 2:
        frontShot();
        break;

    case 4:
        doubleSpinShot();
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
