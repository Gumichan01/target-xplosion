
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


#include "GamepadControl.hpp"


namespace GPconfig
{

const std::unordered_map<ActionControl, UTF8string> GamepadControl::DEFAULT_MAP =
{
    { ActionControl::SHOT, A_BUTTON },
    { ActionControl::ROCKET, X_BUTTON },
    { ActionControl::BOMB, B_BUTTON },
    { ActionControl::SLOW, RB_BUTTON }
};

std::unordered_map<ActionControl, UTF8string> GamepadControl::hmap = DEFAULT_MAP;


const std::unordered_map<ActionControl, UTF8string>& GamepadControl::getMap() noexcept
{
    return hmap;
}

void GamepadControl::updateControl(const ActionControl& ac, const UTF8string& u8) noexcept
{
    try
    {
        hmap[ac] = u8;
    }
    catch(...)
    {

    }
}

}
