
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017  Luxon Jean-Pierre
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

#ifndef BALANCE_HPP_INCLUDED
#define BALANCE_HPP_INCLUDED

#include <Lunatix/utils/float.hpp>

/**
*   This namespace handles the Dynamic Game Difficulty Balancing of Target Xplosion
*   https://en.wikipedia.org/wiki/Dynamic_game_difficulty_balancing
*/
namespace DynamicGameBalance
{
void reset();

// Notification
void notifyCombo();
void notifyDeath();
void notifyShield();
void notifyHealth();
void notifyRocket();
void notifyBomb();

Float apply_dgb( const Float& v );
unsigned int getComboLimit();
Float dgb_mult();
void debugDisplay();
}


#endif // BALANCE_HPP_INCLUDED
