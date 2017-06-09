
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

#include "Level.hpp"
#include "../resources/EnemyLoader.hpp"


unsigned int Level::id = 0;

Level::Level(const unsigned int lvl) : loaded(false), enemy_queue(), qsize(0)
{
    EnemyLoader::load(lvl, enemy_queue);
    loaded = true;
    id = lvl;

    for(auto it = enemy_queue.cbegin(); it != enemy_queue.cend(); ++it)
    {
        if(!(*it)._alarm) qsize += 1;
    };
}

bool Level::isLoaded() const
{
    return loaded;
}

bool Level::statEnemyInfo(EnemyInfo& data)
{
    if(enemy_queue.empty())
        return false;

    EnemyInfo front_data = enemy_queue.front();
    data.e = front_data.e;
    data.t = front_data.t;
    data._alarm = front_data._alarm;
    data.boss = front_data.boss;
    return true;
}

void Level::popData()
{
    if(!enemy_queue.empty())
    {
        if(qsize > 0 && !enemy_queue.front()._alarm)qsize--;
        enemy_queue.pop_front();
    }
}

unsigned int Level::getLevelNum()
{
    return id;
}

unsigned long Level::numberOfEnemies() const
{
    return qsize;
}
