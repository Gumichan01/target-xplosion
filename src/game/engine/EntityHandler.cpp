
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
#include "../../level/Level.hpp"

#include <LunatiX/LX_Timer.hpp>

#include <algorithm>


// Entity handler
EntityHandler& EntityHandler::getInstance() noexcept
{
    static EntityHandler singleton;
    return singleton;
}

void EntityHandler::setGameEnv(GameEnv& env) noexcept
{
    genv.level = env.level;
    genv.background = env.background;
    start_point = LX_Timer::getTicks();
}

bool EntityHandler::generateEnemy() noexcept
{
    using AudioHandler::AudioHDL;
    AudioHDL * audiohdl = AudioHDL::getInstance();

    EnemyInfo data;

    if(genv.level == nullptr)
        return false;

    if(genv.level->statEnemyInfo(data))
    {
        if((LX_Timer::getTicks() - start_point) > data.t)
        {
            genv.level->popData();

            if(data._alarm)
            {
                genv.background->setIncrease();
                audiohdl->playAlarm();
                audiohdl->playVoiceBoss();
            }
            else if(data.boss)
                audiohdl->playBossMusic();

            if(data.e != nullptr)
            {
                enemies.push_back(data.e);
                data.e->start();
            }

            return true;
        }
    }
    return false;
}

void EntityHandler::pushEnemyMissile(Missile& m) noexcept
{
    missiles_queue.push(&m);
}

void EntityHandler::pushEnemy(Enemy& e) noexcept
{
    enemies.push_back(&e);
}

void EntityHandler::pushPlayerMissile(Missile& m) noexcept
{
    player_missiles.push_back(&m);
}

void EntityHandler::pushItem(Item& i) noexcept
{
    items.push_back(&i);
}


void EntityHandler::physics(Player& p) noexcept
{
    // That does not make sense to check collisions if the player is dead
    if(!p.isDead())
    {
        // Don't test collision between a dying player and an item
        if(!p.isDying())
        {
            for(Item * i : items)
            {
                p.collision(i);
            }
        }

        for(Enemy * e: enemies)
        {
            // enemy/player collision
            e->collision(&p);

            if(e->isDead())
                continue;

            // enemy/missile collision
            for(Missile * m : player_missiles)
            {
                e->collision(m);
            }
        }

        for(Missile * m : enemies_missiles)
        {
            p.collision(m);
        }
    }
}

void EntityHandler::updateStatus(Player& p) noexcept
{
    static uint32_t death_start = 0;
    const uint32_t DELAY_TO_REBORN = 2000;

    /// @todo Move the player (the player must handle it TODO)
    if(!p.isDead())
    {
        p.move();
        p.checkLaserShot();
        death_start = LX_Timer::getTicks();
    }
    else
    {
        if((LX_Timer::getTicks() - death_start) > DELAY_TO_REBORN)
            p.reborn();
    }
    /// end todo

    // Move the items
    for(Item * i : items)
    {
        if(i->getX() > (-(i->getWidth())))
            i->move();
        else
            i->die();
    }

    // Move the missiles of the player
    for(Missile * pm: player_missiles)
    {
        if(Engine::outOfBound(pm->getHitbox()) || pm->explosion())
            pm->die();
        else
            pm->move();
    }

    /// @todo In another function
    while(!missiles_queue.empty())
    {
        enemies_missiles.push_back(missiles_queue.front());
        missiles_queue.pop();
    }
    /// end todo

    // Move the missiles of enemies
    for(Missile * em: enemies_missiles)
    {
        if(em == nullptr) // unreachable?
            continue;

        if(Engine::outOfBound(em->getHitbox()) || em->explosion())
            em->die();
        else
            em->move();
    }

    // The enemy strategy
    for(Enemy * e : enemies)
    {
        if(e->getX() <= (-(e->getWidth()) -1))
            e->die();
        else
            e->strategy();
    }
}

