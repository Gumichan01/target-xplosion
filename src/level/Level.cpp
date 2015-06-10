
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


using namespace std;

Level::Level()
{
    loaded = false;
}


Level::Level(const unsigned int lvl)
{
    loaded = false;
    load(lvl);
}

Level::~Level()
{

}


bool Level::load(const unsigned int lvl)
{
    const int tag = 0xCF3A1;
    int size = 0;
    int tmp1;
    unsigned int tmp2;

    FILE *reader = NULL;
    EnemyData tmp_data;


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

    fread(&tmp1,sizeof(int),1,reader);

    cout << "TAG : " << tmp1 << endl;

    if(tmp1 != tag)
    {
        cerr << "Invalid file" << endl;
        fclose(reader);
        return false;
    }
    else
        cout << "TAG OK " << endl;

    fread(&size,sizeof(int),1,reader);
    cout << "Number of enemies " << size << endl;

    for(int i = 0; i < size; i++)
    {
        fread(&tmp_data.type,sizeof(unsigned int),1,reader);
        fread(&tmp_data.hp,sizeof(unsigned int),1,reader);
        fread(&tmp_data.att,sizeof(unsigned int),1,reader);
        fread(&tmp_data.sh,sizeof(unsigned int),1,reader);
        fread(&tmp_data.time,sizeof(uint64_t),1,reader);
        fread(&tmp_data.y,sizeof(unsigned int),1,reader);
        fread(&tmp_data.w,sizeof(unsigned int),1,reader);
        fread(&tmp_data.h,sizeof(unsigned int),1,reader);

        cout << tmp_data.type << " "
             << tmp_data.att << " "
             << tmp_data.sh << " "
             << tmp_data.time << " "
             << tmp_data.y << " "
             << tmp_data.w << " "
             << tmp_data.h << "\n" << endl;
    }

    fclose(reader);
    loaded = true;

    return loaded;
}




