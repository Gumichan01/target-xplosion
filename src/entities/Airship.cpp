
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
#include "Missile.hpp"
#include "../pattern/Strategy.hpp"

#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Polygon.hpp>
#include <LunatiX/LX_Log.hpp>
#include <vector>

using namespace LX_Physics;

namespace
{
const int AIRSHIP_WIDTH = 250;
const int AIRSHIP_HEIGHT = 100;
}


Airship::Airship(unsigned int hp, unsigned int att, unsigned int sh,
                 LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                 float vx, float vy)
    : Enemy(hp, att, sh, image, x, y, w, h, vx, vy),
    main_hitbox({position.x, position.y, AIRSHIP_WIDTH, AIRSHIP_HEIGHT})
{
    std::vector<LX_Point> hpoints {LX_Point(12,38), LX_Point(24,18),
                                   LX_Point(120,6), LX_Point(222,18),LX_Point(248,38), LX_Point(222,64),
                                   LX_Point(184,70), LX_Point(156,96), LX_Point(61,96), LX_Point(45,68),
                                   LX_Point(24,58)
                                  };

    addStrategy(new MoveStrategy(this));
    poly_hitbox = new LX_Polygon();

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


Airship::~Airship()
{
    delete poly_hitbox;
}
