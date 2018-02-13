
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

#include "Tower.hpp"
#include "Bullet.hpp"
#include "Player.hpp"

#include "../asset/TX_Asset.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../game/engine/AudioHandler.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Timer.hpp>

#include <algorithm>

using namespace LX_Graphics;

namespace
{
const uint32_t DELAY_TOWER = 500;
const int TOWER_BULLET_ID = 4;

using LX_Physics::LX_Point;

const std::vector<LX_Point> HPOINTS
{
    LX_Point(119,43), LX_Point(193,90),
    LX_Point(218,84), LX_Point(191,106),
    LX_Point(164,175), LX_Point(191,270),
    LX_Point(230,275), LX_Point(230,397),
    LX_Point(6,397), LX_Point(6,275),
    LX_Point(45,270), LX_Point(68,175),
    LX_Point(42,106), LX_Point(24,84), LX_Point(48,90)
};
}


Tower1::Tower1(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy)
    : LargeEnemy(hp, att, sh, image, x, y, w, h, vx, vy),
      main_hitbox(), shape(HPOINTS, LX_Physics::LX_Point{x,y})
{
    main_hitbox = {position.x, position.y, position.y, position.h};
    addStrategy(new Tower1Strat(this));
}


void Tower1::move()
{
    LX_Physics::moveRect(main_hitbox,speed);
    LX_Physics::movePoly(shape.getPoly(), speed);
    Enemy::move();
}


void Tower1::collision(Missile *mi)
{
    if(!mi->isDead() && !mi->explosion() && mi->getX() <= (position.x + position.w) && !dying)
    {
        if(LX_Physics::collisionRect(main_hitbox, mi->getHitbox()))
        {
            if(LX_Physics::collisionRectPoly(mi->getHitbox(), shape.getPoly()))
            {
                if(destroyable) reaction(mi);
                mi->die();
            }
        }
    }
}

void Tower1::collision(Player *play)
{
    if(play->getX() <= (position.x + position.w) && !dying)
    {
        if(LX_Physics::collisionCircleRect(play->getHitbox(), main_hitbox))
        {
            if(LX_Physics::collisionCirclePoly(play->getHitbox(), shape.getPoly()))
            {
                play->die();
            }
        }
    }
}


void Tower1::boom()
{
    AudioHandler::AudioHDL::getInstance()->playMediumExplosion();
}


void Tower1::draw()
{
    if(dying)
    {
        const int N = 7;
        LX_AABB box[N] = {{64,64,64,64}, {130,100,64,64},
            {60,232,64,64}, {60,120,64,64}, {150,80,64,64},
            {130,160,64,64}, {100,256,64,64},
        };

        for(int i = 0; i < N; i++)
        {
            box[i].x += position.x;
            box[i].y += position.y;
            graphic->draw(&box[i]);
        }
    }
    else
        LargeEnemy::draw();
}

void Tower1::fire()
{
    const float BULLET_VEL = -7.0f;

    LX_AABB rect[2] = {{position.x + 40, position.y + 145, 24, 20},
        {position.x + 40, position.y + 185, 24, 20}
    };

    if(isDead())
        return;

    LX_Physics::LX_Vector2D velocity[] = {{BULLET_VEL, 0.0f}, {BULLET_VEL, -1.0f},
        {BULLET_VEL, 1.0f}, {BULLET_VEL, -2.0f}, {BULLET_VEL, 2.0f},
        {BULLET_VEL, -3.0f}, {BULLET_VEL, 3.0f}, {BULLET_VEL, -4.0f},
        {BULLET_VEL, 4.0f}
    };

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Sprite *spr = rc->getResource(RC_MISSILE, TOWER_BULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    for(LX_Physics::LX_Vector2D& v : velocity)
    {
        hdl.pushEnemyMissile(*(new Bullet(attack_val, spr, rect[0], v)));
        hdl.pushEnemyMissile(*(new Bullet(attack_val, spr, rect[1], v)));
    }
}

void Tower1::die()
{
    if(!dying)
    {
        if((position.x + position.w) > 0)
            EntityHandler::getInstance().bulletCancel();
    }

    Enemy::die();
}


// Strategy

Tower1Strat::Tower1Strat(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    reference_time = 0;
}

void Tower1Strat::proceed()
{
    if((LX_Timer::getTicks() - reference_time) > DELAY_TOWER)
    {
        target->fire();
        reference_time = LX_Timer::getTicks();
    }
    target->move();
}
