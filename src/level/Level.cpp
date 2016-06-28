
/*
*   Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Luxon Jean-Pierre (Gumichan01)
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file Level.cpp
*	@brief The level implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <sstream>

#include <LunatiX/LX_Error.hpp>

#include "Level.hpp"
#include "EnemyData.hpp"
#include "../game/Rank.hpp"
#include "../asset/TX_Asset.hpp"

using namespace std;

unsigned int Level::id = 0;
bool Level::has_alarm = false;


Level::Level(const unsigned int lvl) : loaded(false)
{
    load(lvl);
}


Level::~Level()
{
    EnemyData *d = nullptr;

    while(enemy_queue.empty() == false)
    {
        d = enemy_queue.front();
        enemy_queue.pop();
        delete d;
        d = nullptr;
    }
}


bool Level::load(const unsigned int lvl)
{
    const int tag = 0xCF3A1;
    int size = 0;
    int tmp;
    ostringstream ss;

    FILE *reader = nullptr;
    EnemyData tmp_data;

    id = lvl;
    reader = fopen(TX_Asset::getInstance()->getLevelPath(id),"rb");

    if(reader == nullptr)
    {
        ss << "Error while opening the level file" << "\n";
        LX_SetError(ss.str());
        return false;
    }

    fread(&tmp,sizeof(int),1,reader);

    if(tmp != tag)
    {
        ss << "Invalid file" << "\n";
        LX_SetError(ss.str());
        fclose(reader);
        return false;
    }

    fread(&size,sizeof(int),1,reader);

    for(int i = 0; i < size; i++)
    {
        fread(&tmp_data.type,sizeof(unsigned int),1,reader);
        fread(&tmp_data.hp,sizeof(unsigned int),1,reader);
        fread(&tmp_data.att,sizeof(unsigned int),1,reader);
        fread(&tmp_data.sh,sizeof(unsigned int),1,reader);
        fread(&tmp_data.time,sizeof(unsigned int),1,reader);
        fread(&tmp_data.y,sizeof(unsigned int),1,reader);
        fread(&tmp_data.w,sizeof(unsigned int),1,reader);
        fread(&tmp_data.h,sizeof(unsigned int),1,reader);

        // Check if the level has an alarm signal
        if(tmp_data.type == ALARM_TYPE)
            has_alarm = true;

        pushData(tmp_data);
    }

    fclose(reader);
    loaded = true;

    return loaded;
}


bool Level::isLoaded(void)
{
    return loaded;
}


void Level::pushData(const EnemyData& data)
{
    // Create a copy of the data structure
    EnemyData *object = new EnemyData(data);

    // For the rank
    object->hp = Rank::healthUp(data.hp);
    object->sh = Rank::shieldUp(data.sh);
    enemy_queue.push(object);
}


bool Level::statEnemyData(EnemyData& data)
{
    if(enemy_queue.empty())
        return false;

    EnemyData *front_data = enemy_queue.front();

    // Found the alarm, removed from the queue
    if(front_data->type == ALARM_TYPE)
        has_alarm = false;

    data = (*front_data);
    return true;
}


void Level::popData(void)
{
    if(!enemy_queue.empty())
    {
        EnemyData *tmp = enemy_queue.front();
        enemy_queue.pop();
        delete tmp;
    }
}


unsigned int Level::getLevelNum(void)
{
    return id;
}


unsigned long Level::numberOfEnemies(void)
{
    // Do not take the alarm as an enemy
    if(has_alarm)
        return enemy_queue.size() -1;

    // No alarm
    return enemy_queue.size();
}


bool Level::hasAlarmSignal(void)
{
    return has_alarm;
}

