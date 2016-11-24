

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
*	website: https://gumichan01.github.io/
*	mail: luxon.jean.pierre@gmail.com
*/

#include "PlayerResourceManager.hpp"
#include "../asset/TX_Asset.hpp"

#include <LunatiX/LX_FileIO.hpp>
#include <LunatiX/LX_Graphics.hpp>


PlayerResourceManager::PlayerResourceManager()
    : player_without_sh(nullptr), player_with_sh(nullptr)
{
    LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(0);
    const TX_Asset *asset = TX_Asset::getInstance();

    // Player without shield
    player_without_sh = new LX_Graphics::LX_Sprite(asset->getPlayerFile(),*w);

    if(player_without_sh == nullptr)
        throw LX_FileIO::IOException(std::string("cannot load ") + asset->getPlayerFile());

    // Player with shield
    player_with_sh = new LX_Graphics::LX_Sprite(asset->getPlayerShieldFile(),*w);

    if(player_with_sh == nullptr)
        throw LX_FileIO::IOException(std::string("cannot load ")
                                     + asset->getPlayerShieldFile());
}


LX_Graphics::LX_Sprite * PlayerResourceManager::getTexture(bool shield) const
{
    return shield ? player_with_sh : player_without_sh;
}


PlayerResourceManager::~PlayerResourceManager()
{
    delete player_without_sh;
    delete player_with_sh;
}
