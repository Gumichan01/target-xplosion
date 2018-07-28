
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


#include "Bomb.hpp"
#include "../asset/TX_Asset.hpp"
#include "../resources/WinID.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../game/engine/AudioHandler.hpp"

#include <Lunatix/Time.hpp>
#include <Lunatix/Texture.hpp>
#include <Lunatix/WindowManager.hpp>


namespace
{
const int BOMB_MULTIPLIER = 5;
const unsigned int BOMB_LIFETIME = 1000;
const int BOMB_COEF = 3;
lx::Graphics::BufferedImage * xbuff = nullptr;
}

using namespace FloatBox;

Bomb::Bomb( unsigned int pow, lx::Graphics::Sprite * image, lx::Graphics::ImgRect& rect,
            lx::Physics::Vector2D& sp )
    : Missile( pow, BOMB_MULTIPLIER, image, rect, sp ),
      explosion( false ), ref_time( lx::Time::getTicks() ),
      lifetime( BOMB_LIFETIME ), xtexture( nullptr ) {}


void Bomb::loadExplosionBuffer()
{
    const TX_Asset * a = TX_Asset::getInstance();
    xbuff = new lx::Graphics::BufferedImage( a->getExplosionSpriteFile( 0 ) );
}

void Bomb::destroyExplosionBuffer() noexcept
{
    delete xbuff;
    xbuff = nullptr;
}

void Bomb::move() noexcept
{
    // If the bomb has not more life time and have not been exploded
    if ( ( lx::Time::getTicks() - ref_time ) > lifetime )
        die();
    // Explosion
    else if ( explosion )
        EntityHandler::getInstance().bulletCancel();

    Missile::move();
}

bool Bomb::_dieOutOfScreen() noexcept
{
    if ( Engine::outOfBound( phybox ) )
    {
        still_alive = false;
        return true;
    }

    return false;
}

void Bomb::_die() noexcept
{
    if ( !explosion )
    {
        const TX_Anima * const anima = TX_Asset::getInstance()->getExplosionAnimation( 0 );
        lx::Win::Window& w = lx::Win::getWindowManager().getWindow( WinID::getWinID() );
        xtexture = xbuff->generateAnimatedSprite( w, anima->v, anima->delay, true );
        graphic  = xtexture;     // xtexture

        explosion = true;
        phybox.p.x -= fbox( phybox.w );
        phybox.p.y -= fbox( phybox.h );
        phybox.w *= BOMB_COEF;
        phybox.h *= BOMB_COEF;

        normalize( speed );
        ref_time = lx::Time::getTicks();
    }
    else if ( ( lx::Time::getTicks() - ref_time ) > lifetime )
        still_alive = false;
}

void Bomb::die() noexcept
{
    using lx::Graphics::toPixelPosition;
    if ( !_dieOutOfScreen() )
    {
        if ( !explosion )
            AudioHandler::AudioHDL::getInstance()->playExplosion( toPixelPosition( phybox.p ) );

        _die();
    }
}

Bomb::~Bomb()
{
    delete xtexture;
    xtexture = nullptr;
}

/// EnemyBomb

EnemyBomb::EnemyBomb( unsigned int pow, lx::Graphics::Sprite * image, lx::Graphics::ImgRect& rect,
                      lx::Physics::Vector2D& sp )
    : Bomb( pow, image, rect, sp ) {}


void EnemyBomb::move() noexcept
{
    // If the bomb has not more life time and have not been exploded
    if ( ( lx::Time::getTicks() - ref_time ) > lifetime )
        die();

    Missile::move();
}

void EnemyBomb::die() noexcept
{
    if ( !_dieOutOfScreen() )
        _die();
}
