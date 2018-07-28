
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

#include "ResourceManager.hpp"
#include "../entities/Bomb.hpp"
#include "../entities/Missile.hpp"
#include "../entities/Item.hpp"
#include "../entities/Enemy.hpp"

// Singleton instance
static ResourceManager * rc_singleton = nullptr;


ResourceManager::ResourceManager() : enemy_rc(), missile_rc(), player_rc(),
    sound_rc(), explosion_rc(), menu_rc() {}


void ResourceManager::init()
{
    if ( rc_singleton == nullptr )
        rc_singleton = new ResourceManager();
}

ResourceManager * ResourceManager::getInstance() noexcept
{
    return rc_singleton;
}

void ResourceManager::destroy() noexcept
{
    delete rc_singleton;
    rc_singleton = nullptr;
}


void ResourceManager::loadResources()
{
    Bomb::loadExplosionBuffer();
    Missile::loadExplosionBuffer();
    Item::createItemRessources();
    Enemy::loadExplosionBuffer();
}

void ResourceManager::freeResources() noexcept
{
    Bomb::destroyExplosionBuffer();
    Missile::destroyExplosionBuffer();
    Item::destroyItemRessources();
    Enemy::destroyExplosionBuffer();
}


// Load a specific resource manager
lx::Graphics::Sprite *
ResourceManager::getResource( const RESOURCE_TYPE& ty, unsigned int index ) const noexcept
{
    lx::Graphics::Sprite * t = nullptr;

    if ( ty == RC_ENEMY )
        t = enemy_rc.getTextureAt( index );
    else if ( ty == RC_MISSILE )
        t = missile_rc.getTextureAt( index );
    else if ( ty == RC_XPLOSION )
        t = explosion_rc.getTextureAt( index );

    return t;
}

lx::Graphics::Sprite * ResourceManager::getPlayerResource( bool with_shield ) const noexcept
{
    if ( with_shield )
        return player_rc.getTexture( true );

    return player_rc.getTexture();
}

lx::Graphics::Sprite * ResourceManager::getMenuResource( unsigned int index ) const noexcept
{
    return menu_rc.getImageAt( index );
}

lx::Mixer::Chunk * ResourceManager::getSound( unsigned int index ) const noexcept
{
    return sound_rc.getSoundAt( index );
}
