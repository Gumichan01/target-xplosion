
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

#include "EnemyLoader.hpp"
#include "EnemyData.hpp"
#include "ResourceManager.hpp"

#include "../game/engine/Engine.hpp"
#include "../game/Power.hpp"
#include "../asset/TX_Asset.hpp"
#include "../entities/Player.hpp"
#include "../entities/BasicEnemy.hpp"
#include "../entities/Bachi.hpp"
#include "../entities/Shooter.hpp"
#include "../entities/Tower.hpp"
#include "../entities/Heaviside.hpp"
#include "../entities/boss/SemiBoss01.hpp"
#include "../entities/boss/SemiBoss02.hpp"
#include "../entities/boss/Boss01.hpp"
#include "../entities/boss/Boss02.hpp"
#include "../entities/boss/BossXX.hpp"

#include <LunatiX/LX_FileIO.hpp>
#include <LunatiX/LX_Log.hpp>

using namespace LX_Win;
using namespace LX_Mixer;

namespace EnemyLoader
{

bool readData(LX_FileIO::LX_File& f, EnemyData& datum);
bool generateEnemyInfo(LX_FileIO::LX_File& f, EnemyInfo& info);

bool readData(LX_FileIO::LX_File& f, EnemyData& datum)
{
    if(f.readExactly(&datum.type, sizeof(unsigned int), 1) == 0)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "EnemyLoader::readData - Cannot read the type");
        return false;
    }

    if(f.readExactly(&datum.hp, sizeof(unsigned int), 1) == 0)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "EnemyLoader::readData - Cannot read the hp");
        return false;
    }

    if(f.readExactly(&datum.att, sizeof(unsigned int), 1) == 0)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "EnemyLoader::readData - Cannot read att");
        return false;
    }

    if(f.readExactly(&datum.sh, sizeof(unsigned int), 1) == 0)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "EnemyLoader::readData - Cannot read the shield");
        return false;
    }

    if(f.readExactly(&datum.time, sizeof(unsigned int), 1) == 0)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "EnemyLoader::readData - Cannot read the time value");
        return false;
    }

    if(f.readExactly(&datum.y, sizeof(unsigned int), 1) == 0)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "EnemyLoader::readData - Cannot read the y position");
        return false;
    }

    if(f.readExactly(&datum.w, sizeof(unsigned int), 1) == 0)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "EnemyLoader::readData - Cannot read the width");
        return false;
    }

    if(f.readExactly(&datum.h, sizeof(unsigned int), 1) == 0)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "EnemyLoader::readData - Cannot read the height");
        return false;
    }

    return true;
}


