
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

#include "EnemyResourceManager.hpp"
#include "../asset/TX_Asset.hpp"
#include "../resources/WinID.hpp"

#include <Lunatix/Graphics.hpp>
#include <Lunatix/Log.hpp>
#include <unordered_map>


namespace
{
std::unordered_map<size_t, lx::Graphics::Sprite *> enemy_resources;
}

EnemyResourceManager::EnemyResourceManager()
{
    using namespace lx::Graphics;
    std::string prev_string( "" );
    const TX_Asset * const asset = TX_Asset::getInstance();
    lx::Win::Window& w  = lx::Win::getWindowManager().getWindow( WinID::getWinID() );

    // Load the resources
    for ( size_t i = 0; i < Asset::NB_ENEMIES; ++i )
    {
        const std::string& str = asset->getEnemySpriteFile( i );
        const TX_Anima * const anima  = asset->getEnemyAnimation( i );

        if ( !str.empty() )
        {
            // If the string is the same as the previous → same texture
            if ( str == prev_string && ( anima == nullptr || anima->delay == 0 ) )
            {
                enemy_resources[i] = i > 0 ? enemy_resources[i - 1] : nullptr;
            }
            else if ( anima != nullptr )
            {
                if ( anima->delay != 0 )
                    enemy_resources[i] = new AnimatedSprite( str, w, anima->v,
                            anima->delay, true );
                else
                {
                    const lx::Graphics::ImgRect& rect = anima->v.at( 0 );
                    enemy_resources[i] = new Sprite( str, w, rect );
                }
            }
            else
                enemy_resources[i] = new Sprite( str, w );
        }

        prev_string = str;
    }
}

lx::Graphics::Sprite * EnemyResourceManager::getTextureAt( unsigned int index ) const noexcept
{
    if ( enemy_resources.find( index ) == enemy_resources.cend() )
        return nullptr;

    return enemy_resources[index];
}

EnemyResourceManager::~EnemyResourceManager()
{
    // Free the resources
    const auto N = Asset::NB_ENEMIES;
    const auto CEND = enemy_resources.cend();

    for ( auto i = 0UL; i < N; ++i )
    {
        if ( enemy_resources.find( i ) != CEND && enemy_resources[i] != nullptr )
        {
            unsigned int j = i + 1;

            while ( j < N && enemy_resources.find( j ) != CEND
                    && enemy_resources[j] == enemy_resources[i] )
            {
                enemy_resources[j++] = nullptr;
            }

            delete enemy_resources[i];
            i = j - 1;
        }
    }
}
