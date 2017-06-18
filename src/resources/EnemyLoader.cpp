
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
#include "../entities/boss/SemiBoss03.hpp"
#include "../entities/boss/Boss01.hpp"
#include "../entities/boss/Boss02.hpp"
#include "../entities/boss/Boss04.hpp"

#include <LunatiX/LX_FileIO.hpp>
#include <LunatiX/LX_Log.hpp>

using namespace LX_Win;
using namespace LX_Mixer;

namespace
{
// Bosses
const int BOSS01_XVEL = -4;
const int BOSS02_XVEL = -4;
const int BOSSXX_XVEL = -4;

// Semibosses
const int SEMIBOSS01_XVEL = -1;
const int SEMIBOSS02_XVEL = -4;
const int SEMIBOSS03_XVEL = -3;

// Enemies
const int TOWER1_XVEL = -1;
const int BASIC_XVEL = -5;
const int SHOOTER_XVEL = -6;
const int BACHI_XVEL = -7;
const int BACHI_YVEL = 8;
const int HEAVI_XVEL = -9;

}

namespace EnemyLoader
{

bool readData(LX_FileIO::LX_File& f, EnemyData& datum);
bool generateEnemyInfo(LX_FileIO::LX_File& f, EnemyInfo& info);

bool readData(LX_FileIO::LX_File& f, EnemyData& datum)
{
    unsigned int ypos, width, height;

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

    if(f.readExactly(&ypos, sizeof(unsigned int), 1) == 0)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "EnemyLoader::readData - Cannot read the y position");
        return false;
    }

    // unsigned int → int
    datum.y = static_cast<int>(ypos);

    if(f.readExactly(&width, sizeof(unsigned int), 1) == 0)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "EnemyLoader::readData - Cannot read the width");
        return false;
    }

    // unsigned int → int
    datum.w = static_cast<int>(width);

    if(f.readExactly(&height, sizeof(unsigned int), 1) == 0)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "EnemyLoader::readData - Cannot read the height");
        return false;
    }

    // unsigned int → int
    datum.h = static_cast<int>(height);

    return true;
}

bool generateEnemyInfo(LX_FileIO::LX_File& f, EnemyInfo& info)
{
    EnemyData datum;
    const ResourceManager *rc = ResourceManager::getInstance();

    if(readData(f, datum))
    {
        LX_Graphics::LX_Sprite * texture = nullptr;

        if(datum.type < NB_ENEMIES)
            texture = rc->getResource(RC_ENEMY, datum.type);

        int glimit = Engine::getInstance()->getMaxXlim() + 1;
        info.t = datum.time;
        info._alarm = false;
        info.boss = false;

        switch(datum.type)
        {
        case 0: /// Debug case
        {
            info.e = new SemiBoss01(Rank::healthUp(datum.hp), datum.att,
                                    Rank::shieldUp(datum.sh), texture, glimit,
                                    datum.y, datum.w, datum.h, -1, 1);
        }
        break;

        case 1:
        {
            info.boss = true;
            info.e = new Boss01(Rank::healthUp(datum.hp), datum.att,
                                Rank::shieldUp(datum.sh), texture, glimit,
                                datum.y, datum.w, datum.h, BOSS01_XVEL, 0);
        }
        break;

        case 2:
        {
            info.boss = true;
            info.e = new Boss02(Rank::healthUp(datum.hp), datum.att,
                                Rank::shieldUp(datum.sh), texture, glimit,
                                datum.y, datum.w, datum.h, BOSS02_XVEL, 0);
        }
        break;

        /// @todo (#1#) Boss03
        case 4:
        {
            info.boss = true;
            info.e = new Boss04(Rank::healthUp(datum.hp), datum.att,
                                Rank::shieldUp(datum.sh), texture, glimit,
                                datum.y, datum.w, datum.h, BOSSXX_XVEL, 0);
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
                                    Rank::shieldUp(datum.sh), texture, glimit,
                                    datum.y, datum.w, datum.h, SEMIBOSS01_XVEL, 0);
        }
        break;

        case 51:
        {
            info.e = new SemiBoss02(Rank::healthUp(datum.hp), datum.att,
                                    Rank::shieldUp(datum.sh), texture, glimit,
                                    datum.y, datum.w, datum.h, SEMIBOSS02_XVEL, 0);
        }
        break;

        case 52:
        {
            info.e = new SemiBoss03(Rank::healthUp(datum.hp), datum.att,
                                    Rank::shieldUp(datum.sh), texture, glimit,
                                    datum.y, datum.w, datum.h, SEMIBOSS03_XVEL, 0);
        }
        break;

        case 100:
        {
            info.e = new Tower1(datum.hp, datum.att, datum.sh, texture,
                                glimit, datum.y, datum.w, datum.h,
                                TOWER1_XVEL, 0);
        }
        break;

        case 101:
        {
            info.e = new BasicEnemy(datum.hp, datum.att, datum.sh, texture,
                                    glimit, datum.y, datum.w, datum.h,
                                    BASIC_XVEL, 0);
        }
        break;

        case 102:
        {
            info.e = new Shooter(datum.hp, datum.att, datum.sh, texture,
                                 glimit, datum.y, datum.w, datum.h,
                                 SHOOTER_XVEL, 0);
        }
        break;

        case 103:
        {
            info.e = new Bachi(datum.hp, datum.att, datum.sh, texture,
                               glimit, datum.y, datum.w, datum.h,
                               BACHI_XVEL, BACHI_YVEL);
        }
        break;

        case 104:
        {
            info.e = new Heaviside(datum.hp, datum.att, datum.sh, texture,
                                   glimit, datum.y, datum.w, datum.h,
                                   HEAVI_XVEL, 0);
        }
        break;

        case 105:
        {
            info.e = new RHeaviside(datum.hp, datum.att, datum.sh, texture,
                                    glimit, datum.y, datum.w, datum.h,
                                    HEAVI_XVEL, 0);
        }
        break;

        case 106:
        {
            info.e = new HeavisidePurple(datum.hp, datum.att, datum.sh, texture,
                                         glimit, datum.y, datum.w, datum.h,
                                         HEAVI_XVEL, 0);
        }
        break;

        case 107:
        {
            info.e = new RHeavisidePurple(datum.hp, datum.att, datum.sh, texture,
                                          glimit, datum.y, datum.w, datum.h,
                                          HEAVI_XVEL, 0);
        }
        break;

        default:
            break;
        }

        return true;
    }

    return false;
}

void load(unsigned int id, std::deque<EnemyInfo>& q)
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
        q.push_back(info);
        j++;
        info.clean();
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
