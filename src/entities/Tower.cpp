
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

#include "../asset/TX_Asset.hpp"
#include "../game/Balance.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/engine/AudioHandler.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Timer.hpp>


using namespace LX_Graphics;
using namespace DynamicGameBalance;

namespace
{
const uint32_t DELAY_TOWER = 500;
const int TOWER_BULLET_ID = 4;
const float TOWER_BULLET_VEL = -7.0f;
}


Tower1::Tower1(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy)
    : Enemy(hp, att, sh, image, x, y, w, h, vx, vy)
{
    strat = new Tower1Strat(this);
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
        LX_AABB box[N] =
        {
            {64,64,64,64}, {130,100,64,64},
            {60,232,64,64}, {60,120,64,64}, {150,80,64,64},
            {130,160,64,64}, {100,256,64,64}
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

void Tower1::fire()
{
    const int N = 9;
    LX_AABB rect[2] =
    {
        {position.x, position.y + 125, 24, 20},
        {position.x, position.y + 160, 24, 20}
    };

    if(isDead())
        return;

    float v = apply_dgb(TOWER_BULLET_VEL);
    LX_Physics::LX_Vector2D velocity[N] =
    {
        {v, 0.0f}, {v, -1.0f}, {v, 1.0f}, {v, -2.0f}, {v, 2.0f},
        {v, -3.0f}, {v, 3.0f}, {v, -4.0f}, {v, 4.0f}
    };

    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Sprite *spr = rc->getResource(RC_MISSILE, TOWER_BULLET_ID);

    for(int i = 0; i < N; i++)
    {
        g->acceptEnemyMissile(new Bullet(attack_val, spr, rect[0], velocity[i]));
        g->acceptEnemyMissile(new Bullet(attack_val, spr, rect[1], velocity[i]));
    }
}

void Tower1::die()
{
    if(!dying)
    {
        if((position.x + position.w) > 0)
            Engine::getInstance()->bulletCancel();
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
