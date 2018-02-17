
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017 Luxon Jean-Pierre
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

#ifndef ENEMYDATA_HPP_INCLUDED
#define ENEMYDATA_HPP_INCLUDED

struct EnemyData
{
    unsigned int type = 0U;     /**< From 0 to 100: Boss; > 100: other enemies  */
    unsigned int hp   = 0U;     /**< The health points of the enemy             */
    unsigned int att  = 0U;     /**< The attack value of the enemy              */
    unsigned int sh   = 0U;     /**< The defense value of the enemy             */
    unsigned int time = 0U;     /**< The time for the enemy to come             */
    int y = 0;                  /**< The Y position of the enemy                */
    int w = 0;                  /**< The width of the enemy                     */
    int h = 0;                  /**< The height of the enemy                    */
};

#endif // ENEMYDATA_HPP_INCLUDED
