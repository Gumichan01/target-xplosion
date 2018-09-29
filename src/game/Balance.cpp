
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

#include <Lunatix/Log.hpp>

#define TX_NOBALANCE

using namespace FloatBox;

namespace DynamicGameBalance
{

constexpr Float MIN_DIFFICULTY = fbox( 0.5f );
constexpr Float MAX_DIFFICULTY = fbox( 4.1f );
constexpr unsigned int COMBO_LIMIT = 2786;

constexpr Float COMBO_DGB  = fbox( 1.0f / static_cast<float>( COMBO_LIMIT ) );
constexpr Float DEATH_DGB  = fbox( -0.5f );
constexpr Float SHIELD_DGB = fbox( 0.00250f );
constexpr Float HEALTH_DGB = fbox( -0.00320f );
constexpr Float ROCKET_DGB = fbox( 0.00020f );
constexpr Float BOMB_DGB   = fbox( -0.00640f );


Float difficulty_level = fbox( 1.0f );

void reset()
{
    difficulty_level = fbox( 1.0f );
}


void notifyCombo()
{
    if ( difficulty_level < MAX_DIFFICULTY )
        difficulty_level += COMBO_DGB;
}

void notifyDeath()
{
    if ( difficulty_level > MIN_DIFFICULTY )
        difficulty_level += DEATH_DGB;
}

void notifyShield()
{
    if ( difficulty_level < MAX_DIFFICULTY )
        difficulty_level += SHIELD_DGB;
}

void notifyHealth()
{
    if ( difficulty_level > MIN_DIFFICULTY )
        difficulty_level += HEALTH_DGB;
}

void notifyRocket()
{
    if ( difficulty_level < MAX_DIFFICULTY )
        difficulty_level += ROCKET_DGB;
}

void notifyBomb()
{
    if ( difficulty_level > MIN_DIFFICULTY )
        difficulty_level += BOMB_DGB;
}


Float apply_dgb( const Float& v )
{
#ifdef TX_NOBALANCE
    return v;
#else
    return v > FNIL ? v + difficulty_level : v - difficulty_level;
#endif
}

unsigned int getComboLimit()
{
    return COMBO_LIMIT;
}

Float dgb_mult()
{
    return difficulty_level;
}

void debugDisplay()
{
    lx::Log::log( "difficulty: %f", difficulty_level );
}

}
