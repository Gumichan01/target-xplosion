

/*
*	Target_Xplosion - A classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Luxon Jean-Pierre (Gumichan01)
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

#include "ResourceManager.hpp"


// Singleton instance
static ResourceManager *rc_singleton = nullptr;


void ResourceManager::init()
{
    if(rc_singleton == nullptr)
        rc_singleton = new ResourceManager();
}

ResourceManager * ResourceManager::getInstance()
{
    return rc_singleton;
}

void ResourceManager::destroy()
{
    delete rc_singleton;
    rc_singleton = nullptr;
}

ResourceManager::ResourceManager() {}


// Load a specific resource manager
LX_Graphics::LX_Sprite * ResourceManager::getResource(const RESOURCE_TYPE& ty,
                                                      unsigned int index)
{
    LX_Graphics::LX_Sprite * t = nullptr;

    if(ty == RC_ENEMY)
        t = enemy_rc.getTextureAt(index);
    else if(ty == RC_MISSILE)
        t = missile_rc.getTextureAt(index);
    else if(ty == RC_XPLOSION)
        t = explosion_rc.getTextureAt(index);

    return t;
}

LX_Graphics::LX_Sprite * ResourceManager::getPlayerResource(bool with_shield)
{
    if(with_shield)
        return player_rc.getTexture(true);

    return player_rc.getTexture();
}

LX_Mixer::LX_Sound * ResourceManager::getSound(unsigned int index)
{
    return sound_rc.getSoundAt(index);
}
