

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
*	@file PlayerResourceManager.cpp
*	@brief The player ressource implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "PlayerResourceManager.hpp"
#include "../asset/TX_Asset.hpp"

#include <LunatiX/LX_Log.hpp>
#include <LunatiX/LX_FileIO.hpp>
#include <LunatiX/LX_Graphics.hpp>

using namespace std;
using namespace LX_Graphics;


PlayerResourceManager::PlayerResourceManager()
    : player_without_sh(nullptr), player_with_sh(nullptr)
{
    TX_Asset *asset = TX_Asset::getInstance();

    player_without_sh = loadTextureFromFile(asset->getPlayerFile());

    if(player_without_sh == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "fatal error: cannot load data from the player\n");
        throw LX_FileIO::IOException(string("cannot load ") + asset->getPlayerFile());
    }

    player_with_sh = loadTextureFromFile(asset->getPlayerShieldFile());

    if(player_with_sh == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "fatal error: cannot load data from the player\n");
        throw LX_FileIO::IOException(string("cannot load ")
                                     + asset->getPlayerShieldFile());
    }
}


SDL_Texture * PlayerResourceManager::getTexture(bool shield)
{
    return shield ? player_with_sh : player_without_sh;
}
