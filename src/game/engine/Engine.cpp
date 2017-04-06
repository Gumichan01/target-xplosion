
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

#include <sstream>

using namespace LX_Win;
using namespace LX_Physics;
using namespace LX_Device;
using namespace Result;

namespace
{
const int GAME_X_OFFSET = -128;
const int GAME_Y_OFFSET = 256;
const int GAME_YMIN = 68;
// Viewport
const int GAME_VPORT_H = 68;
// Fading
const int FADE_MAX_VALUE = 255;

// Load the important ressources
void loadRessources()
{
    Bomb::loadExplosionBuffer();
    Item::createItemRessources();
}

// Free all ressources
void freeRessources()
{
    Item::destroyItemRessources();
}
};

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
      level(nullptr), score(nullptr), bg(nullptr), gamepad(),
      audiohdl(nullptr), resources(nullptr), gw(nullptr)
{
    score = new Score();
    resources = ResourceManager::getInstance();
    gw = LX_WindowManager::getInstance()->getWindow(WinID::getWinID());

    game_minXlimit = 0;
    game_maxXlimit = gw->getLogicalWidth();
    game_minYlimit = GAME_YMIN;
    game_maxYlimit = gw->getLogicalHeight();

    if(numberOfDevices() > 0)
        gamepad.open(0);
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
    gamepad.close();
    delete player;
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
                          unsigned int critic, LX_Graphics::LX_Sprite *image,
                          int x, int y, int w, int h, float vx, float vy)
{
    LX_AABB new_pos = {x, y, w, h};
    LX_Vector2D new_speed(vx, vy);

    delete player;
    player = new Player(hp, att, sh, critic, image, nullptr,
                        new_pos, new_speed, game_maxXlimit, game_maxYlimit);
}


bool Engine::loadLevel(const unsigned int lvl)
{
    unsigned int hp, att, def, critic;
    TX_Asset *a = TX_Asset::getInstance();

    level = new Level(lvl);
    end_of_level = false;

    // The player's skills
    hp = 100;
    att = 20;
    def = 12;
    critic = 3;
    std::string tmp = a->getLevelMusic(lvl);

    if(level->isLoaded())
    {
        loadRessources();
        setBackground(lvl);
        audiohdl = AudioHandler::AudioHandler::init(lvl);
        //audiohdl = new AudioHandler::AudioHandler(lvl);
        LX_Graphics::LX_Sprite *player_sprite = resources->getPlayerResource();

        if(lvl != 0)
        {
            hp *= lvl;
            att *= lvl;
            def *= lvl;
            critic *= lvl;
        }

        createPlayer(hp, att, def, critic, player_sprite,
                     (game_maxXlimit/2)-(PLAYER_WIDTH/2),
                     (game_maxYlimit/2)-(PLAYER_HEIGHT/2),
                     PLAYER_WIDTH, PLAYER_HEIGHT, 0, 0);

        player_missiles.reserve(DEFAULT_RESERVE);
        enemies_missiles.reserve(ENEMY_MISSILES_RESERVE);
        enemies.reserve(ENEMY_RESERVE);
        return true;
    }
    return false;
}


void Engine::endLevel()
{
    delete bg;
    delete level;
    delete game_item;
    AudioHandler::AudioHandler::destroy();

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

    // For debug mode
    LX_Mixer::setOverallVolume(OV_VOLUME);
    LX_Mixer::setMusicVolume(MUSIC_VOLUME);
    LX_Mixer::setFXVolume(FX_VOLUME);
    // For debug mode ENDs
    audiohdl->playMainMusic();

    LX_Device::mouseCursorDisplay(LX_MOUSE_HIDE);
    gw->setDrawBlendMode(LX_Win::LX_BLENDMODE_BLEND);
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION, "Allocated channels: %d",
                     LX_Mixer::allocateChannels(-1));
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION, "Number of enemies: %u", nb_enemies);

    while(!done && !end_of_level)
    {
        if((done = input()) == true)
            continue;

        createItem();

        // Logic
        physics();
        status();
        clean();
        display();
        while(generateEnemy());

        cycle();
        Framerate::regulate();
        Framerate::cycle();
        Framerate::frame();
    }

    // A this point, the game is over
    generateResult(info);
    info.max_nb_enemies = static_cast<unsigned int>(nb_enemies);

    LX_Device::mouseCursorDisplay(LX_MOUSE_SHOW);
    audiohdl->stopMainMusic();
    clearVectors();

    // Status of the game
    if(end_of_level)
        game_status = GAME_FINISH;
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


void Engine::cycle()
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


void Engine::generateResult(ResultInfo& info)  const
{
    // Create the result and copy it
    info.level = level->getLevelNum();
    info.nb_death = player->nb_death();
    info.score = score->getCurrentScore();
    info.total_score += score->getTotalScore();
    info.nb_killed_enemies = score->getKilledEnemies();
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
    enemies_missiles.push_back(m);
}

void Engine::acceptEnemy(Enemy *e)
{
    enemies.push_back(e);
}

void Engine::acceptPlayerMissile(Missile *m)
{
    player_missiles.push_back(m);
}

void Engine::targetEnemy(Missile * m)
{
    if(!enemies.empty() && enemies[0] != nullptr)
        m->visit(enemies[0]);
}

