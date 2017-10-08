
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

// Engine
#include "Engine.hpp"
#include "Hud.hpp"
#include "Framerate.hpp"
#include "PlayerInput.hpp"
#include "AudioHandler.hpp"

// Game
#include "../Result.hpp"
#include "../Background.hpp"
#include "../Scoring.hpp"

// Entities
#include "../../entities/Item.hpp"
#include "../../entities/Player.hpp"
#include "../../entities/Enemy.hpp"
#include "../../entities/Missile.hpp"
#include "../../entities/Rocket.hpp"
#include "../../entities/Bomb.hpp"

// Data
#include "../../level/Level.hpp"
#include "../../resources/EnemyInfo.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../resources/WinID.hpp"
#include "../../asset/TX_Asset.hpp"

// Including some header files of the engine
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Mixer.hpp>
#include <LunatiX/LX_Device.hpp>
#include <LunatiX/LX_Log.hpp>
#include <LunatiX/LX_Timer.hpp>

#include <algorithm>
#include <sstream>

using namespace Result;
using namespace AudioHandler;
using namespace LX_Physics;
using namespace LX_Device;
using namespace LX_Win;

namespace
{
const int GAME_X_OFFSET = -128;
const int GAME_Y_OFFSET = 8;
const int GAME_YMIN = 68;
// Viewport
const int GAME_VPORT_H = 68;
// Fading
const int FADE_MAX_VALUE = 255;

// Player
const unsigned int MIN_HEALTH_POINTS = 100;
const unsigned int MIN_ATTACK = 20;
const unsigned int MIN_DEFENSE = 12;
const unsigned int MIN_CRITIC = 3;

// Load the important ressources
void loadRessources()
{
    Bomb::loadExplosionBuffer();
    Missile::loadExplosionBuffer();
    Item::createItemRessources();
    Enemy::loadExplosionBuffer();
}

// Free all ressources
void freeRessources()
{
    Bomb::destroyExplosionBuffer();
    Missile::destroyExplosionBuffer();
    Item::destroyItemRessources();
    Enemy::destroyExplosionBuffer();
}

}

// Internal variables
int Engine::game_minXlimit = 0;
int Engine::game_maxXlimit = 0;
int Engine::game_minYlimit = 0;
int Engine::game_maxYlimit = 0;
uint8_t Engine::fade_out_counter = 0;
static Engine *game_instance = nullptr;
// The height of the background
// if the Y limit of the Engine (on screen)
const int BG_WIDTH = 1600;


Engine::Engine()
    : game_state(EngineStatusV::GAME_RUNNING), start_point(0),
      end_of_level(false), player(nullptr), game_item(nullptr),
      bgm(nullptr), level(nullptr), score(nullptr), bg(nullptr),
      audiohdl(nullptr), resources(nullptr), gw(nullptr)
{
    score = new Score();
    resources = ResourceManager::getInstance();
    gw = LX_WindowManager::getInstance()->getWindow(WinID::getWinID());

    game_minXlimit = 0;
    game_maxXlimit = gw->getLogicalWidth();
    game_minYlimit = GAME_YMIN;
    game_maxYlimit = gw->getLogicalHeight();
}


Engine * Engine::init()
{
    if(game_instance == nullptr)
        game_instance = new Engine();

    return game_instance;
}

Engine * Engine::getInstance()
{
    return game_instance;
}

void Engine::destroy()
{
    delete game_instance;
    game_instance = nullptr;
}

Engine::~Engine()
{
    delete player;
}

bool Engine::outOfBound(const LX_AABB& pos)
{
    return (pos.x < (-pos.w + GAME_X_OFFSET) || pos.x > game_maxXlimit
            || pos.y < (-pos.h - GAME_Y_OFFSET)
            || pos.y > game_maxYlimit + GAME_Y_OFFSET);
}


int Engine::getMinXlim()
{
    return game_minXlimit;
}

int Engine::getMaxXlim()
{
    return game_maxXlimit;
}

int Engine::getMinYlim()
{
    return game_minYlimit;
}

int Engine::getMaxYlim()
{
    return game_maxYlimit;
}


void Engine::createPlayer(unsigned int hp, unsigned int att, unsigned int sh,
                          unsigned int critic)
{
    LX_AABB ppos;
    ppos.x = game_maxXlimit / 2 - Player::PLAYER_WIDTH / 2;
    ppos.y = game_maxYlimit / 2 - Player::PLAYER_HEIGHT / 2;
    ppos.w = Player::PLAYER_WIDTH;
    ppos.h = Player::PLAYER_HEIGHT;

    LX_Vector2D pvel(0.0f, 0.0f);
    LX_Graphics::LX_Sprite *psprite = resources->getPlayerResource();

    delete player;
    player = new Player(hp, att, sh, critic, psprite, ppos, pvel, game_maxXlimit, game_maxYlimit);
}


