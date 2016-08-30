
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
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
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/

#include "EnemyResourceManager.hpp"
#include "../asset/TX_Asset.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Log.hpp>

using namespace LX_Graphics;

namespace
{
std::array<LX_Graphics::LX_Sprite*,NB_ENEMIES> enemy_resources;
};

EnemyResourceManager::EnemyResourceManager()
{
    LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(0);
    TX_Asset *asset = TX_Asset::getInstance();
    enemy_resources.fill(nullptr);

    // Load the resources
    for(unsigned int i = 0; i < enemy_resources.size(); i++)
    {
        const std::string& str = asset->getEnemySpriteFile(i).c_str();
        enemy_resources[i] = new LX_Graphics::LX_Sprite(str,*w);

        if(enemy_resources[i] == nullptr)
        {
            if(LX_Log::isDebugMode())
            {
                LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                                 "The enemy resources #%d is unavailable",i);
            }
        }
    }
}

LX_Graphics::LX_Sprite * EnemyResourceManager::getTextureAt(unsigned int index)
{
    if(index > enemy_resources.size() || enemy_resources[index] == nullptr)
        return nullptr;

    return enemy_resources[index];
}

EnemyResourceManager::~EnemyResourceManager()
{
    // Free the resources
    for(unsigned int i = 0; i < enemy_resources.size(); i++)
    {
        if(enemy_resources[i] != nullptr)
            delete enemy_resources[i];
    }
}
