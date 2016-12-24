
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
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
//#include "../Bullet.hpp"
#include "../BasicMissile.hpp"
//#include "../../game/Game.hpp"
//#include "../../resources/ResourceManager.hpp"

//#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Physics.hpp>
//#include <LunatiX/LX_Timer.hpp>

using namespace LX_Physics;


Boss02::Boss02(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
               int x, int y, int w, int h, float vx, float vy)
    : Boss(hp, att, sh, image, audio, x, y, w, h, vx, vy)
{
    /// @todo set the core hitbox and the sentinel hitbox
}

void Boss02::fire() {}
void Boss02::strategy() {}

void Boss02::move()
{
    moveCircle(core_hbox,speed);

    for(int i = 0; i< NB_SENTINELS; i++)
        moveCircle(sentinel_hbox[i],speed);

    Enemy::move();
}

void Boss02::collision(Missile *mi)
{
    const LX_AABB& box = *(mi->getHitbox());

    if(collisionCircleRect(hitbox, box))
    {
        if(collisionCircleRect(core_hbox, box))
            reaction(mi);
        else
        {
            for(int i = 0; i< NB_SENTINELS; i++)
            {
                if(collisionCircleRect(sentinel_hbox[i], box))
                {
                    mi->die();
                    break;
                }
            }
        }
    }
}

void Boss02::die() {}

const LX_Physics::LX_Circle * Boss02::getHitbox()
{
    return nullptr;
}

Boss02::~Boss02() {}

