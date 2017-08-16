
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

#include "Balance.hpp"


namespace DynamicGameBalance
{
const short MIN_DIFFICULTY = -1;
const short MAX_DIFFICULTY = 3;

short difficulty_level = 0;

void reset()
{
    difficulty_level = 0;
}

void notifyDeath()
{
    if(difficulty_level > MIN_DIFFICULTY)
        difficulty_level--;
}

void notifyUp()
{
    if(difficulty_level < MAX_DIFFICULTY)
    difficulty_level++;
}

float apply_dgb(float v)
{
    // 0 is the standard difficulty
    if(difficulty_level == 0)
        return v;

    // I have to adapt the speed of the bullets accordind to the difficulty
    float df = static_cast<float>(difficulty_level);
    return v > 0.0f ? v + df : v - df;
}

}