void Engine::targetPlayer(EnemyRocket * m)
{
    m->visitp(player);
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
    const int SPEED_BG = -3;
    const TX_Asset * asset = TX_Asset::getInstance();
    LX_AABB box = {0, 0, BG_WIDTH, game_maxYlimit};

    /// @todo (#1#) Background for the second level
    bg = new Background(asset->getLevelBg(lvl), box, SPEED_BG);
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
    if(game_item->isDead() || game_item->getPowerUp() == POWER_UP::NO_POWER_UP)
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
        if(player_missiles[i] != nullptr)
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
        if(enemies_missiles[k] != nullptr)
            delete enemies_missiles[k];

        enemies_missiles.erase(enemies_missiles.begin() + k);
        k--;
    }
}

void Engine::clearEnemies()
{
    // Enemies
    for(std::vector<Enemy *>::size_type j = 0; j != enemies.size(); j++)
    {
        if(enemies[j] != nullptr)
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
        if(items[l] != nullptr)
            delete items[l];

        items.erase(items.begin() + l);
        l--;
    }
}

void Engine::screenCancel()
{
    missileToScore();
    clearEnemyMissiles();
}

void Engine::missileToScore()
{
    for(auto m_it = enemies_missiles.begin();
            m_it != enemies_missiles.end(); m_it++)
    {
        items.push_back(new Item((*m_it)->getX(),
                                 (*m_it)->getY()));
    }
}


void Engine::physics()
{
    if(player->isDead() == false)
    {
        if(game_item != nullptr)
            player->collision(game_item);

        for(auto it = items.begin(); it != items.end(); it++)
        {
            player->collision(*it);
        }
    }

    for(auto en_it = enemies.begin(); en_it != enemies.end(); en_it++)
    {
        // enemy/player collision
        if(!player->isDead())
            (*en_it)->collision(player);

        if((*en_it)->isDead())
            continue;

        // enemy/missile collision
        for(auto pm_it = player_missiles.begin();
                pm_it != player_missiles.end(); pm_it++)
        {
            if((*pm_it) == nullptr)
                continue;

            (*en_it)->collision(*pm_it);
        }
    }

    if(!player->isDead())
    {
        for(auto m_it = enemies_missiles.begin();
                m_it != enemies_missiles.end(); m_it++)
        {
            // enemy missiles/player collision
            player->collision(*m_it);
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
    for(auto it = items.begin(); it != items.end(); it++)
    {
        if((*it)->getX() > (-((*it)->getWidth()) - 1))
            (*it)->move();
        else
            (*it)->die();
    }

    // Move the player
    if(!player->isDead())
    {
        if(player->isLaserActivated())
        {
            player->fire(MISSILE_TYPE::LASER_TYPE);
            screenCancel();
        }

        player->move();
        death_start = LX_Timer::getTicks();   // Moment of possible death
    }
    else
    {
        if((LX_Timer::getTicks() - death_start) > DELAY_TO_REBORN)
            player->reborn();
    }

    // Move the missiles of the player
    for(auto pm_it = player_missiles.begin();
            pm_it != player_missiles.end(); pm_it++)
    {
        if((*pm_it) == nullptr)
            continue;

        if((*pm_it)->getX() >= game_maxXlimit)
            (*pm_it)->die();
        else
            (*pm_it)->move();
    }

    // Move the missiles of enemies
    for(unsigned int i = 0; i < enemies_missiles.size(); i++)
    {
        if(enemies_missiles[i] == nullptr)
            continue;

        // If an enemy missile is not visible -> it dies.
        int x = enemies_missiles[i]->getX();
        int y = enemies_missiles[i]->getY();
        int w = enemies_missiles[i]->getWidth();
        int h = enemies_missiles[i]->getHeight();
        int xoff = GAME_X_OFFSET;
        int yoff = GAME_Y_OFFSET;

        if(x <= (-w + xoff) || x >= game_maxXlimit || y <= (-h + xoff) || y >= game_maxYlimit + yoff)
            enemies_missiles[i]->die();
        else
            enemies_missiles[i]->move();
    }

    // The enemy strategy
    for(auto en_it = enemies.begin(); en_it != enemies.end(); en_it++)
    {
        if((*en_it)->getX() <= (-((*en_it)->getWidth()) -1))
            (*en_it)->die();
        else
            (*en_it)->strategy();
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
            if(enemies[j]->killed())
            {
                int sc = static_cast<int>(enemies[j]->getMaxHP() +
                                          enemies[j]->getATT() +
                                          enemies[j]->getDEF());
                score->notify(sc, true);
            }

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

    gw->setViewPort(&viewport);
    gw->setDrawColour(bcolour);
    gw->fillRect(cvport);
    score->display();

    for(auto it = huds.begin(); it != huds.end(); it++)
    {
        if((*it) != nullptr)
            (*it)->displayHUD();
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
    displayPlayerMissiles();
    displayEnemies();
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
    for(auto pm_it = player_missiles.cbegin();
            pm_it != player_missiles.cend(); pm_it++)
    {
        if((*pm_it) != nullptr) (*pm_it)->draw();
    }
}

void Engine::displayItems() const
{
    for(auto it = items.cbegin(); it != items.cend(); it++)
    {
        if((*it) != nullptr) (*it)->draw();
    }
}

void Engine::displayEnemies() const
{
    for(auto en_it = enemies.cbegin(); en_it != enemies.cend(); en_it++)
    {
        if((*en_it) != nullptr && (*en_it)->getX() < game_maxXlimit)
            (*en_it)->draw();
    }
}

void Engine::displayEnemyMissiles() const
{
    for(auto m_it = enemies_missiles.cbegin();
            m_it != enemies_missiles.cend(); m_it++)
    {
        if((*m_it) != nullptr) (*m_it)->draw();
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
                audiohdl->playAlarm();
            else
                enemies.push_back(data.e);

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
