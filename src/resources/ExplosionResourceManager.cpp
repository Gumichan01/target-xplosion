
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

/**
*	@file ExplosionResourceManager.cpp
*	@brief A file that manages explosion ressources
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "ExplosionResourceManager.hpp"
#include "../asset/TX_Asset.hpp"

#include <LunatiX/LX_Graphics.hpp>

ExplosionResourceManager::ExplosionResourceManager()
{
    LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(0);
    TX_Asset *asset = TX_Asset::getInstance();
    explosion_resources.fill(nullptr);

    for(unsigned int i = 0; i < explosion_resources.size(); i++)
    {
        const std::string& str = asset->getExplosionSpriteFile(i);
        /// @todo [HIGH] For bosses → build animated sprite
        explosion_resources[i] = new LX_Graphics::LX_Sprite(str,*w);
    }
}

LX_Graphics::LX_Sprite * ExplosionResourceManager::getTextureAt(unsigned int index)
{
    if(index > explosion_resources.size() || explosion_resources[index] == nullptr)
        return nullptr;

    return explosion_resources[index];
}

ExplosionResourceManager::~ExplosionResourceManager()
{
    // Free the resources
    /// @fixme Segmentation fault to fix
    for(unsigned int i = 0; i < explosion_resources.size(); i++)
    {
        if(explosion_resources[i] != nullptr)
            delete explosion_resources[i];
    }
}