void EntityHandler::cleanEntities() noexcept
{
    // Items
    for(size_t l = 0; l != items.size(); l++)
    {
        if((items[l]->getX() < (-(items[l]->getWidth())) ) || items[l]->isDead())
        {
            delete items[l];
            items.erase(items.begin() + l);
            l--;
        }
    }

    // Missiles of the player
    for(size_t i = 0; i != player_missiles.size() ; i++)
    {
        if(player_missiles[i] == nullptr || player_missiles[i]->isDead())
        {
            delete player_missiles[i];
            player_missiles.erase(player_missiles.begin() + i);
            i--;
        }
    }

    // Missiles of enemies
    for(size_t k = 0; k != enemies_missiles.size(); k++)
    {
        if(enemies_missiles[k] == nullptr || enemies_missiles[k]->isDead())
        {
            delete enemies_missiles[k];
            enemies_missiles.erase(enemies_missiles.begin() + k);
            k--;
        }
    }

    // Enemies
    for(size_t j = 0; j != enemies.size(); j++)
    {
        if(enemies[j]->isDead())
        {
            delete enemies[j];
            enemies.erase(enemies.begin() + j);
            j--;
        }
    }
}

void EntityHandler::displayEntities() noexcept
{
    const auto display_ = [] (Entity * t)
    {
        t->draw();
    };
    std::for_each(items.begin(),items.end(), display_);
    std::for_each(enemies.begin(), enemies.end(), display_);
    std::for_each(player_missiles.begin(), player_missiles.end(), display_);
    std::for_each(player_missiles.begin(), player_missiles.end(), display_);
    std::for_each(enemies_missiles.begin(), enemies_missiles.end(), display_);
}


void EntityHandler::targetEnemy(PlayerRocket& pr) noexcept
{
    if(!enemies.empty())
    {
        const int MIN_DISTANCE = 2048;
        const int XREL = pr.getX() + pr.getWidth();

        Enemy * closest = nullptr;
        int min_d = MIN_DISTANCE;

        for(Enemy * e: enemies)
        {
            if(e == nullptr || e->isDying())
                continue;

            int t = e->getX() + e->getWidth() + Rocket::ROCKET_RANGE - XREL;

            if(t > 0 && t < min_d)
            {
                min_d = t;
                closest = e;
            }
        }

        if(closest != nullptr)
            pr.visit(closest);
    }
}

void EntityHandler::targetPlayer(Player& p, EnemyRocket& m) noexcept
{
    int delta = m.getX() - p.getX();

    if(!p.isDead() && !p.isDying() && delta > 0)
    {
        m.visit(&p);
    }
}

// private
void EntityHandler::missileToScore() noexcept
{
    for(auto em : enemies_missiles)
    {
        items.push_back(new Item(em->getX(), em->getY()));
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
    for(size_t i = 0; i != player_missiles.size(); i++)
    {
        delete player_missiles[i];
        player_missiles.erase(player_missiles.begin() + i);
        i--;
    }
}

void EntityHandler::clearEnemyMissiles() noexcept
{
    // Enemies missiles
    for(size_t k = 0; k != enemies_missiles.size(); k++)
    {
        delete enemies_missiles[k];
        enemies_missiles.erase(enemies_missiles.begin() + k);
        k--;
    }

    while(!missiles_queue.empty())
    {
        Missile *m = missiles_queue.front();
        missiles_queue.pop();
        delete m;
    }
}

void EntityHandler::clearEnemies() noexcept
{
    // Enemies
    for(size_t j = 0; j != enemies.size(); j++)
    {
        delete enemies[j];
        enemies.erase(enemies.begin() + j);
        j--;
    }
}

void EntityHandler::clearItems() noexcept
{
    // Items
    for(size_t l = 0; l != items.size(); l++)
    {
        delete items[l];
        items.erase(items.begin() + l);
        l--;
    }
}

unsigned int EntityHandler::nbEnemies() const noexcept
{
    return enemies.size();
}
