
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
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "MissileResourceManager.hpp"

#include <LunatiX/LX_Graphics.hpp>

namespace
{
std::array<LX_Graphics::LX_Sprite*,PLAYER_MISSILES + ENEMY_MISSILES> missile_resources;
};

MissileResourceManager::MissileResourceManager()
{
    LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(0);
    const TX_Asset *asset = TX_Asset::getInstance();
    unsigned int j = PLAYER_MISSILES;

    missile_resources.fill(nullptr);

    for(unsigned int i = 0; i < missile_resources.size(); i++)
    {
        std::string str;

        if(i < PLAYER_MISSILES)
            str = asset->getPlayerMissilesFile(i);
        else
            str = asset->getEnemyMissilesFile(i-j);

        if(!str.empty())
            missile_resources[i] = new LX_Graphics::LX_Sprite(str,*w);
    }
}

LX_Graphics::LX_Sprite * MissileResourceManager::getTextureAt(unsigned int index) const
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
            delete missile_resources[i];
    }
}
