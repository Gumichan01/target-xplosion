
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

#include "EnemyResourceManager.hpp"
#include "../asset/TX_Asset.hpp"
#include "../resources/WinID.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <array>

using namespace LX_Graphics;

namespace
{
std::array<LX_Graphics::LX_Sprite*, NB_ENEMIES> enemy_resources;
};

EnemyResourceManager::EnemyResourceManager()
{
    std::string prev_string("");
    LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(WinID::getWinID());
    const TX_Asset *asset = TX_Asset::getInstance();
    enemy_resources.fill(nullptr);

    // Load the resources
    for(unsigned int i = 0; i < enemy_resources.size(); i++)
    {
        const std::string& str = asset->getEnemySpriteFile(i).c_str();
        const TX_Anima* anima = asset->getEnemyAnimation(i);

        if(!str.empty())
        {
            // If the string is the same as the previous → same texture
            if(str == prev_string)
                enemy_resources[i] = i > 0 ? enemy_resources[i-1]: nullptr;
            else
            {
                using namespace LX_Graphics;
                if(anima != nullptr)
                    enemy_resources[i] = new LX_AnimatedSprite(str, *w, anima->v,
                            anima->delay);
                else
                    enemy_resources[i] = new LX_Sprite(str, *w);
            }
        }

        prev_string = str;
    }
}

LX_Graphics::LX_Sprite * EnemyResourceManager::getTextureAt(unsigned int index) const
{
    if(index > enemy_resources.size() || enemy_resources[index] == nullptr)
        return nullptr;

    return enemy_resources[index];
}

EnemyResourceManager::~EnemyResourceManager()
{
    // Free the resources
    const auto N = enemy_resources.size();

    for(unsigned int i = 0; i < N; i++)
    {
        if(enemy_resources[i] != nullptr)
        {
            unsigned int j = i + 1;

            while(j < N && enemy_resources[j] == enemy_resources[i])
            {
                enemy_resources[j] = nullptr;
                j += 1;
            }

            delete enemy_resources[i];
        }
    }
}
