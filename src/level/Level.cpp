
/*
*   Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2015  Luxon Jean-Pierre
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
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file Level.cpp
*	@brief The level implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>

#include "Level.hpp"
#include "EnemyData.hpp"

#define TAG_LENGTH 6
#define BUFSIZE 64


unsigned int Level::id = 0;

Level::Level()
{
    loaded = false;
}


Level::Level(const unsigned int lvl)
{
    loaded = false;
    id = lvl;
    load(lvl);
}

Level::~Level()
{
    EnemyData *d = NULL;

    while(enemy_queue.empty() == false)
    {
        d = enemy_queue.front();
        enemy_queue.pop();
        delete d;
        d = NULL;
    }
}


bool Level::load(const unsigned int lvl)
{
    const int tag = 0xCF3A1;
    int size = 0;
    int tmp;

    FILE *reader = NULL;
    EnemyData tmp_data;

    id =lvl;

    switch(lvl)
    {
        case 0 :
        {
            reader = fopen("data/00.targetx","rb");
        }
        break;

        default :
        {
            return false;
        }
        break;
    }

    if(reader == NULL)
    {
        cerr << "Error while opening the level file" << endl;
        return false;
    }

    fread(&tmp,sizeof(int),1,reader);

    if(tmp != tag)
    {
        cerr << "Invalid file" << endl;
        fclose(reader);
        return false;
    }

    fread(&size,sizeof(int),1,reader);

    for(int i = 0; i < size; i++)
    {
        //memset(&tmp_data,0,sizeof(EnemyData));
        fread(&tmp_data.type,sizeof(unsigned int),1,reader);
        fread(&tmp_data.hp,sizeof(unsigned int),1,reader);
        fread(&tmp_data.att,sizeof(unsigned int),1,reader);
        fread(&tmp_data.sh,sizeof(unsigned int),1,reader);
        fread(&tmp_data.time,sizeof(unsigned int),1,reader);
        fread(&tmp_data.y,sizeof(unsigned int),1,reader);
        fread(&tmp_data.w,sizeof(unsigned int),1,reader);
        fread(&tmp_data.h,sizeof(unsigned int),1,reader);

        /*cout << tmp_data.type << " "
             << tmp_data.hp << " "
             << tmp_data.att << " "
             << tmp_data.sh << " "
             << tmp_data.time << " "
             << tmp_data.y << " "
             << tmp_data.w << " "
             << tmp_data.h << endl;*/

        pushData(&tmp_data);
    }

    fclose(reader);
    loaded = true;

    return loaded;
}


bool Level::isLoaded(void)
{
    return loaded;
}


void Level::pushData(const EnemyData *data)
{
    EnemyData *object = NULL;

    object = new EnemyData();
    memcpy(object,data,sizeof(EnemyData));
    enemy_queue.push(object);
}


bool Level::statEnemyData(EnemyData *data)
{
    EnemyData *front_data = NULL;

    if(enemy_queue.empty())
        return false;

    front_data = enemy_queue.front();
    memcpy(data,front_data,sizeof(EnemyData));

    return true;
}


void Level::popData(void)
{
    EnemyData *tmp;

    if(!enemy_queue.empty())
    {
        tmp = enemy_queue.front();
        enemy_queue.pop();
        delete tmp;
    }
}


unsigned int Level::getLevelNum(void)
{
    return id;
}

