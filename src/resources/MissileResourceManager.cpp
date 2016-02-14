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
*	@file MissileResourceManager.cpp
*	@brief The implementation of the missile ressource manager
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "MissileResourceManager.hpp"
#include "../xml/XMLReader.hpp"

#include <SDL2/SDL_render.h>
#include <LunatiX/LX_Log.hpp>
#include <LunatiX/LX_FileIO.hpp>
#include <LunatiX/LX_FileBuffer.hpp>
#include <LunatiX/LX_Graphics.hpp>


using namespace std;
using namespace LX_FileIO;
using namespace LX_Graphics;

MissileResourceManager::MissileResourceManager()
{
    TX_Asset *asset = TX_Asset::getInstance();
    missile_resources.fill(nullptr);
    int j = PLAYER_MISSILES;

    for(unsigned int i = 0; i < missile_resources.size(); i++)
    {
        const char * str = nullptr;
        if(i < PLAYER_MISSILES)
            str = asset->getPlayerMissilesFile(i).c_str();
        else
            str = asset->getEnemyMissilesFile(i-j).c_str();

        try
        {
            missile_resources[i] = new LX_FileBuffer(str);
        }
        catch(IOException& e)
        {
            LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                             "The missile resource #%d is unavailable.",i+1);
        }
    }
}

/*
    Return a fresh SDL_Texture object. This object is allocated.
    So the user must free it to avoid memory leaks.
*/
SDL_Texture * MissileResourceManager::loadTextureAt(unsigned int index)
{
    if(index > missile_resources.size() || missile_resources[index] == nullptr)
        return nullptr;

    SDL_Surface *s = loadSurfaceFromFileBuffer(missile_resources[index]);

    if(s == nullptr)
        return nullptr;

    SDL_Texture *t = loadTextureFromSurface(s);
    SDL_FreeSurface(s);
    return t;
}


MissileResourceManager::~MissileResourceManager()
{
    // Free the resources
    for(unsigned int i = 0; i < missile_resources.size(); i++)
    {
        delete missile_resources[i];
        missile_resources[i] = nullptr;
    }
}