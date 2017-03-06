
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
#include "../Player.hpp"

#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Log.hpp>

namespace
{
const int GLOBAL_XOFFSET = 48;
const int GLOBAL_YOFFSET = 8;
const int GLOBAL_BOXWIDTH = 448;
const int GLOBAL_BOXHEIGHT = 256;   // or 248
};

using namespace LX_Physics;

Boss02::Boss02(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
               int x, int y, int w, int h, float vx, float vy)
    : Boss(hp, att, sh, image, audio, x, y, w, h, vx, vy),
    global_hitbox({x + GLOBAL_XOFFSET, y + GLOBAL_YOFFSET, GLOBAL_BOXWIDTH, GLOBAL_BOXHEIGHT})
{
    addStrategy(new MoveStrategy(this));
}


//void Boss02::fire() {}
void Boss02::move()
{
    moveRect(global_hitbox,speed);
    Boss::move();
}

void Boss02::collision(Missile *mi)
{

}

void Boss02::collision(Player *play)
{
    if(collisionCircleRect(*(play->getHitbox()), global_hitbox))
    {
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"collision player/boss");
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"p {(%d, %d), %d} | r {%d, %d, %d, %d}",
                         play->getHitbox()->center.x, play->getHitbox()->center.y,
                         play->getHitbox()->radius, global_hitbox.x, global_hitbox.y,
                         global_hitbox.w, global_hitbox.h);
        //play->die();
    }
}

Boss02::~Boss02() {}
