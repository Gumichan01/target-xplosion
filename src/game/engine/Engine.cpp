
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
#include "EntityHandler.hpp"

// Game
#include "../Result.hpp"
#include "../Background.hpp"
#include "../Scoring.hpp"

// Entities
#include "../../entities/Rocket.hpp"
#include "../../entities/Player.hpp"

// Data
#include "../../level/Level.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../resources/WinID.hpp"

// Including some header files of the engine
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Mixer.hpp>
#include <LunatiX/LX_Device.hpp>
#include <LunatiX/LX_Log.hpp>


using namespace Result;
using namespace AudioHandler;
using namespace LX_Device;
using namespace LX_Win;

namespace
{
const int GAME_X_OFFSET = -128;
const int GAME_Y_OFFSET = 8;
const int GAME_YMIN = 68;

// Player
const unsigned int MIN_HEALTH_POINTS = 100;
const unsigned int MIN_ATTACK = 20;
const unsigned int MIN_DEFENSE = 12;
const unsigned int MIN_CRITIC = 3;

}

// Internal variables
FrameLimits Engine::flimits;
static Engine *game_instance = nullptr;
// The height of the background
// if the Y limit of the Engine (on screen)
const int BG_WIDTH = 1600;


Engine::Engine()
    : game_state(EngineStatusV::GAME_RUNNING), end_of_level(false),
      game_item(nullptr), bgm(nullptr), score(nullptr),
      hudhdl(HudHandler::getInstance()), entityhdl(EntityHandler::getInstance()),
      playerhdl(PlayerHandler::getInstance()), audiohdl(nullptr),
      level(nullptr), bg(nullptr), gw(nullptr)
{
    score = new Score();
    hudhdl.addHUD(*score);
    gw = LX_WindowManager::getInstance()->getWindow(WinID::getWinID());

    flimits.min_x = 0;
    flimits.min_y = GAME_YMIN;
    flimits.max_x = gw->getLogicalWidth();
    flimits.max_y = gw->getLogicalHeight();
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

bool Engine::outOfBound(const LX_AABB& pos)
{
    return (pos.x < (-pos.w + GAME_X_OFFSET) || pos.x > flimits.max_x
            || pos.y < (-pos.h - GAME_Y_OFFSET)
            || pos.y > flimits.max_y + GAME_Y_OFFSET);
}


int Engine::getMinXlim()
{
    return flimits.min_x;
}

int Engine::getMaxXlim()
{
    return flimits.max_x;
}

int Engine::getMinYlim()
{
    return flimits.min_y;
}

int Engine::getMaxYlim()
{
    return flimits.max_y;
}


void Engine::createPlayer(unsigned int hp, unsigned int att, unsigned int sh,
                          unsigned int critic)
{
    PlayerParam param;

    param.hp = hp;
    param.att = att;
    param.sh = sh;
    param.critic = critic;
    param.x = flimits.max_x / 2 - Player::PLAYER_WIDTH / 2;
    param.y = flimits.max_y / 2 - Player::PLAYER_HEIGHT / 2;
    param.w = Player::PLAYER_WIDTH;
    param.h = Player::PLAYER_HEIGHT;

    playerhdl.setPlayer(param);
}


bool Engine::loadLevel(const unsigned int lvl)
{
    unsigned int hp, att, def, critic;

    // Load ressources first !!!
    ResourceManager::getInstance()->loadResources();
    end_of_level = false;

    // The player's skills
    hp = MIN_HEALTH_POINTS;
    att = MIN_ATTACK;
    def = MIN_DEFENSE;
    critic = MIN_CRITIC;

    // Game
    level = new Level(lvl);

    // Level loaded
    bgm = new BGM(lvl);
    hudhdl.setBGM(*bgm);
    setBackground(lvl);
    audiohdl = AudioHDL::init(lvl);

    {
        GameEnv env{level, bg};
        entityhdl.setGameEnv(env);
    }

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

    ResourceManager::getInstance()->freeResources();
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
        if((done = input()))
            continue;

        createItem();
        physics();
        status();
        clean();
        display();
        while(entityhdl.generateEnemy());

        // Framerate regulation
        Framerate::regulate();

        if(LX_Log::isDebugMode())
        {
            Framerate::cycle();
        }
    }

    // A this point, the game is over
    LX_Device::mouseCursorDisplay(LX_MOUSE_SHOW);
    audiohdl->stopMainMusic();
    entityhdl.clearAll();

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
    if(loadLevel(lvl))
    {
        score->resetScore();
        game_state = loop(info);
        endLevel();
    }
    else
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "Cannot load the level #%u", lvl);
    return game_state;
}


void Engine::generateResult(ResultInfo& info) const
{
    info.level = level->getLevelNum();
    info.nb_death = playerhdl.getPlayer().nb_death();
    info.score = score->getCurrentScore();
    info.nb_killed_enemies = score->getKilledEnemies();
    info.max_combo = score->getMaxCombo();
    info.max_nb_enemies = 0;
}

bool Engine::input()
{
    bool is_done = false;
    PlayerInput::input(playerhdl.getPlayer(), is_done);
    return is_done;
}


void Engine::physics()
{
    const Player& cplayer = playerhdl.getPlayerConst();
    Player& player = playerhdl.getPlayer();

    if(!cplayer.isDead() && !cplayer.isDying())
    {
        if(game_item != nullptr)
            player.collision(game_item);
    }

    entityhdl.physics(player);
}

void Engine::status()
{
    if(game_item->getX() <= (-(game_item->getWidth()) - 1))
    {
        game_item->die();
    }
    else if(!game_item->isDead())
        game_item->move();

    entityhdl.updateStatus(playerhdl.getPlayer());
}

void Engine::clean()
{
    destroyItem();
    entityhdl.cleanEntities();
}

// In loop
void Engine::display()
{
    gw->clearWindow();
    bg->update();

    entityhdl.displayEntities();

    // Display the item
    if(game_item != nullptr)
        game_item->draw();

    playerhdl.getPlayer().draw();

    if(entityhdl.nbEnemies() == 0 && level->numberOfEnemies() == 0)
        hudhdl.fadeOut(end_of_level);
    else
        hudhdl.displayHUD();

    gw->update();
    gw->setViewPort(nullptr);
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
    if(game_item->isDead() || game_item->getPowerUp() == ItemType::NOPOW)
    {
        delete game_item;
        game_item = nullptr;
    }
}


void Engine::setBackground(unsigned int lvl)
{
    const int SPEED_BG = -4;
    LX_AABB box = {0, 0, BG_WIDTH, flimits.max_y};
    bg = new Background(lvl, box, SPEED_BG);
}

void Engine::targetPlayer(EnemyRocket * m)
{
    entityhdl.targetPlayer(playerhdl.getPlayer(), *m);
}

Score * Engine::getScore() const
{
    return score;
}
