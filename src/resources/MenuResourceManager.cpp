
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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

#include "MenuResourceManager.hpp"
#include "../asset/TX_Asset.hpp"
#include "../resources/WinID.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <array>

using namespace LX_Graphics;

namespace
{
std::array<LX_Graphics::LX_Sprite*, Asset::NB_MENU_IMG> menu_resources;
}

MenuResourceManager::MenuResourceManager()
{
    const TX_Asset * const ASSET = TX_Asset::getInstance();
    LX_Win::LX_Window& w = LX_Win::getWindowManager().getWindow(WinID::getWinID());
    menu_resources.fill(nullptr);

    // Load the resources
    for(unsigned int i = 0; i < menu_resources.size(); i++)
    {
        const std::string& str = ASSET->getMenuImgFile(i);

        if(!str.empty())
            menu_resources[i] = new LX_Graphics::LX_Sprite(str, w);
    }
}

LX_Graphics::LX_Sprite * MenuResourceManager::getImageAt(unsigned int index) const noexcept
{
    if(index > menu_resources.size())
        return nullptr;

    return menu_resources[index];
}

MenuResourceManager::~MenuResourceManager()
{
    // Free the resources
    for(unsigned int i = 0; i < menu_resources.size(); i++)
    {
        delete menu_resources[i];
    }
}