bool Engine::loadLevel(const unsigned int lvl)
{
    unsigned int hp, att, def, critic;

    // Whatever what you are doing, load ressources first !!!
    loadRessources();
    end_of_level = false;

    {
        LX_Window *w = LX_WindowManager::getInstance()->getWindow(WinID::getWinID());
        LX_AABB load_screen = {0,0, w->getLogicalWidth(), w->getLogicalHeight()};

        // The player's skills
        hp = MIN_HEALTH_POINTS;
        att = MIN_ATTACK;
        def = MIN_DEFENSE;
        critic = MIN_CRITIC;

        // Game
        player_missiles.reserve(DEFAULT_RESERVE);
        enemies_missiles.reserve(ENEMY_MISSILES_RESERVE);
        enemies.reserve(ENEMY_RESERVE);

        w->clearWindow();
        w->fillRect(load_screen);
        w->update();
        LX_Timer::delay(33);

        level = new Level(lvl);
    }

    // Level loaded
    bgm = new BGM(lvl);
    setBackground(lvl);
    audiohdl = AudioHDL::init(lvl);

    if(lvl != 0)
    {
        hp *= lvl;
        att *= lvl;
        def *= lvl;
        critic *= lvl;
    }

    createPlayer(hp, att, def, critic);
    return true;
}


void Engine::endLevel()
{
    delete bg;
    delete bgm;
    delete level;
    delete game_item;
    AudioHDL::destroy();

    game_item = nullptr;
    bg = nullptr;
    level = nullptr;
    audiohdl = nullptr;

    huds.clear();
    freeRessources();
}


EngineStatusV Engine::loop(ResultInfo& info)
{
    const unsigned long nb_enemies = level->numberOfEnemies();
    EngineStatusV game_status;
    bool done = false;

    /// Debug mode
    if(LX_Log::isDebugMode())
    {
        LX_Mixer::setOverallVolume(OV_VOLUME);
        LX_Mixer::setMusicVolume(MUSIC_VOLUME);
        LX_Mixer::setFXVolume(FX_VOLUME);
    }

    audiohdl->playMainMusic();

    LX_Device::mouseCursorDisplay(LX_MOUSE_HIDE);
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION, "Allocated channels: %d",
                     LX_Mixer::allocateChannels(-1));
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION, "Number of enemies: %u",
                     nb_enemies + (level->hasBossParts() ? 1 : 0));

    while(!done && !end_of_level)
    {
        if((done = input()) == true)
            continue;

        createItem();
        physics();
        status();
        clean();
        display();
        while(generateEnemy());

        // Framerate regulation
        Framerate::regulate();

        if(LX_Log::isDebugMode())
        {
            Framerate::cycle();
            debugInfo();
        }
    }

    // A this point, the game is over
    LX_Device::mouseCursorDisplay(LX_MOUSE_SHOW);
    audiohdl->stopMainMusic();
    clearVectors();

    // Status of the game
    if(end_of_level)
    {
        game_status = GAME_FINISH;
        generateResult(info);
        info.max_nb_enemies = nb_enemies + (level->hasBossParts() ? 1 : 0);
    }
    else
        game_status = GAME_QUIT;

    return game_status;
}


EngineStatusV Engine::play(ResultInfo& info, unsigned int lvl)
{
    fade_out_counter = 0;

    if(loadLevel(lvl))
    {
        score->resetScore();
        start_point = LX_Timer::getTicks();
        game_state = loop(info);
        endLevel();
    }
    else
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "Cannot load the level #%u", lvl);
    return game_state;
}


void Engine::debugInfo()
{
    static uint32_t SECOND = 1000;
    static uint32_t previous_time = 0;

    if(LX_Timer::getTicks() - previous_time >= SECOND)
    {
        previous_time = LX_Timer::getTicks();
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION, "Enemies: %u\n", enemies.size());
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION, "Enemy missiles: %u\n",
                         enemies_missiles.size());
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION, "Player's missiles: %u\n",
                         player_missiles.size());
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION, "Death(s): %d\n\n",
                         player->nb_death());
    }
}


void Engine::generateResult(ResultInfo& info) const
{
    info.level = level->getLevelNum();
    info.nb_death = player->nb_death();
    info.score = score->getCurrentScore();
    info.nb_killed_enemies = score->getKilledEnemies();
    info.max_combo = score->getMaxCombo();
    info.max_nb_enemies = 0;
}

bool Engine::input()
{
    bool is_done = false;
    PlayerInput::input(*player, is_done);
    return is_done;
}

void Engine::acceptEnemyMissile(Missile *m)
{
    /// @todo add a queue
    emissiles_queue.push(m);
    //enemies_missiles.push_back(m);
}

