
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
*	@file EnemyLoader.cpp
*	@brief The implementation of the enemy loader
*	@author Luxon Jean-Pierre(Gumichan01)
*/

#include "EnemyLoader.hpp"
#include "../level/EnemyData.hpp"
#include "../asset/TX_Asset.hpp"
#include <LunatiX/LX_FileIO.hpp>
#include <LunatiX/LX_Log.hpp>
#include <string>

namespace EnemyLoader
{

bool readData(LX_FileIO::LX_File& f,EnemyData& datum);
bool generateEnemyInfo(LX_FileIO::LX_File& f,EnemyInfo& info);

bool readData(LX_FileIO::LX_File& f,EnemyData& datum)
{
    if(f.readExactly(&datum.type,sizeof(unsigned int),1) == 0)
    {
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                         "EnemyLoader::readData - Cannot read the type");
        return false;
    }

    if(f.readExactly(&datum.hp,sizeof(unsigned int),1) == 0)
    {
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                         "EnemyLoader::readData - Cannot read the hp");
        return false;
    }

    if(f.readExactly(&datum.att,sizeof(unsigned int),1) == 0)
    {
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                         "EnemyLoader::readData - Cannot read att");
        return false;
    }

    if(f.readExactly(&datum.sh,sizeof(unsigned int),1) == 0)
    {
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                         "EnemyLoader::readData - Cannot read the shield");
        return false;
    }

    if(f.readExactly(&datum.time,sizeof(unsigned int),1) == 0)
    {
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                         "EnemyLoader::readData - Cannot read the time value");
        return false;
    }

    if(f.readExactly(&datum.y,sizeof(unsigned int),1) == 0)
    {
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                         "EnemyLoader::readData - Cannot read the y position");
        return false;
    }

    if(f.readExactly(&datum.w,sizeof(unsigned int),1) == 0)
    {
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                         "EnemyLoader::readData - Cannot read the width");
        return false;
    }

    if(f.readExactly(&datum.h,sizeof(unsigned int),1) == 0)
    {
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                         "EnemyLoader::readData - Cannot read the height");
        return false;
    }

    return true;
}


bool generateEnemyInfo(LX_FileIO::LX_File& f,EnemyInfo& info)
{
    EnemyData datum;

    if(readData(f,datum))
    {
        /// @todo (3) Generate the enemy
        return true;
    }

    return false;
}

void load(unsigned int id, std::queue<EnemyInfo>& q)
{
    const int TX_TAG = 0xCF3A1;
    LX_FileIO::LX_File f(TX_Asset::getInstance()->getLevelPath(id),
                         LX_FILEIO_RDONLY);

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Level file %s : opened\n",
                     f.getFilename());

    // Read the tag
    int tag = 0;
    f.readExactly(&tag,sizeof(int),1);

    if(tag != TX_TAG)
    {
        f.close();
        throw LX_FileIO::IOException("Invalid file: bad tag\n");
    }

    // Read the number of data
    int sz = 0;
    if(f.readExactly(&sz,sizeof(int),1) == 0)
    {
        f.close();
        throw LX_FileIO::IOException(SDL_GetError());
    }

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Tag: 0x%x; size: %u\n",tag,sz);

    int j = 0;
    EnemyInfo info;
    while(j != sz && generateEnemyInfo(f,info))
    {
        //q.push(info);
        j++;
    }

    if(j != sz)
    {
        const char * s = SDL_GetError();
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,"%s - Cannot read data no %d\n",
                         f.getFilename(),j);
        f.close();
        throw LX_FileIO::IOException(s);
    }

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Done\n");
    f.close();
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Level file closed\n");
}

}
