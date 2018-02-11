
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

// Player
const unsigned int MIN_HEALTH_POINTS = 100;
const unsigned int MIN_ATTACK = 20;
const unsigned int MIN_DEFENSE = 12;
const unsigned int MIN_CRITIC = 3;

}

// Internal variables
FrameLimits Engine::flimits;
uint8_t Engine::fade_out_counter = 0;
static Engine *game_instance = nullptr;
// The height of the background
// if the Y limit of the Engine (on screen)
const int BG_WIDTH = 1600;


Engine::Engine()
    : game_state(EngineStatusV::GAME_RUNNING), start_point(0),
      end_of_level(false), player(nullptr), game_item(nullptr),
      bgm(nullptr), score(nullptr), hudhdl(HudHandler::getInstance()),
      entityhdl(EntityHandler::getInstance()), audiohdl(nullptr),
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

Engine::~Engine()
{
    delete player;
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
    LX_AABB ppos;
    ppos.x = flimits.max_x / 2 - Player::PLAYER_WIDTH / 2;
    ppos.y = flimits.max_y / 2 - Player::PLAYER_HEIGHT / 2;
    ppos.w = Player::PLAYER_WIDTH;
    ppos.h = Player::PLAYER_HEIGHT;
    const ResourceManager * resources = ResourceManager::getInstance();

    LX_Vector2D pvel(0.0f, 0.0f);
    LX_Graphics::LX_Sprite *psprite = resources->getPlayerResource();

    delete player;
    player = new Player(hp, att, sh, critic, psprite, ppos, pvel, flimits.max_x, flimits.max_y);
}


bool Engine::loadLevel(const unsigned int lvl)
{
    unsigned int hp, att, def, critic;

    // Whatever what you are doing, load ressources first !!!
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

    ResourceManager::getInstance()->freeResources();   /// @todo put it in ResourceManager
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


void Engine::physics()
{
    if(!player->isDead() && !player->isDying())
    {
        if(game_item != nullptr)
            player->collision(game_item);
    }

    entityhdl.physics(*player);
}

void Engine::status()
{
    if(game_item->getX() <= (-(game_item->getWidth()) - 1))
    {
        game_item->die();
    }
    else if(!game_item->isDead())
        game_item->move();

    entityhdl.updateStatus(*player);
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

    player->draw();

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
    if(game_item->isDead() || game_item->getPowerUp() == POWER_UP::NOPOW)
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


void Engine::acceptEnemyMissile(Missile *m)
{
    entityhdl.pushEnemyMissile(*m);
}

void Engine::acceptEnemy(Enemy *e)
{
    entityhdl.pushEnemy(*e);
}

void Engine::acceptPlayerMissile(Missile *m)
{
    entityhdl.pushPlayerMissile(*m);
}

void Engine::targetEnemy(PlayerRocket * m)
{
    entityhdl.targetEnemy(*m);
}

void Engine::targetPlayer(EnemyRocket * m)
{
    entityhdl.targetPlayer(*player, *m);
}

void Engine::acceptItem(Item * y)
{
    entityhdl.pushItem(*y);
}


void Engine::bulletCancel()
{
    entityhdl.bulletCancel();
}

Score *Engine::getScore() const
{
    return score;
}
