
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

#include "Missile.hpp"
#include "../game/engine/Engine.hpp"
#include "../resources/WinID.hpp"
#include "../asset/TX_Asset.hpp"

#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Log.hpp>

namespace
{
const unsigned int BULLETX_ID = 10;
const unsigned int BULLETX_DIM = 32;
const unsigned int BULLETX_OFF = 4;
const unsigned int BULLETX_DELAY = 128;

LX_Graphics::LX_BufferedImage *bxbuff = nullptr;
}

Missile::Missile(unsigned int pow, unsigned int mul, LX_Graphics::LX_Sprite *image,
                 LX_AABB& rect, LX_Physics::LX_Vector2D& sp)
    : Entity(image, rect, sp), bulletx(nullptr), xplosion(false), bref(0),
      power(pow), multiplier(mul)
{
    const TX_Anima* anima = TX_Asset::getInstance()->getExplosionAnimation(BULLETX_ID);
    LX_Win::LX_Window *w  = LX_Win::getWindowManager()->getWindow(WinID::getWinID());
    bulletx = bxbuff->generateAnimatedSprite(*w, anima->v, anima->delay, true);
    missile_box = {rect.x, rect.y, rect.w, rect.h};

    // A missile that has no graphical repreesntation cannot exist
    if(graphic == nullptr)
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,"missile - No graphical resource: graphic");
}


void Missile::loadExplosionBuffer()
{
    const TX_Asset *a = TX_Asset::getInstance();
    bxbuff = new LX_Graphics::LX_BufferedImage(a->getExplosionSpriteFile(BULLETX_ID));
}

void Missile::destroyExplosionBuffer()
{
    delete bxbuff;
    bxbuff = nullptr;
}


unsigned int Missile::hit() const
{
    return power * multiplier;
}

void Missile::move()
{
    fpos += speed;
    fpos.toPixelUnit(position);
    fpos.toPixelUnit(missile_box);
}

void Missile::die()
{
    if(Engine::outOfBound(position))
        Entity::die();

    if(!xplosion)
    {
        xplosion = true;
        graphic = bulletx;
        position.w = BULLETX_DIM;
        position.h = BULLETX_DIM;
        setX(position.x + BULLETX_OFF);
        normalize(speed);
        bulletx->resetAnimation();
        bref = LX_Timer::getTicks();
    }
    else if((LX_Timer::getTicks() - bref) > BULLETX_DELAY)
        Entity::die();
}

const LX_AABB& Missile::getHitbox() const
{
    return missile_box;
}

bool Missile::explosion() const
{
    return xplosion;
}


Missile::~Missile()
{
    delete bulletx;
    bulletx = nullptr;
}
