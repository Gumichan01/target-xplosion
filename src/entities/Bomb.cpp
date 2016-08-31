
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
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/


#include "Bomb.hpp"
#include "../game/Game.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Audio.hpp>
#include <LunatiX/LX_Timer.hpp>

namespace
{
const double ANIMATION_DELAY = 125;
const uint32_t BOMB_LIFETIME = 875;
const uint32_t BOMB_XPLOSION_W = 94;
const uint32_t BOMB_XPLOSION_H = 94;
const uint32_t BOMB_SPRITE_DELAY = 125;

LX_Graphics::LX_Sprite *explosion_texture = nullptr;
}


Bomb::Bomb(unsigned int pow, LX_Graphics::LX_Sprite *image,
           LX_Mixer::LX_Sound *audio, LX_AABB& rect,
           LX_Physics::LX_Vector2D& sp)
    : Missile(pow, 4, image, audio, rect, sp),explosion(false),
      ref_time(LX_Timer::getTicks()),lifetime(BOMB_LIFETIME) {}


void Bomb::loadExplosionBuffer(void)
{
    ResourceManager *rc = ResourceManager::getInstance();
    explosion_texture = rc->getResource(RC_XPLOSION,0);
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
    if(position.x <= (-(position.w)) || position.x > Game::getXlim())
        Missile::die();

    // If no explosion occured
    if(!explosion)
    {
        graphic = explosion_texture;

        if(position.x < Game::getXlim()-1)
            Game::getInstance()->screenCancel();

        explosion = true;
        position.x -= BOMB_WIDTH /2;
        position.y -= BOMB_HEIGHT /2;
        position.w = BOMB_WIDTH *2 + 1;
        position.h = BOMB_HEIGHT *2 + 1;
        ref_time = LX_Timer::getTicks();

        if(sound != nullptr)
            sound->play();
    }
    else if((LX_Timer::getTicks() - ref_time) > lifetime)
        Missile::die();
}

