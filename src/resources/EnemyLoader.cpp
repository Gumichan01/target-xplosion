
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

namespace EnemyLoader
{

bool readData(LX_FileIO::LX_File& f,EnemyData& datum);
bool generateEnemyInfo(LX_FileIO::LX_File& f,EnemyInfo& info);

bool readData(LX_FileIO::LX_File& f,EnemyData& datum)
{
    /// @todo (2) read each datum
    return false;
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
    LX_Log::logDebug(LX_Log::LX_LOG_SYSTEM,"Open level file\n");
    LX_FileIO::LX_File f(TX_Asset::getInstance()->getLevelPath(id),
                         LX_FILEIO_RDONLY);

    LX_Log::logDebug(LX_Log::LX_LOG_SYSTEM,"Level file %s : opened\n",f.getFilename());

    int tag;
    f.readExactly(&tag,sizeof(int),1);

    if(tag != TX_TAG)
    {
        f.close();
        throw LX_FileIO::IOException("Invalid file\n");
    }

    EnemyInfo info;
    LX_Log::logDebug(LX_Log::LX_LOG_SYSTEM,"Generate enemy\n");
    while(generateEnemyInfo(f,info))
    {
        q.push(info);
    }
    LX_Log::logDebug(LX_Log::LX_LOG_SYSTEM,"Done\n");
    f.close();
    LX_Log::logDebug(LX_Log::LX_LOG_SYSTEM,"Level file closed\n");
}

}
