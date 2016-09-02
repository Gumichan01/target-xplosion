
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
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
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/

#include "Level.hpp"
#include "../game/Rank.hpp"
#include "../asset/TX_Asset.hpp"
#include "../resources/EnemyLoader.hpp"

#include <LunatiX/LX_Error.hpp>
#include <LunatiX/LX_Log.hpp>

#include <sstream>

using namespace std;

unsigned int Level::id = 0;


Level::Level(const unsigned int lvl) : loaded(false), enemy_queue()
{
    load(lvl);
}


void Level::load(const unsigned int lvl)
{
    EnemyLoader::load(lvl,enemy_queue);
    loaded = true;
}


bool Level::isLoaded()
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
        enemy_queue.pop();
}


unsigned int Level::getLevelNum()
{
    return id;
}


unsigned long Level::numberOfEnemies()
{
    return enemy_queue.size();
}
