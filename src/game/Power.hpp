
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

#ifndef RANK_HPP_INCLUDED
#define RANK_HPP_INCLUDED


namespace Rank
{
constexpr unsigned int POWER_LEVEL = 3;
constexpr unsigned int POWER_LIFE = 6;

// More shield for the enemies
inline unsigned int shieldUp(const unsigned int sh)
{
    return POWER_LEVEL * sh;
}

// More health for bosses
inline unsigned int healthUp(const unsigned int hp)
{
    return hp * POWER_LIFE;
}

}

#endif // RANK_HPP_INCLUDED

