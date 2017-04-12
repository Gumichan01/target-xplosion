
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


#include "Bomb.hpp"
#include "../game/engine/Engine.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Audio.hpp>
#include <LunatiX/LX_Timer.hpp>

/// @todo (#1#) v0.4.7: bomb use (clean the screen!!!)
/*
    When the bomb is deployed it can be used during a moment
    (~ 10-30 seconds)
*/

namespace
{
const int BOMB_MULTIPLIER = 4;
const uint32_t BOMB_LIFETIME = 900;
LX_Graphics::LX_Sprite *explosion_texture = nullptr;
}


Bomb::Bomb(unsigned int pow, LX_Graphics::LX_Sprite *image,
           LX_Mixer::LX_Chunk *audio, LX_AABB& rect,
           LX_Physics::LX_Vector2D& sp)
    : Missile(pow, BOMB_MULTIPLIER, image, audio, rect, sp), explosion(false),
      ref_time(LX_Timer::getTicks()), lifetime(BOMB_LIFETIME) {}


void Bomb::loadExplosionBuffer()
{
    const ResourceManager *rc = ResourceManager::getInstance();
    explosion_texture = rc->getResource(RC_XPLOSION, 0);
}


void Bomb::move()
{
    //if the bomb has not more life time and have not been exploded
    if((LX_Timer::getTicks() - ref_time) > lifetime)
    {
        die();
    }
    else if(!explosion)
        Missile::move();
}


void Bomb::die()
{
    if(position.x <= (-(position.w)) || position.x > Engine::getMaxXlim())
    {
        Missile::die();
        return;
    }

    // If no explosion occured
    if(!explosion)
    {
        graphic = explosion_texture;

        if(position.x < Engine::getMaxXlim()-1)
            Engine::getInstance()->screenCancel();

        explosion = true;
        position.x += position.w/2 - EXPLOSION_WIDTH/2;
        position.y += position.h/2 - EXPLOSION_WIDTH/2;
        position.w = EXPLOSION_WIDTH;
        position.h = EXPLOSION_HEIGHT;
        missile_box = position;
        missile_box.w = EXPLOSION_WIDTH * 2;
        missile_box.h = EXPLOSION_HEIGHT * 2,

        ref_time = LX_Timer::getTicks();

        if(sound != nullptr)
            sound->play();
    }
    else if((LX_Timer::getTicks() - ref_time) > lifetime)
        Missile::die();
}
