
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

#include <LunatiX/LX_Log.hpp>

namespace DynamicGameBalance
{
const float MIN_DIFFICULTY = 0.0f;
const float MAX_DIFFICULTY = 4.0f;
const unsigned int COMBO_LIMIT = 1024;

const float COMBO_DGB  =  1.0f / static_cast<float>(COMBO_LIMIT);
const float DEATH_DGB  = -0.5f;
const float SHIELD_DGB =  0.00250f;
const float HEALTH_DGB = -0.03000f;
const float ROCKET_DGB =  0.00020f;
const float BOMB_DGB   = -0.06000f;

float difficulty_level = 1.0f;

void reset()
{
    difficulty_level = 1.0f;
}


void notifyCombo()
{
    if(difficulty_level < MAX_DIFFICULTY)
        difficulty_level += COMBO_DGB;
}

void notifyDeath()
{
    if(difficulty_level > MIN_DIFFICULTY)
        difficulty_level += DEATH_DGB;
}

void notifyShield()
{
    difficulty_level += SHIELD_DGB;
}

void notifyHealth()
{
    difficulty_level += HEALTH_DGB;
}

void notifyRocket()
{
    difficulty_level += ROCKET_DGB;
}

void notifyBomb()
{
    difficulty_level += BOMB_DGB;
}


float apply_dgb(float v)
{
    // 0 is the standard difficulty
    if(difficulty_level == 0)
        return v;

    return v > 0.0f ? v + difficulty_level : v - difficulty_level;
}

unsigned int getComboLimit()
{
    return COMBO_LIMIT;
}

float dgb_mult()
{
    return difficulty_level;
}

void debugDisplay()
{
    LX_Log::log("difficulty: %f", difficulty_level);
}

}
