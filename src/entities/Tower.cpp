
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

#include "../game/engine/Engine.hpp"
#include "../asset/TX_Asset.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Timer.hpp>


const uint32_t DELAY_TOWER = 500;


Tower1::Tower1(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Chunk *audio,
               int x, int y, int w, int h, float vx, float vy)
    : Enemy(hp, att, sh, image, audio, x, y, w, h, vx, vy)
{
    strat = new Tower1Strat(this);
}


void Tower1::fire()
{
    const int BULLET_VEL = -7;
    const int N = 9;

    LX_AABB rect[2] = {{position.x, position.y + 125, 24, 20},
        {position.x, position.y + 160, 24, 20}
    };

    if(isDead())
        return;

    LX_Physics::LX_Vector2D velocity[] = {{BULLET_VEL, 0}, {BULLET_VEL, -1},
        {BULLET_VEL, 1}, {BULLET_VEL, -2}, {BULLET_VEL, 2}, {BULLET_VEL, -3},
        {BULLET_VEL, 3}, {BULLET_VEL, -4}, {BULLET_VEL, 4}
    };

    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();

    for(int i = 0; i < N; i++)
    {
        g->acceptEnemyMissile(new Bullet(attack_val, rc->getResource(RC_MISSILE, 4),
                                         nullptr, rect[0], velocity[i]));
        g->acceptEnemyMissile(new Bullet(attack_val, rc->getResource(RC_MISSILE, 4),
                                         nullptr, rect[1], velocity[i]));
    }
}


void Tower1::die()
{
    if((position.x + position.w) > 0)
        Engine::getInstance()->screenCancel();

    Entity::die();
}


Tower1::~Tower1() {}


Tower1Strat::Tower1Strat(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    reference_time = 0;
}


Tower1Strat::~Tower1Strat() {}


void Tower1Strat::proceed()
{
    if((LX_Timer::getTicks() - reference_time) > DELAY_TOWER)
    {
        fire(ROCKET_TYPE);
        reference_time = LX_Timer::getTicks();
    }
    target->move();
}


void Tower1Strat::fire(const MISSILE_TYPE& m_type)
{
    if(m_type == ROCKET_TYPE)
    {
        target->fire();
    }
}
