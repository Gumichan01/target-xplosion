
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

#ifndef GINPUT_HPP
#define GINPUT_HPP

class Player;

namespace lx
{

namespace Event
{
class EventHandler;
}

}

namespace PlayerInput
{

void input( Player& p, bool& done ) noexcept;

void keyboardState( Player& p ) noexcept;
void joystickState( Player& p ) noexcept;
void inputKeyboard( const lx::Event::EventHandler& event, Player& p ) noexcept;
void inputJoystickAxis( const lx::Event::EventHandler& event, Player& p ) noexcept;
void inputJoystickButton( const lx::Event::EventHandler& event, Player& p ) noexcept;

}

#endif
