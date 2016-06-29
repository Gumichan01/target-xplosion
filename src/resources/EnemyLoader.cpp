
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

namespace EnemyLoader
{

bool readData(LX_FileIO::LX_File& f,EnemyData& datum);
bool generateEnemyInfo(LX_FileIO::LX_File& f,EnemyInfo& info);

bool readData(LX_FileIO::LX_File& f,EnemyData& datum)
{
    /// @todo (2) read each datum
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
    /// @todo (1) open the file
    LX_FileIO::LX_File f(TX_Asset::getInstance()->getLevelPath(id),
                         LX_FILEIO_RDONLY);

    EnemyInfo info;
    while(generateEnemyInfo(f,info))
    {
        q.push(info);
    }

    /// @todo (1) close the file
    f.close();
}

}
