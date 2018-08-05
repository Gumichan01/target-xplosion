
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

#include "EnemyLoader.hpp"
#include "EnemyData.hpp"
#include "ResourceManager.hpp"

#include "../game/engine/Engine.hpp"
#include "../game/Power.hpp"
#include "../asset/TX_Asset.hpp"
#include "../level/LoadingScreen.hpp"
#include "../entities/Player.hpp"
#include "../entities/BasicEnemy.hpp"
#include "../entities/Bachi.hpp"
#include "../entities/TargetShooter.hpp"
#include "../entities/Tower.hpp"
#include "../entities/Heaviside.hpp"
#include "../entities/NetShooter.hpp"
#include "../entities/Airship.hpp"
#include "../entities/Kamikaze.hpp"
#include "../entities/boss/SemiBoss01.hpp"
#include "../entities/boss/SemiBoss02.hpp"
#include "../entities/boss/SemiBoss03.hpp"
#include "../entities/boss/Boss01.hpp"
#include "../entities/boss/Boss02.hpp"
#include "../entities/boss/Boss03.hpp"
#include "../entities/boss/Boss04.hpp"

#include <Lunatix/FileIO.hpp>

using namespace lx::Win;
using namespace lx::Mixer;

namespace
{
// Bosses
const float BOSS01_XVEL = -4.0f;
const float BOSS02_XVEL = -4.0f;
const float BOSS03_XVEL = -2.0f;
const float BOSSXX_XVEL = -4.0f;

// Semibosses
const float SEMIBOSS01_XVEL = -5.0f;
const float SEMIBOSS02_XVEL = -5.0f;
const float SEMIBOSS03_XVEL = -5.0f;

// Enemies
const float TOWER1_XVEL = -1.0f;
const float BASIC_XVEL = -5.0f;
const float SHOOTER_XVEL = -6.0f;
const float BACHI_XVEL = -7.0f;
const float BACHI_YVEL = 8.0f;
const float HEAVI_XVEL = -9.0f;
const float NETSH_XVEL = -8.0f;
const float AIRSHIP_XVEL = -4.0f;
const float KAMIKAZE_VEL = -4.5f;

inline void cleanInfo( EnemyInfo& info ) noexcept
{
    info.e      = nullptr;
    info.t      = 0U;
    info.alarm = false;
    info.boss   = false;
}

}