void Engine::acceptEnemy(Enemy *e)
{
    enemies.push_back(e);
}

void Engine::acceptPlayerMissile(Missile *m)
{
    player_missiles.push_back(m);
}

void Engine::targetEnemy(PlayerRocket * m)
{
    if(!enemies.empty())
    {
        const int MIN_DISTANCE = 10000;
        const int XREL = m->getX() + m->getWidth();
        const auto it_end = enemies.end();

        auto it_result = it_end;
        int min_d = MIN_DISTANCE;

        for(auto it = enemies.begin(); it != it_end; ++it)
        {
            if((*it) == nullptr) continue;
            int t = (*it)->getX() + (*it)->getWidth() + Rocket::ROCKET_RANGE - XREL;

            if(t > 0 && t < min_d && !(*it)->isDying())
            {
                min_d = t;
                it_result= it;
            }
        }

        if(it_result != it_end)
            m->visit(*it_result);
    }
}

void Engine::targetPlayer(EnemyRocket * m)
{
    int delta = m->getX() - player->getX();

    if(!player->isDead() && !player->isDying() && delta > 0)
    {
        m->visit(player);
    }
}

void Engine::acceptItem(Item * y)
{
    items.push_back(y);
}

void Engine::acceptHUD(HUD * h)
{
    size_t found = huds.size();
    for(size_t i = 0; i < huds.size(); i++)
    {
        if(huds[i] == h)
        {
            found = i;
            break;
        }
    }

    if(found == huds.size())
        huds.push_back(h);
    else
        huds[found] = nullptr;
}


void Engine::setBackground(unsigned int lvl)
{
    const int SPEED_BG = -4;
    LX_AABB box = {0, 0, BG_WIDTH, game_maxYlimit};
    bg = new Background(lvl, box, SPEED_BG);
}

// Create a new item only if it does not exist
void Engine::createItem()
{
    if(game_item == nullptr)
        game_item = new Item();
}

// Destroy the item
void Engine::destroyItem()
{
    if(game_item->isDead() || game_item->getPowerUp() == POWER_UP::NOPOW)
    {
        delete game_item;
        game_item = nullptr;
    }
}

// Clean all objects
void Engine::clearVectors()
{
    clearPlayerMissiles();
    clearEnemyMissiles();
    clearEnemies();
    clearItems();
}

void Engine::clearPlayerMissiles()
{
    // Player's missiles
    for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size(); i++)
    {
        delete player_missiles[i];
        player_missiles.erase(player_missiles.begin() + i);
        i--;
    }
}

void Engine::clearEnemyMissiles()
{
    // Enemies missiles
    for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size(); k++)
    {
        delete enemies_missiles[k];
        enemies_missiles.erase(enemies_missiles.begin() + k);
        k--;
    }

    while(!emissiles_queue.empty())
    {
        Missile *m = emissiles_queue.front();
        emissiles_queue.pop();
        delete m;
    }
}

void Engine::clearEnemies()
{
    // Enemies
    for(std::vector<Enemy *>::size_type j = 0; j != enemies.size(); j++)
    {
        delete enemies[j];
        enemies.erase(enemies.begin() + j);
        j--;
    }
}

void Engine::clearItems()
{
    // Items
    for(std::vector<Item *>::size_type l = 0; l != items.size(); l++)
    {
        delete items[l];
        items.erase(items.begin() + l);
        l--;
    }
}

void Engine::bulletCancel()
{
    missileToScore();
    clearEnemyMissiles();
}

void Engine::missileToScore()
{
    for(auto m_it = enemies_missiles.begin(); m_it != enemies_missiles.end(); ++m_it)
    {
        items.push_back(new Item((*m_it)->getX(), (*m_it)->getY()));
    }
}


void Engine::physics()
{
    if(!player->isDead() && !player->isDying())
    {
        if(game_item != nullptr)
            player->collision(game_item);

        for(Item * i : items)
        {
            player->collision(i);
        }
    }

    for(Enemy * e: enemies)
    {
        // enemy/player collision
        if(!player->isDead())
            e->collision(player);

        if(e->isDead())
            continue;

        // enemy/missile collision
        for(Missile * m : player_missiles)
        {
            e->collision(m);
        }
    }

    if(!player->isDead())
    {
        for(Missile * m : enemies_missiles)
        {
            player->collision(m);
        }
    }
}

