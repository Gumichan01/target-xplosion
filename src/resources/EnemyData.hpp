
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

#ifndef ENEMYDATA_HPP_INCLUDED
#define ENEMYDATA_HPP_INCLUDED

typedef struct EnemyData
{
    unsigned int type;          /**< From 0 to 100: Boss; > 100: other enemies */
    unsigned int hp;            /**< The health points of the enemy */
    unsigned int att;           /**< The attack value of the enemy */
    unsigned int sh;            /**< The defense value of the enemy */
    unsigned int time;          /**< The time for the enemy to come */
    unsigned int y;             /**< The Y position of the enemy */
    unsigned int w;             /**< The width of the enemy */
    unsigned int h;             /**< The height of the enemy */

    EnemyData();
    EnemyData(const EnemyData& data);
    EnemyData& operator =(const EnemyData& data);

} EnemyData;

#endif // ENEMYDATA_HPP_INCLUDED
