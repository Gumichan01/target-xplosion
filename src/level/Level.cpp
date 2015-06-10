
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

#include <cstring>
#include <fstream>
#include <sstream>

#include "Level.hpp"

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
    const uint32_t tag = 0xCF3A1;
    uint32_t t;
    int64_t timeValue;
    int16_t Yvalue;
    int16_t wh;
    unsigned int point;

    char bufTag[TAG_LENGTH+1];
    char buf[BUFSIZE];
    int size = 0;

    ifstream reader;
    string ch;
    stringstream s;

    memset(bufTag,0,TAG_LENGTH);

    switch(lvl)
    {
        case 0 :
        {
            reader.open("data/00.targetx",ios::in|ios::binary);
        }
        break;

        default :
        {
            return false;
        }
        break;
    }

    if(!reader.is_open())
        return false;

    reader.read(bufTag,TAG_LENGTH*sizeof(char));

    ch = bufTag;
    cleanStream(&s);
    s.str(ch);
    s >> t;

    cout << "TAG : " << ch << endl;

    if(t != tag)
    {
        cerr << "Invalid file" << endl;
        return false;
    }
    else
        cout << "TAG OK " << endl;

    memset(buf,0,BUFSIZE);
    reader.read(buf,sizeof(char));

    ch = buf;
    cleanStream(&s);
    s.str(ch);
    s >> size;
    cout << "value : " << ch << endl;
    cout << "Number of enemies " << size << endl;

    /*for(int i = 0; i < size; i++)
    {
        memset(buf,0,BUFSIZE);
        reader.read(buf,sizeof(char));

        ch = buf;
        cleanStream(&s);
        s.str(ch);
    }*/


    reader.close();
    loaded = true;

    return loaded;
}


void Level::cleanStream(stringstream *s)
{
    s->clear();
    s->str("");
}














