

/*
*	Target_Xplosion - The classic shoot'em up video game
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
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file ResourceManager.hpp
*	@brief Define the ressource handler
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "ResourceManager.hpp"
#include "EnemyResourcesManager.hpp"
#include "MissileResourceManager.hpp"

#include <LunatiX/LX_Log.hpp>
#include <SDL2/SDL_render.h>

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


ResourceManager::ResourceManager()
    : enemy_manager(new EnemyResourcesManager()),
      missile_manager(new MissileResourceManager)
{
    // Empty
}

// Load a specific resource manager
SDL_Texture * ResourceManager::getResource(RESOURCE_TYPE ty,
        unsigned int index)
{
    SDL_Texture * t = nullptr;

    if(ty == RC_ENEMY)
        t = enemy_manager->loadTextureAt(index);
    else if(ty == RC_MISSILE)
        t = missile_manager->loadTextureAt(index);

    return t;
}

ResourceManager::~ResourceManager()
{
    delete enemy_manager;
}