bool generateEnemyInfo(LX_FileIO::LX_File& f, EnemyInfo& info)
{
    const int EXPLOSION_ID = 3;
    EnemyData datum;
    const ResourceManager *rc = ResourceManager::getInstance();

    if(readData(f, datum))
    {
        LX_Graphics::LX_Sprite * texture = nullptr;

        if(datum.type < NB_ENEMIES)
            texture = rc->getResource(RC_ENEMY, datum.type);

        int glimit = Engine::getInstance()->getMaxXlim();
        info.t = datum.time;
        info._alarm = false;
        info.boss = false;

        switch(datum.type)
        {
        case 0:
        {
            info.e = new SemiBoss01(Rank::healthUp(datum.hp), datum.att,
                                    Rank::shieldUp(datum.sh), texture,
                                    rc->getSound(EXPLOSION_ID), glimit + 1,
                                    datum.y, datum.w, datum.h, -1, 1);
        }
        break;

        case 1:
        {
            info.boss = true;
            info.e = new Boss01(Rank::healthUp(datum.hp), datum.att,
                                Rank::shieldUp(datum.sh), texture,
                                rc->getSound(EXPLOSION_ID), glimit + 1,
                                datum.y, datum.w, datum.h, -4, 0);
        }
        break;

        case 2:
        {
            info.boss = true;
            info.e = new Boss02(Rank::healthUp(datum.hp), datum.att,
                                Rank::shieldUp(datum.sh), texture,
                                rc->getSound(EXPLOSION_ID), glimit + 1,
                                datum.y, datum.w, datum.h, -4, 0);
        }
        break;

        case 3:
        {
            info.boss = true;
            info.e = new BossXX(Rank::healthUp(datum.hp), datum.att,
                                Rank::shieldUp(datum.sh), texture,
                                rc->getSound(EXPLOSION_ID), glimit + 1,
                                datum.y, datum.w, datum.h, -4, 0);
        }
        break;

        case 22:
        {
            // Boss is comming ( T_T)
            info._alarm = true;
        }
        break;

        case 50:
        {
            info.e = new SemiBoss01(Rank::healthUp(datum.hp), datum.att,
                                    Rank::shieldUp(datum.sh), texture,
                                    rc->getSound(EXPLOSION_ID), glimit + 1,
                                    datum.y, datum.w, datum.h, -1, 0);
        }
        break;

        case 51:
        {
            info.e = new SemiBoss02(Rank::healthUp(datum.hp), datum.att,
                                    Rank::shieldUp(datum.sh), texture,
                                    rc->getSound(EXPLOSION_ID), glimit + 1,
                                    datum.y, datum.w, datum.h, -4, 0);
        }
        break;

        case 100:
        {
            info.e = new Tower1(datum.hp, datum.att, datum.sh,
                                texture, nullptr, glimit + 1,
                                datum.y, datum.w, datum.h, -1, 0);
        }
        break;

        case 101:
        {
            info.e = new BasicEnemy(datum.hp, datum.att, datum.sh,
                                    texture, nullptr, glimit + 1,
                                    datum.y, datum.w, datum.h, -5, 0);
        }
        break;

        case 102:
        {
            info.e = new Shooter(datum.hp, datum.att, datum.sh,
                                 texture, nullptr, glimit + 1,
                                 datum.y, datum.w, datum.h, -6, 0);
        }
        break;

        case 103:
        {
            info.e = new Bachi(datum.hp, datum.att, datum.sh,
                               texture, nullptr, glimit + 1,
                               datum.y, datum.w, datum.h, -7, 8);
        }
        break;

        case 104:
        {
            info.e = new Heaviside(datum.hp, datum.att, datum.sh,
                                   texture, nullptr, glimit + 1,
                                   datum.y, datum.w, datum.h, -9, 0);
        }
        break;

        case 105:
        {
            info.e = new RHeaviside(datum.hp, datum.att, datum.sh,
                                    texture, nullptr, glimit + 1,
                                    datum.y, datum.w, datum.h, -9, 0);
        }
        break;

        default:
            break;
        }

        return true;
    }

    return false;
}

void load(unsigned int id, std::queue<EnemyInfo>& q)
{
    const int TX_TAG = 0xCF3A1;
    LX_FileIO::LX_File f(TX_Asset::getInstance()->getLevelPath(id),
                         LX_FileIO::LX_FILEIO_RDONLY);

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Level file %s : opened\n",
                     f.getFilename());

    /// Read the tag
    int tag = 0;
    f.readExactly(&tag, sizeof(int), 1);

    if(tag != TX_TAG)
    {
        f.close();
        throw LX_FileIO::IOException("Invalid file: bad tag\n");
    }

    /// Read the number of data
    int sz = 0;
    if(f.readExactly(&sz, sizeof(int), 1) == 0)
    {
        f.close();
        throw LX_FileIO::IOException(LX_GetError());
    }

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Tag: 0x%x; size: %u\n", tag, sz);

    int j = 0;
    EnemyInfo info;

    /// Read data
    while(j != sz && generateEnemyInfo(f, info))
    {
        q.push(info);
        j++;
    }

    if(j != sz)
    {
        std::string s = LX_GetError();
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "%s - Cannot read data no %d\n",
                            f.getFilename(), j);
        f.close();
        throw LX_FileIO::IOException(s);
    }

    f.close();
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Done, level file closed\n");
}

}
