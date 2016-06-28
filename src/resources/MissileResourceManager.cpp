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
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file MissileResourceManager.cpp
*	@brief The implementation of the missile ressource manager
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "MissileResourceManager.hpp"

#include <SDL2/SDL_render.h>
#include <LunatiX/LX_Log.hpp>
#include <LunatiX/LX_Graphics.hpp>


using namespace LX_Graphics;

MissileResourceManager::MissileResourceManager()
{
    TX_Asset *asset = TX_Asset::getInstance();
    missile_resources.fill(nullptr);
    unsigned int j = PLAYER_MISSILES;

    for(unsigned int i = 0; i < missile_resources.size(); i++)
    {
        const char * str = nullptr;
        if(i < PLAYER_MISSILES)
            str = asset->getPlayerMissilesFile(i).c_str();
        else
            str = asset->getEnemyMissilesFile(i-j).c_str();

        missile_resources[i] = loadTextureFromFile(str);
    }
}

/*
    Return a fresh SDL_Texture object. This object is allocated.
    So the user must free it to avoid memory leaks.
*/
SDL_Texture * MissileResourceManager::getTextureAt(unsigned int index)
{
    if(index > missile_resources.size() || missile_resources[index] == nullptr)
        return nullptr;

    return missile_resources[index];
}


MissileResourceManager::~MissileResourceManager()
{
    // Free the resources
    for(unsigned int i = 0; i < missile_resources.size(); i++)
    {
        if(missile_resources[i] != nullptr)
            SDL_DestroyTexture(missile_resources[i]);
    }
}
