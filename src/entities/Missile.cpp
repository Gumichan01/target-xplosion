
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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
#include "boss/Boss02.hpp"
#include "../game/engine/Engine.hpp"
#include "../resources/WinID.hpp"
#include "../asset/TX_Asset.hpp"

#include <Lunatix/Physics.hpp>
#include <Lunatix/Texture.hpp>
#include <Lunatix/WindowManager.hpp>
#include <Lunatix/Time.hpp>
#include <Lunatix/Log.hpp>

namespace
{
const unsigned int BULLETX_ID = 10;
const unsigned int BULLETX_DIM = 32;
const Float BULLETX_OFF = {4.0f};
const unsigned int BULLETX_DELAY = 128;

lx::Graphics::BufferedImage * bxbuff = nullptr;
}

Missile::Missile( unsigned int pow, unsigned int mul, lx::Graphics::Sprite * image,
                  lx::Graphics::ImgRect& rect, lx::Physics::Vector2D& sp )
    : Entity( image, rect, sp ), bulletx( nullptr ), xplosion( false ), mref( 0 ),
      power( pow ), multiplier( mul )
{
    const TX_Anima * const anima = TX_Asset::getInstance()->getExplosionAnimation( BULLETX_ID );
    lx::Win::Window& w = lx::Win::getWindowManager().getWindow( WinID::getWinID() );
    bulletx = bxbuff->generateAnimatedSprite( w, anima->v, anima->delay, true );

    // A missile that has no graphical repreesntation cannot exist
    if ( graphic == nullptr )
        lx::Log::logError( lx::Log::LogType::APPLICATION, "missile - No graphical resource: missile" );
}


void Missile::loadExplosionBuffer()
{
    const TX_Asset * a = TX_Asset::getInstance();
    bxbuff = new lx::Graphics::BufferedImage( a->getExplosionSpriteFile( BULLETX_ID ) );
}

void Missile::destroyExplosionBuffer() noexcept
{
    delete bxbuff;
    bxbuff = nullptr;
}

void Missile::accept( Boss02& v )
{
    v.visit( *this );
}

unsigned int Missile::hit() const noexcept
{
    return power * multiplier;
}

void Missile::move() noexcept
{
    lx::Physics::moveBox( phybox, speed );
}

void Missile::die() noexcept
{
    if ( Engine::outOfBound( phybox ) )
        still_alive = false;

    if ( !xplosion )
    {
        xplosion = true;
        graphic = bulletx;
        phybox.w = BULLETX_DIM;
        phybox.h = BULLETX_DIM;
        setX( phybox.p.x + BULLETX_OFF );
        normalize( speed );
        bulletx->resetAnimation();
        mref = lx::Time::getTicks();
    }
    else if ( ( lx::Time::getTicks() - mref ) > BULLETX_DELAY )
        still_alive = false;
}

const lx::Physics::FloatingBox& Missile::getHitbox() const noexcept
{
    return phybox;
}

bool Missile::explosion() const noexcept
{
    return xplosion;
}


Missile::~Missile()
{
    delete bulletx;
    bulletx = nullptr;
}
