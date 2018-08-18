
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
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#ifndef ENTITYHANDLER_HPP_INCLUDED
#define ENTITYHANDLER_HPP_INCLUDED

#include <Lunatix/Time.hpp>

#include <algorithm>
#include <vector>
#include <queue>

class Player;
class Missile;
class PlayerRocket;
class EnemyRocket;
class Enemy;
class Item;
class Level;
class Background;
class Player;


struct GameEnv final
{
    Level * level;
    Background * background;
};


class EntityHandler final
{
    lx::Time::Timer timer;
    GameEnv genv{ nullptr, nullptr };

    // This queue must contain only enemy missiles
    std::queue<Missile *> missiles_queue = std::queue<Missile *> {};

    std::vector<Missile *> player_missiles = {};
    std::vector<Missile *> enemies_missiles = {};
    std::vector<Enemy *> enemies = {};
    std::vector<Item *> items = {};

    EntityHandler();
    EntityHandler( const EntityHandler& ) = delete;
    EntityHandler& operator =( const EntityHandler& ) = delete;
    ~EntityHandler() = default;

    void missileToScore();

    void itemStatus() noexcept;
    void missileStatus() noexcept;
    void enemyStatus() noexcept;

    void clearPlayerMissiles() noexcept;
    void clearEnemyMissiles() noexcept;
    void clearEnemies() noexcept;
    void clearItems() noexcept;

public:

    static EntityHandler& getInstance() noexcept;

    /*  It must know what level the game is playing,
        so it can get information about where enemies are generated */
    void setGameEnv( GameEnv& env ) noexcept;
    bool generateEnemy();

    // Push entities
    void pushEnemyMissile( Missile& m );
    void pushEnemy( Enemy& e );
    void pushPlayerMissile( Missile& m );
    void pushItem( Item& i );

    // Internal logic (entities)
    void physics( Player& p ) noexcept;
    void updateStatus( Player& p ) noexcept;
    void cleanEntities();
    void displayEntities();

    // Shoot to kill
    void targetEnemy( PlayerRocket& pr ) noexcept;
    void targetPlayer( Player& p, EnemyRocket& m ) noexcept;

    void bulletCancel() noexcept;
    void clearAll() noexcept;

    unsigned int nbEnemies() const noexcept;
};

struct PlayerParam final
{
    unsigned int hp  = 0;
    unsigned int att = 0;
    unsigned int sh  = 0;
    unsigned int critic = 0;
    int x = 0, y = 0, w = 0, h = 0;
    float vx = 0.0f, vy = 0.0f;
};

class PlayerHandler final
{
    Player * player = nullptr;

    PlayerHandler( const PlayerHandler& ) = delete;
    PlayerHandler& operator =( const PlayerHandler& ) = delete;

public:

    static PlayerHandler& getInstance() noexcept;

    PlayerHandler() = default;
    void setPlayer( const PlayerParam& param );
    Player& getPlayer() noexcept;
    ~PlayerHandler();
};

#endif // ENTITYHANDLER_HPP_INCLUDED
