
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

class Missile;
class PlayerRocket;
class Enemy;
class Item;


class EntityHandler
{
    std::queue<Missile *> emissiles_queue;

    std::vector<Missile *> player_missiles;
    std::vector<Missile *> enemies_missiles;
    std::vector<Enemy *> enemies;
    std::vector<Item *> items;

    EntityHandler()  = default;
    ~EntityHandler() = default;

    void missileToScore();

public:

    static EntityHandler& getInstance() noexcept;

    bool generateEnemy();

    // Push entities
    void pushEnemyMissile(Missile * m);
    void pushEnemy(Enemy *e);
    void pushPlayerMissile(Missile * m);
    void pushItem(Item * y);

    // Shoot to kill
    void targetEnemy(PlayerRocket * m);
    void targetPlayer(EnemyRocket * m);

    // Yeah!!!
    void bulletCancel();

    // Clean up
    void clearVectors();
    void clearPlayerMissiles();
    void clearEnemyMissiles();
    void clearEnemies();
    void clearItems();

};

#endif // ENTITYHANDLER_HPP_INCLUDED
