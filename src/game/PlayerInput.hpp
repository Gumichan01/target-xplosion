
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
*	@file GameInput.hpp
*	@brief The Game input namespace
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/


#ifndef GINPUT_HPP
#define GINPUT_HPP

union SDL_Event;
class Player;

namespace PlayerInput
{

void input(Player& p, bool& done);

void keyboardState(Player& p);
void joystickState(Player& p);
void inputKeyboard(SDL_Event& event, Player& p);
void inputJoystickAxis(SDL_Event& event, Player& p);
void inputJoystickButton(SDL_Event& event, Player& p);

};

#endif