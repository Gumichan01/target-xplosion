
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

#ifndef GAMEPADCONTROL_HPP
#define GAMEPADCONTROL_HPP

#include <LunatiX/utils/utf8_string.hpp>

#include <unordered_map>
#include <type_traits>
#include <functional>

namespace GPconfig
{

/*
    The different action made by the player in game

    - normal shot
    - rocket
    - bomb
    - slow motion
*/
enum class ActionControl: int
{
    SHOT = 0,
    ROCKET,
    BOMB,
    SLOW
};


const UTF8string A_BUTTON("a");
const UTF8string B_BUTTON("b");
const UTF8string X_BUTTON("x");
const UTF8string LB_BUTTON("leftshoulder");
const UTF8string RB_BUTTON("rightshoulder");
const UTF8string START_BUTTON("start");


class GamepadControl
{
    static const std::unordered_map<ActionControl, UTF8string> DEFAULT_MAP;
    static std::unordered_map<ActionControl, UTF8string> hmap;

    GamepadControl(const GamepadControl&) = delete;
    GamepadControl(const GamepadControl&&) = delete;
    GamepadControl& operator=(const GamepadControl&) = delete;
    GamepadControl& operator=(const GamepadControl&&) = delete;

public:

    GamepadControl() = default;

    static const UTF8string& getControl(const ActionControl& a) noexcept;
    static void updateControl(const ActionControl& ac, const UTF8string& u8) noexcept;

    ~GamepadControl() = default;
};


}

namespace std
{
template<>
class hash<GPconfig::ActionControl>
{
    using realtype_t = underlying_type<GPconfig::ActionControl>::type;

public:
    size_t operator()(const GPconfig::ActionControl& a) const
    {
        return hash<realtype_t>()(static_cast<realtype_t>(a));
    }
};
}

#endif // GAMEPADCONTROL_HPP
