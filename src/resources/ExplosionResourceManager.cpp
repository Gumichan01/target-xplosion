
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

#include "ExplosionResourceManager.hpp"
#include "../asset/TX_Asset.hpp"
#include "../resources/WinID.hpp"

#include <Lunatix/Graphics.hpp>
#include <array>

namespace
{
std::array<lx::Graphics::Sprite *, Asset::NB_XPLOSION> explosion_resources;
}


ExplosionResourceManager::ExplosionResourceManager()
{
    using namespace lx::Graphics;
    const TX_Asset * const asset = TX_Asset::getInstance();
    lx::Win::Window& w = lx::Win::getWindowManager().getWindow( WinID::getWinID() );
    explosion_resources.fill( nullptr );

    for ( unsigned int i = 0; i < explosion_resources.size(); i++ )
    {
        std::string str = asset->getExplosionSpriteFile( i );
        const TX_Anima * const anima = asset->getExplosionAnimation( i );

        if ( anima != nullptr )
            explosion_resources[i] = new AnimatedSprite( str, w, anima->v, anima->delay, true );
        else
        {
            if ( !str.empty() )
                explosion_resources[i] = new Sprite( str, w );
        }
    }
}

lx::Graphics::Sprite * ExplosionResourceManager::getTextureAt( unsigned int index ) const noexcept
{
    if ( index > explosion_resources.size() || explosion_resources[index] == nullptr )
        return nullptr;

    return explosion_resources[index];
}

ExplosionResourceManager::~ExplosionResourceManager()
{
    // Free the resources
    for ( unsigned int i = 0; i < explosion_resources.size(); i++ )
    {
        delete explosion_resources[i];
    }
}
