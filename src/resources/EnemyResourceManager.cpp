

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
*	@file EnemyResourceManager.cpp
*	@brief The implementation of the enemy ressource manager
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "EnemyResourceManager.hpp"
#include "../xml/XMLReader.hpp"

#include <SDL2/SDL_render.h>
#include <LunatiX/LX_Log.hpp>
#include <LunatiX/LX_FileIO.hpp>
#include <LunatiX/LX_FileBuffer.hpp>
#include <LunatiX/LX_Graphics.hpp>

using namespace std;
using namespace LX_FileIO;
using namespace LX_Graphics;

EnemyResourceManager::EnemyResourceManager()
{
    TX_Asset *asset = TX_Asset::getInstance();
    enemy_resources.fill(nullptr);

    // Load the resources
    for(unsigned int i = 0; i < enemy_resources.size(); i++)
    {
        try
        {
            const char * str = asset->getEnemySpriteFile(i).c_str();
            enemy_resources[i] = new LX_FileBuffer(str);
        }
        catch(IOException& e)
        {
            if(LX_Log::isDebugMode())
            {
                LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                                 "The ennemy resources #%d is unavailable: %s",
                                 i, e.what());
            }
        }
    }
}

/*
    Return a fresh SDL_Texture object. This object is allocated.
    So the user must free it to avoid memory leaks.
*/
SDL_Texture * EnemyResourceManager::loadTextureAt(unsigned int index)
{
    if(index > enemy_resources.size() || enemy_resources[index] == nullptr)
        return nullptr;

    SDL_Surface *s = loadSurfaceFromFileBuffer(enemy_resources[index]);

    if(s == nullptr)
        return nullptr;

    SDL_Texture *t = loadTextureFromSurface(s);
    SDL_FreeSurface(s);
    return t;
}


EnemyResourceManager::~EnemyResourceManager()
{
    // Free the resources
    for(unsigned int i = 0; i < enemy_resources.size(); i++)
    {
        delete enemy_resources[i];
        enemy_resources[i] = nullptr;
    }
}