void Engine::status()
{
    static uint32_t death_start = 0;
    const uint32_t DELAY_TO_REBORN = 2000;

    if(game_item->getX() <= (-(game_item->getWidth()) - 1))
    {
        game_item->die();
    }
    else if(!game_item->isDead())
        game_item->move();

    // Move the items
    for(Item * i : items)
    {
        if(i->getX() > (-(i->getWidth())))
            i->move();
        else
            i->die();
    }

    // Move the player
    if(!player->isDead())
    {
        player->move();
        player->checkLaserShot();
        death_start = LX_Timer::getTicks();
    }
    else
    {
        if((LX_Timer::getTicks() - death_start) > DELAY_TO_REBORN)
            player->reborn();
    }

    // Move the missiles of the player
    for(Missile * pm: player_missiles)
    {
        if(pm->getX() >= game_maxXlimit || pm->explosion())
            pm->die();
        else
            pm->move();
    }

    while(!emissiles_queue.empty())
    {
        enemies_missiles.push_back(emissiles_queue.front());
        emissiles_queue.pop();
    }

    // Move the missiles of enemies
    for(Missile * em: enemies_missiles)
    {
        if(em == nullptr)
            continue;

        if(outOfBound(em->getHitbox()) || em->explosion())
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

void Engine::clean()
{
    destroyItem();

    // Items
    for(std::vector<Item *>::size_type l = 0; l != items.size(); l++)
    {
        if((items[l]->getX() < (-(items[l]->getWidth())) ) || items[l]->isDead())
        {
            delete items[l];
            items.erase(items.begin() + l);
            l--;
        }
    }

    // Missiles of the player
    for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size() ; i++)
    {
        if(player_missiles[i] == nullptr || player_missiles[i]->isDead())
        {
            delete player_missiles[i];
            player_missiles.erase(player_missiles.begin() + i);
            i--;
        }
    }

    // Missiles of enemies
    for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size(); k++)
    {
        if(enemies_missiles[k] == nullptr || enemies_missiles[k]->isDead())
        {
            delete enemies_missiles[k];
            enemies_missiles.erase(enemies_missiles.begin() + k);
            k--;
        }
    }

    // Enemies
    for(std::vector<Enemy *>::size_type j = 0; j != enemies.size(); j++)
    {
        if(enemies[j]->isDead())
        {
            delete enemies[j];
            enemies.erase(enemies.begin() + j);
            j--;
        }
    }
}


void Engine::displayHUD() const
{
    LX_AABB viewport = {0, 0, game_maxXlimit, GAME_VPORT_H};
    const LX_AABB cvport = viewport;
    LX_Colour bcolour = {0,0,0,64};

    bgm->displayHUD();
    gw->setViewPort(&viewport);
    gw->setDrawColour(bcolour);
    gw->fillRect(cvport);
    score->displayHUD();

    for(HUD * hud : huds)
    {
        if(hud != nullptr) hud->displayHUD();
    }
}

void Engine::updateHUD()
{
    LX_Colour colour = {0, 0, 0, fade_out_counter};
    LX_AABB box = {0, 0, game_maxXlimit, game_maxYlimit};

    if(enemies.size() == 0 && level->numberOfEnemies() == 0)
    {
        if(fade_out_counter < FADE_MAX_VALUE)
        {
            gw->setDrawColour(colour);
            fade_out_counter++;
            gw->fillRect(box);
        }
        else
        {
            fade_out_counter = 0;
            end_of_level = true;
            gw->clearWindow();
        }
    }
    else
        displayHUD();
}

// In loop
void Engine::display()
{
    gw->clearWindow();
    bg->update();
    displayItems();
    displayEnemies();
    displayPlayerMissiles();
    displayEnemyMissiles();

    // Display the item
    if(game_item != nullptr)
        game_item->draw();

    player->draw();
    updateHUD();
    gw->update();
    gw->setViewPort(nullptr);
}

void Engine::displayPlayerMissiles() const
{
    for(Missile * pm : player_missiles)
    {
        pm->draw();
    }
}

void Engine::displayItems() const
{
    for(Item * i : items)
    {
        i->draw();
    }
}

void Engine::displayEnemies() const
{
    for(Enemy * e : enemies)
    {
        if(e != nullptr && e->getX() < game_maxXlimit)
            e->draw();
    }
}

void Engine::displayEnemyMissiles() const
{
    for(Missile * em : enemies_missiles)
    {
        em->draw();
    }
}


bool Engine::generateEnemy()
{
    EnemyInfo data;

    if(level->statEnemyInfo(data))
    {
        if((LX_Timer::getTicks() - start_point) > data.t)
        {
            level->popData();

            if(data._alarm)
            {
                bg->setIncrease();
                audiohdl->playAlarm();
                audiohdl->playVoiceBoss();
            }
            else
            {
                enemies.push_back(data.e);
                data.e->start();
            }

            if(data.boss)
                audiohdl->playBossMusic();

            return true;
        }
    }
    return false;
}


void Engine::stopBossMusic()
{
    audiohdl->stopBossMusic();
}

Score *Engine::getScore() const
{
    return score;
}
