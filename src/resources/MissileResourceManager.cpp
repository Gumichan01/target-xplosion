
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
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "MissileResourceManager.hpp"
#include "../asset/TX_Asset.hpp"
#include "../resources/WinID.hpp"

#include <Lunatix/Graphics.hpp>

namespace
{
std::array<lx::Graphics::Sprite *, Asset::NB_MISSILES> missile_resources;
}

MissileResourceManager::MissileResourceManager()
{
    using namespace lx::Graphics;
    lx::Win::Window& w = lx::Win::getWindowManager().getWindow( WinID::getWinID() );
    const TX_Asset * const ASSET = TX_Asset::getInstance();

    missile_resources.fill( nullptr );

    for ( unsigned int i = 0; i < missile_resources.max_size(); i++ )
    {
        std::string str;
        const TX_Anima * const anima = ASSET->getMissileAnimation( i );
        str = ASSET->getMissileFile( i );

        if ( !str.empty() )
        {
            using namespace lx::Graphics;
            if ( anima != nullptr && anima->delay != 0 )
                missile_resources[i] = new AnimatedSprite( str, w, anima->v, anima->delay, true );
            else
                missile_resources[i] = new Sprite( str, w );
        }
    }
}

lx::Graphics::Sprite * MissileResourceManager::getTextureAt( unsigned int index ) const noexcept
{
    if ( index > missile_resources.size() )
        return nullptr;

    return missile_resources[index];
}

MissileResourceManager::~MissileResourceManager()
{
    // Free the resources
    for ( unsigned int i = 0; i < missile_resources.size(); i++ )
    {
        delete missile_resources[i];
    }
}
