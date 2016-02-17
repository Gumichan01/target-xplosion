

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
*	@file PlayerResourceManager.cpp
*	@brief The player ressource implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "PlayerResourceManager.hpp"

#include <LunatiX/LX_Log.hpp>
#include <LunatiX/LX_FileIO.hpp>
#include <LunatiX/LX_FileBuffer.hpp>
#include <LunatiX/LX_Graphics.hpp>

using namespace std;
using namespace LX_FileIO;
using namespace LX_Graphics;


PlayerResourceManager::PlayerResourceManager()
    : player_without_sh(nullptr), player_with_sh(nullptr)
{
    try
    {
        TX_Asset *asset = TX_Asset::getInstance();

        player_without_sh = new LX_FileBuffer(asset->getPlayerFile());
        player_with_sh = new LX_FileBuffer(asset->getPlayerShieldFile());
    }
    catch(IOException& e)
    {
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                         "fatal error : cannot load the data of the player\n");

        if(player_without_sh != nullptr)
            delete player_without_sh;

        throw e;
    }
}


SDL_Texture * PlayerResourceManager::loadTexture(bool shield)
{
    SDL_Surface *s = nullptr;

    if(shield)
        s = loadSurfaceFromFileBuffer(player_with_sh);
    else
        s = loadSurfaceFromFileBuffer(player_without_sh);

    if(s == nullptr)
        return nullptr;

    SDL_Texture *t = loadTextureFromSurface(s);
    SDL_FreeSurface(s);
    return t;
}


PlayerResourceManager::~PlayerResourceManager()
{
    delete player_with_sh;
    delete player_without_sh;
    player_with_sh = nullptr;
    player_without_sh = nullptr;

}








