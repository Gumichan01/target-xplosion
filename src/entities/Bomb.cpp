
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
#include "../asset/TX_Asset.hpp"
#include "../resources/WinID.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/engine/AudioHandler.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Graphics.hpp>


namespace
{
const int BOMB_MULTIPLIER = 5;
const uint32_t BOMB_LIFETIME = 1000;
const uint32_t BOMB_COEF = 3;
const float BOMB_DIV = 2.0f;
LX_Graphics::LX_BufferedImage *xbuff = nullptr;
}


Bomb::Bomb(unsigned int pow, LX_Graphics::LX_Sprite *image, LX_AABB& rect,
           LX_Physics::LX_Vector2D& sp)
    : Missile(pow, BOMB_MULTIPLIER, image, rect, sp),
      explosion(false), ref_time(LX_Timer::getTicks()),
      lifetime(BOMB_LIFETIME), xtexture(nullptr) {}


void Bomb::loadExplosionBuffer()
{
    const TX_Asset *a = TX_Asset::getInstance();
    xbuff = new LX_Graphics::LX_BufferedImage(a->getExplosionSpriteFile(0));
}

void Bomb::destroyExplosionBuffer()
{
    delete xbuff;
    xbuff = nullptr;
}

void Bomb::move()
{
    // If the bomb has not more life time and have not been exploded
    if((LX_Timer::getTicks() - ref_time) > lifetime)
        die();

    else if(!explosion)
        Missile::move();

    else    // Explosion
        Engine::getInstance()->screenCancel();

    //Missile::move();
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
        const TX_Anima* anima = TX_Asset::getInstance()->getExplosionAnimation(0);
        LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(WinID::getWinID());
        xtexture = xbuff->generateAnimatedSprite(*w, anima->v, anima->delay, true);
        graphic = xtexture;     // xtexture

        if(position.x < Engine::getMaxXlim()-1)
            Engine::getInstance()->screenCancel();

        explosion = true;
        position.w *= BOMB_COEF;
        position.h *= BOMB_COEF;
        position.x -= position.w/3;
        position.y -= position.h/3;
        missile_box = position;
        //speed /= BOMB_DIV;

        ref_time = LX_Timer::getTicks();
        AudioHandler::AudioHDL::getInstance()->playExplosion();
    }
    else if((LX_Timer::getTicks() - ref_time) > lifetime)
        Missile::die();
}

Bomb::~Bomb()
{
    delete xtexture;
    xtexture = nullptr;
}
