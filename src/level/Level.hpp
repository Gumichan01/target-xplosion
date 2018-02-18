
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

#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED

#include "../resources/EnemyInfo.hpp"

#include <queue>


class Level
{
    static unsigned int id;
    std::queue<EnemyInfo> enemy_queue;
    unsigned long  qsize;
    bool has_bparts;

    Level();
    Level(const Level&);
    Level& operator =(const Level&);

public:

    static const unsigned int ROCKET_LEVEL_MIN = 1;     // Minimal level to use rockets
    static const unsigned int BOMB_LEVEL_MIN = 3;       // Minimal level to use bombs
    static const unsigned int LASER_LEVEL_MIN = 5;      // Minimal level to use laser
    static const unsigned int ALARM_TYPE = 22;          // Alarm identifier
    static const unsigned int MAX_LEVEL = 3;            // Maximum number of levels

    explicit Level(const unsigned int lvl);
    bool statEnemyInfo(EnemyInfo& data) noexcept;
    void popData() noexcept;

    bool hasBossParts() const noexcept;
    unsigned long numberOfEnemies() const noexcept;
    static unsigned int getLevelNum() noexcept;

    ~Level() = default;
};

#endif // LEVEL_HPP_INCLUDED