namespace EnemyLoader
{

bool readData( lx::FileIO::File& f, EnemyData& datum ) noexcept;
bool generateEnemyInfo( lx::FileIO::File& f, EnemyInfo& info );

bool readData( lx::FileIO::File& f, EnemyData& datum ) noexcept
{
    unsigned int ypos, width, height;

    if ( f.readExactly( &datum.type, sizeof( unsigned int ), 1 ) == 0 )
    {
        lx::Log::logCritical( lx::Log::LogType::APPLICATION,
                             "EnemyLoader::readData - Cannot read the type" );
        return false;
    }

    if ( f.readExactly( &datum.hp, sizeof( unsigned int ), 1 ) == 0 )
    {
        lx::Log::logCritical( lx::Log::LogType::APPLICATION,
                             "EnemyLoader::readData - Cannot read the hp" );
        return false;
    }

    if ( f.readExactly( &datum.att, sizeof( unsigned int ), 1 ) == 0 )
    {
        lx::Log::logCritical( lx::Log::LogType::APPLICATION,
                             "EnemyLoader::readData - Cannot read att" );
        return false;
    }

    if ( f.readExactly( &datum.sh, sizeof( unsigned int ), 1 ) == 0 )
    {
        lx::Log::logCritical( lx::Log::LogType::APPLICATION,
                             "EnemyLoader::readData - Cannot read the shield" );
        return false;
    }

    if ( f.readExactly( &datum.time, sizeof( unsigned int ), 1 ) == 0 )
    {
        lx::Log::logCritical( lx::Log::LogType::APPLICATION,
                             "EnemyLoader::readData - Cannot read the time value" );
        return false;
    }

    if ( f.readExactly( &ypos, sizeof( unsigned int ), 1 ) == 0 )
    {
        lx::Log::logCritical( lx::Log::LogType::APPLICATION,
                             "EnemyLoader::readData - Cannot read the y position" );
        return false;
    }

    // unsigned int → int
    datum.y = static_cast<int>( ypos );

    if ( f.readExactly( &width, sizeof( unsigned int ), 1 ) == 0 )
    {
        lx::Log::logCritical( lx::Log::LogType::APPLICATION,
                             "EnemyLoader::readData - Cannot read the width" );
        return false;
    }

    // unsigned int → int
    datum.w = static_cast<int>( width );

    if ( f.readExactly( &height, sizeof( unsigned int ), 1 ) == 0 )
    {
        lx::Log::logCritical( lx::Log::LogType::APPLICATION,
                             "EnemyLoader::readData - Cannot read the height" );
        return false;
    }

    // unsigned int → int
    datum.h = static_cast<int>( height );
    return true;
}

bool generateEnemyInfo( lx::FileIO::File& f, EnemyInfo& info )
{
    EnemyData datum;
    const ResourceManager * rc = ResourceManager::getInstance();

    if ( readData( f, datum ) )
    {
        lx::Graphics::Sprite * texture = nullptr;

        if ( datum.type < Asset::NB_ENEMIES )
            texture = rc->getResource( RC_ENEMY, datum.type );

        int glimit = Engine::getInstance()->getMaxXlim() + 1;
        info.t = datum.time;
        info.alarm = false;
        info.boss = false;

        switch ( datum.type )
        {
        case 0: /// Debug case
        {
            info.e = new SemiBoss01( Rank::healthUp( datum.hp ), datum.att,
                                     Rank::shieldUp( datum.sh ), texture, glimit,
                                     datum.y, datum.w, datum.h, -1, 1 );
        }
        break;

        case 1:
        {
            info.boss = true;
            info.e = new Boss01( Rank::healthUp( datum.hp ), datum.att,
                                 Rank::shieldUp( datum.sh ), texture, glimit,
                                 datum.y, datum.w, datum.h, BOSS01_XVEL, 0 );
        }
        break;

        case 2:
        {
            info.boss = true;
            info.e = new Boss02( Rank::healthUp( datum.hp ), datum.att,
                                 Rank::shieldUp( datum.sh ), texture, glimit,
                                 datum.y, datum.w, datum.h, BOSS02_XVEL, 0 );
        }
        break;

        case 3:
        {
            info.boss = true;
            info.e = new Boss03( Rank::healthUp( datum.hp ), datum.att,
                                 Rank::shieldUp( datum.sh ), texture, glimit,
                                 datum.y, datum.w, datum.h, BOSS03_XVEL, 0 );
        }
        break;

        case 4:
        {
            info.boss = true;
            info.e = new Boss04( Rank::healthUp( datum.hp ), datum.att,
                                 Rank::shieldUp( datum.sh ), texture, glimit,
                                 datum.y, datum.w, datum.h, BOSSXX_XVEL, 0 );
        }
        break;

        case 22:
        {
            // Boss is coming ( T_T)
            info.alarm = true;
        }
        break;

        case 50:
        {
            info.e = new SemiBoss01( Rank::healthUp( datum.hp ), datum.att,
                                     Rank::shieldUp( datum.sh ), texture, glimit,
                                     datum.y, datum.w, datum.h, SEMIBOSS01_XVEL, 0 );
        }
        break;

        case 51:
        {
            info.e = new SemiBoss02( Rank::healthUp( datum.hp ), datum.att,
                                     Rank::shieldUp( datum.sh ), texture, glimit,
                                     datum.y, datum.w, datum.h, SEMIBOSS02_XVEL, 0 );
        }
        break;

        case 52:
        {
            info.e = new SemiBoss03( Rank::healthUp( datum.hp ), datum.att,
                                     Rank::shieldUp( datum.sh ), texture, glimit,
                                     datum.y, datum.w, datum.h, SEMIBOSS03_XVEL, 0 );
        }
        break;

        case 100:
        {
            info.e = new Tower1( datum.hp, datum.att, datum.sh, texture,
                                 glimit, datum.y, datum.w, datum.h,
                                 TOWER1_XVEL, 0 );
        }
        break;

        case 101:
        {
            info.e = new BasicEnemy( datum.hp, datum.att, datum.sh, texture,
                                     glimit, datum.y, datum.w, datum.h,
                                     BASIC_XVEL, 0 );
        }
        break;

        case 102:
        {
            info.e = new TargetShooter( datum.hp, datum.att, datum.sh, texture,
                                        glimit, datum.y, datum.w, datum.h,
                                        SHOOTER_XVEL, 0 );
        }
        break;

        case 103:
        {
            info.e = new Bachi( datum.hp, datum.att, datum.sh, texture,
                                glimit, datum.y, datum.w, datum.h,
                                BACHI_XVEL, BACHI_YVEL );
        }
        break;

        case 104:
        {
            info.e = new Heaviside( datum.hp, datum.att, datum.sh, texture,
                                    glimit, datum.y, datum.w, datum.h,
                                    HEAVI_XVEL, 0 );
        }
        break;

        case 105:
        {
            info.e = new RHeaviside( datum.hp, datum.att, datum.sh, texture,
                                     glimit, datum.y, datum.w, datum.h,
                                     HEAVI_XVEL, 0 );
        }
        break;

        case 106:
        {
            info.e = new HeavisidePurple( datum.hp, datum.att, datum.sh, texture,
                                          glimit, datum.y, datum.w, datum.h,
                                          HEAVI_XVEL, 0 );
        }
        break;

        case 107:
        {
            info.e = new RHeavisidePurple( datum.hp, datum.att, datum.sh, texture,
                                           glimit, datum.y, datum.w, datum.h,
                                           HEAVI_XVEL, 0 );
        }
        break;

        case 108:
        {
            info.e = new NetShooter( datum.hp, datum.att, datum.sh, texture,
                                     glimit, datum.y, datum.w, datum.h,
                                     NETSH_XVEL, 0 );
        }
        break;

        case 109:
        {
            info.e = new Airship( datum.hp, datum.att, datum.sh, texture,
                                  glimit, datum.y, datum.w, datum.h,
                                  AIRSHIP_XVEL, 0 );
        }
        break;

        case 110:
        {
            info.e = new Kamikaze( datum.hp, datum.att, datum.sh, texture,
                                   glimit, datum.y, datum.w, datum.h,
                                   KAMIKAZE_VEL, KAMIKAZE_VEL );
        }
        break;

        default:
            break;
        }

        return true;
    }

    return false;
}

unsigned long load( unsigned int id, std::queue<EnemyInfo>& q )
{
    const int TX_TAG = 0xCF3A1;
    lx::FileIO::File f( TX_Asset::getInstance()->getLevelPath( id ),
                          lx::FileIO::FileMode::RDONLY );

    lx::Log::logDebug( lx::Log::LogType::APPLICATION, "Level file %s : opened\n",
                      f.getFilename() );

    /// Read the tag
    int tag = 0;
    f.readExactly( &tag, sizeof( int ), 1 );

    if ( tag != TX_TAG )
    {
        f.close();
        throw lx::FileIO::IOException( "Invalid file: bad tag\n" );
    }

    /// Read the number of data
    int sz = 0;
    if ( f.readExactly( &sz, sizeof( int ), 1 ) == 0 )
    {
        f.close();
        throw lx::FileIO::IOException( lx::getError() );
    }

    lx::Log::logDebug( lx::Log::LogType::APPLICATION, "Tag: 0x%x; size: %u\n", tag, sz );

    EnemyInfo info;
    LoadingScreen lscreen;

    unsigned long j = 0UL;
    unsigned long qsize = 0UL;
    const unsigned long SZ = static_cast<unsigned long>( sz );

    /// Read data
    while ( j != SZ && generateEnemyInfo( f, info ) )
    {
        if ( !info.alarm )
            qsize += 1;

        q.push( info );
        lscreen( j, SZ );
        cleanInfo( info );
        j += 1;
    }

    f.close();

    if ( j != SZ )
    {
        std::string s = lx::getError();
        lx::Log::logCritical( lx::Log::LogType::APPLICATION,
                             "%s - Cannot read data no %d\n",
                             f.getFilename(), j );

        throw lx::FileIO::IOException( s );
    }

    lx::Log::logDebug( lx::Log::LogType::APPLICATION, "Done, level file closed\n" );
    return qsize;
}

}
