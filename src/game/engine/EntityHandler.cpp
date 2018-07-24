
/*
*   Target_Xplosion - A classic shoot'em up video game

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

#include "EntityHandler.hpp"
#include "AudioHandler.hpp"
#include "Engine.hpp"

#include "../Background.hpp"
#include "../../entities/Item.hpp"
#include "../../entities/Enemy.hpp"
#include "../../entities/Missile.hpp"
#include "../../entities/Rocket.hpp"
#include "../../entities/Player.hpp"
#include "../../resources/EnemyInfo.hpp"
#include "../../resources/EnemyInfo.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../level/Level.hpp"
#include "../../utils/misc.hpp"

#include <Lunatix/Time.hpp>
#include <Lunatix/ImgRect.hpp>
#include <Lunatix/Vector2D.hpp>

#include <algorithm>


EntityHandler::EntityHandler() : missiles_queue() {}

// Entity handler
EntityHandler& EntityHandler::getInstance() noexcept
{
    static EntityHandler singleton;
    return singleton;
}

void EntityHandler::setGameEnv( GameEnv& env ) noexcept
{
    genv.level = env.level;
    genv.background = env.background;
    start_point = lx::Time::getTicks();
}

bool EntityHandler::generateEnemy()
{
    using AudioHandler::AudioHDL;
    AudioHDL * audiohdl = AudioHDL::getInstance();

    EnemyInfo data;

    if ( genv.level == nullptr )
        return false;

    if ( genv.level->statEnemyInfo( data ) )
    {
        if ( ( lx::Time::getTicks() - start_point ) > data.t )
        {
            genv.level->popData();

            if ( data._alarm )
            {
                genv.background->setIncrease();
                audiohdl->playAlarm();
                audiohdl->playVoiceBoss();
            }
            else if ( data.boss )
                audiohdl->playBossMusic();

            if ( data.e != nullptr )
            {
                enemies.push_back( data.e );
                data.e->start();
            }

            return true;
        }
    }
    return false;
}

void EntityHandler::pushEnemyMissile( Missile& m )
{
    missiles_queue.push( &m );
}

void EntityHandler::pushEnemy( Enemy& e )
{
    enemies.push_back( &e );
}

void EntityHandler::pushPlayerMissile( Missile& m )
{
    player_missiles.push_back( &m );
}

void EntityHandler::pushItem( Item& i )
{
    items.push_back( &i );
}


void EntityHandler::physics( Player& p ) noexcept
{
    // That does not make sense to check collisions if the player is dead
    if ( !p.isDead() )
    {
        // Don't test collision between a dying player and an item
        if ( !p.isDying() )
        {
            for ( Item * i : items )
            {
                p.collision( i );
            }
        }

        for ( Enemy * e : enemies )
        {
            // enemy/player collision
            e->collision( &p );

            if ( e->isDead() )
                continue;

            // enemy/missile collision
            for ( Missile * m : player_missiles )
            {
                e->collision( m );
            }
        }

        for ( Missile * m : enemies_missiles )
        {
            p.collision( m );
        }
    }
}

void EntityHandler::updateStatus( Player& p ) noexcept
{
    p.status();
    itemStatus();
    missileStatus();
    enemyStatus();
}

void EntityHandler::itemStatus() noexcept
{
    for ( Item * i : items )
    {
        if ( i->getX() > ( -( i->getWidth() ) ) )
            i->move();
        else
            i->die();
    }
}

void EntityHandler::missileStatus() noexcept
{
    auto fstatus = [] ( Missile * m )
    {
        if ( Engine::outOfBound( m->getHitbox() ) || m->explosion() )
            m->die();
        else
            m->move();
    };

    while ( !missiles_queue.empty() )
    {
        enemies_missiles.push_back( missiles_queue.front() );
        missiles_queue.pop();
    }

    std::for_each( player_missiles.begin(), player_missiles.end(), fstatus );
    std::for_each( enemies_missiles.begin(), enemies_missiles.end(), fstatus );

}

void EntityHandler::enemyStatus() noexcept
{
    for ( Enemy * e : enemies )
    {
        if ( e->getX() <= ( -( e->getWidth() ) - 1 ) )
            e->die();
        else
            e->strategy();
    }
}


void EntityHandler::cleanEntities()
{
    // Items
    for ( size_t l = 0; l != items.size(); l++ )
    {
        if ( ( items[l]->getX() < -( items[l]->getWidth() ) ) || items[l]->isDead() )
        {
            delete items[l];
            items[l] = nullptr;
        }
    }

    // Missiles of the player
    for ( size_t i = 0; i != player_missiles.size() ; i++ )
    {
        if ( player_missiles[i] == nullptr || player_missiles[i]->isDead() )
        {
            delete player_missiles[i];
            player_missiles[i] = nullptr;
        }
    }

    // Missiles of enemies
    for ( size_t k = 0; k != enemies_missiles.size(); k++ )
    {
        if ( enemies_missiles[k] == nullptr || enemies_missiles[k]->isDead() )
        {
            delete enemies_missiles[k];
            enemies_missiles[k] = nullptr;
        }
    }

    // Enemies
    for ( size_t j = 0; j != enemies.size(); j++ )
    {
        if ( enemies[j]->isDead() )
        {
            delete enemies[j];
            enemies[j] = nullptr;
        }
    }

    // Remove null pointers
    items.erase( misc::remove( items.begin(), items.end(), nullptr ), items.end() );
    player_missiles.erase( misc::remove( player_missiles.begin(), player_missiles.end(), nullptr ),
                           player_missiles.end() );
    enemies_missiles.erase( misc::remove( enemies_missiles.begin(), enemies_missiles.end(), nullptr ),
                            enemies_missiles.end() );
    enemies.erase( misc::remove( enemies.begin(), enemies.end(), nullptr ), enemies.end() );
}

void EntityHandler::displayEntities()
{
    const auto display_ = [] ( Entity * t )
    {
        t->draw();
    };
    std::for_each( items.begin(), items.end(), display_ );
    std::for_each( enemies.begin(), enemies.end(), display_ );
    std::for_each( player_missiles.begin(), player_missiles.end(), display_ );
    std::for_each( player_missiles.begin(), player_missiles.end(), display_ );
    std::for_each( enemies_missiles.begin(), enemies_missiles.end(), display_ );
}


void EntityHandler::targetEnemy( PlayerRocket& pr ) noexcept
{
    if ( !enemies.empty() )
    {
        const int MIN_DISTANCE = 2048;
        const int XREL = pr.getX() + pr.getWidth();

        Enemy * closest = nullptr;
        int min_d = MIN_DISTANCE;

        for ( Enemy * e : enemies )
        {
            if ( e == nullptr || e->isDying() )
                continue;

            int t = e->getX() + e->getWidth() + Rocket::ROCKET_RANGE - XREL;

            if ( t > 0 && t < min_d )
            {
                min_d = t;
                closest = e;
            }
        }

        if ( closest != nullptr )
            pr.visit( *closest );
    }
}

void EntityHandler::targetPlayer( Player& p, EnemyRocket& m ) noexcept
{
    int delta = m.getX() - p.getX();

    if ( !p.isDead() && !p.isDying() && delta > 0 )
    {
        m.visit( p );
    }
}

// private
void EntityHandler::missileToScore()
{
    for ( auto em : enemies_missiles )
    {
        items.push_back( new Item( em->getX(), em->getY() ) );
    }
}

void EntityHandler::bulletCancel() noexcept
{
    missileToScore();
    clearEnemyMissiles();
}


// Clean all objects
void EntityHandler::clearAll() noexcept
{
    clearPlayerMissiles();
    clearEnemyMissiles();
    clearEnemies();
    clearItems();
}

void EntityHandler::clearPlayerMissiles() noexcept
{
    // Player's missiles
    for ( size_t i = 0; i != player_missiles.size(); i++ )
    {
        delete player_missiles[i];
        player_missiles.erase( player_missiles.begin() + i );
        i--;
    }
}

void EntityHandler::clearEnemyMissiles() noexcept
{
    // Enemies missiles
    for ( size_t k = 0; k != enemies_missiles.size(); k++ )
    {
        delete enemies_missiles[k];
        enemies_missiles.erase( enemies_missiles.begin() + k );
        k--;
    }

    while ( !missiles_queue.empty() )
    {
        Missile * m = missiles_queue.front();
        missiles_queue.pop();
        delete m;
    }
}

void EntityHandler::clearEnemies() noexcept
{
    // Enemies
    for ( size_t j = 0; j != enemies.size(); j++ )
    {
        delete enemies[j];
        enemies.erase( enemies.begin() + j );
        j--;
    }
}

void EntityHandler::clearItems() noexcept
{
    // Items
    for ( size_t l = 0; l != items.size(); l++ )
    {
        delete items[l];
        items.erase( items.begin() + l );
        l--;
    }
}

unsigned int EntityHandler::nbEnemies() const noexcept
{
    return enemies.size();
}

// Playe handler

PlayerHandler::~PlayerHandler()
{
    delete player;
}

PlayerHandler& PlayerHandler::getInstance() noexcept
{
    static PlayerHandler singleton;
    return singleton;
}

void PlayerHandler::setPlayer( const PlayerParam& param )
{
    using lx::Graphics::Sprite;
    lx::Graphics::ImgRect rect{param.x, param.y, param.w, param.h};
    lx::Physics::Vector2D vec{param.vx, param.vy};
    Sprite * sp = ResourceManager::getInstance()->getPlayerResource();

    delete player;
    player = new Player( param.hp, param.att, param.sh, param.critic, sp, rect,
                         vec );
}

const Player& PlayerHandler::getPlayerConst() const noexcept
{
    return *player;
}

Player& PlayerHandler::getPlayer() noexcept
{
    return *player;
}
