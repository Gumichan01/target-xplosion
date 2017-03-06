
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
#include "../../pattern/Strategy.hpp"
#include "../Missile.hpp"
#include "../Player.hpp"

#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Polygon.hpp>
#include <LunatiX/LX_Log.hpp>

#include <vector>
#include <algorithm>

using namespace LX_Physics;

namespace
{
const int GLOBAL_XOFFSET = 48;
const int GLOBAL_YOFFSET = 8;
const int GLOBAL_BOXWIDTH = 448;
const int GLOBAL_BOXHEIGHT = 256;   // or 248
};



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

    std::for_each(hpoints.begin(), hpoints.end(), [x,y](LX_Point& p)
    {
        p.x += x;
        p.y += y;
    });

    // todo: update Lunatix
    std::for_each(hpoints.begin(), hpoints.end(), [this](const LX_Point& p)
    {
        poly->addPoint(p);
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Added point (%d,%d)", p.x, p.y);
    });
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
        /*LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"collision missile/boss");
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"m {%d, %d, %d, %d} | r {%d, %d, %d, %d}",
                         mi->getHitbox()->x, mi->getHitbox()->y,
                         mi->getHitbox()->w, mi->getHitbox()->h,
                         global_hitbox.x, global_hitbox.y,
                         global_hitbox.w, global_hitbox.h);*/

        if(collisionRectPoly(*(mi->getHitbox()), *poly))
        {
            LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"collision missile/boss");
            LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"m {%d, %d, %d, %d}",
                             mi->getHitbox()->x, mi->getHitbox()->y,
                             mi->getHitbox()->w, mi->getHitbox()->h);
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
