
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

#ifndef ENTITYHANDLER_HPP_INCLUDED
#define ENTITYHANDLER_HPP_INCLUDED

#include <vector>
#include <queue>

class Missile;
class PlayerRocket;
class EnemyRocket;
class Enemy;
class Item;
class Level;
class Background;
class Player;


struct GameEnv
{
    Level * level;
    Background * background;
};

class EntityHandler
{
    unsigned int start_point = 0;
    GameEnv genv{nullptr, nullptr};

    // enemy missiles
    std::queue<Missile *> missiles_queue;

    std::vector<Missile *> player_missiles;
    std::vector<Missile *> enemies_missiles;
    std::vector<Enemy *> enemies;
    std::vector<Item *> items;

    EntityHandler()  = default;
    ~EntityHandler() = default;

    void missileToScore() noexcept;

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
    void setGameEnv(GameEnv& env) noexcept;
    bool generateEnemy() noexcept;

    // Push entities
    void pushEnemyMissile(Missile& m) noexcept;
    void pushEnemy(Enemy& e) noexcept;
    void pushPlayerMissile(Missile& m) noexcept;
    void pushItem(Item& i) noexcept;

    // Internal logic (entities)
    void physics(Player& p) noexcept;
    void updateStatus(Player& p) noexcept;
    void cleanEntities() noexcept;
    void displayEntities() noexcept;

    // Shoot to kill
    void targetEnemy(PlayerRocket& pr) noexcept;
    void targetPlayer(Player& p, EnemyRocket& m) noexcept;

    // Yeah!!!
    void bulletCancel() noexcept;

    // Clean up
    void clearAll() noexcept;

    unsigned int nbEnemies() const noexcept;
};

#endif // ENTITYHANDLER_HPP_INCLUDED
