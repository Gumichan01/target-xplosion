
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "Level.hpp"
#include "../resources/EnemyLoader.hpp"


namespace
{
const unsigned int LEVEL_WITH_BOSS_PARTS = 3;
}

unsigned int Level::id = 0;

Level::Level( const unsigned int lvl ) : enemy_queue(), qsize( 0 ),
    has_bparts( false )
{
    qsize = EnemyLoader::load( lvl, enemy_queue );
    has_bparts = ( lvl == LEVEL_WITH_BOSS_PARTS );
    id = lvl;
}

bool Level::statEnemyInfo( EnemyInfo& data ) noexcept
{
    if ( enemy_queue.empty() )
        return false;

    EnemyInfo front_data = enemy_queue.front();
    data.e = front_data.e;
    data.t = front_data.t;
    data._alarm = front_data._alarm;
    data.boss = front_data.boss;
    return true;
}

void Level::popData() noexcept
{
    if ( !enemy_queue.empty() )
    {
        if ( qsize > 0 && !enemy_queue.front()._alarm )
            qsize--;

        enemy_queue.pop();
    }
}

bool Level::hasBossParts() const noexcept
{
    return has_bparts;
}

unsigned int Level::getLevelNum() noexcept
{
    return id;
}

unsigned long Level::numberOfEnemies() const noexcept
{
    return qsize;
}
