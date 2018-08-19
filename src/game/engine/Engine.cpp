
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
#include "../../entities/Rocket.hpp"
#include "../../entities/Player.hpp"

// Data
#include "../../level/Level.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../resources/WinID.hpp"

// Including some header files of the engine
#include <Lunatix/Window.hpp>
#include <Lunatix/WindowManager.hpp>
#include <Lunatix/Mixer.hpp>
#include <Lunatix/Device.hpp>
#include <Lunatix/Log.hpp>

using namespace Result;
using namespace GPconfig;
using namespace AudioHandler;


namespace
{
const float GAME_X_OFFSET = -128.0f;
const float GAME_Y_OFFSET = 8.0f;
const Float GAME_YMIN = {68.0f};

// Player
const unsigned int MIN_HEALTH_POINTS = 100;
const unsigned int MIN_ATTACK = 20;
const unsigned int MIN_DEFENSE = 12;
const unsigned int MIN_CRITIC = 3;

}

// Internal variables
FrameLimits Engine::flimits;
static Engine * game_instance = nullptr;
// The height of the background
// if the Y limit of the Engine (on screen)
const int BG_WIDTH = 1600;


Engine::Engine()
    : game_state( EngineStatus::GAME_RUNNING ), end_of_level( false ),
      game_item( nullptr ), bgm( nullptr ), score( nullptr ),
      hudhdl( HudHandler::getInstance() ), entityhdl( EntityHandler::getInstance() ),
      playerhdl(), audiohdl( nullptr ), level( nullptr ), bg( nullptr ),
      gw( lx::Win::WindowManager::getInstance().getWindow( WinID::getWinID() ) )
{
    score = new Score();
    hudhdl.addHUD( *score );

    flimits.min_y = GAME_YMIN;
    flimits.max_x = {static_cast<float>( gw.getLogicalWidth() )};
    flimits.max_y = {static_cast<float>( gw.getLogicalHeight() )};
}


Engine * Engine::getInstance()
{
    if ( game_instance == nullptr )
        game_instance = new Engine();

    return game_instance;
}

void Engine::destroy()
{
    delete game_instance;
    game_instance = nullptr;
}


bool Engine::outOfBound( const lx::Physics::FloatingBox& fpos ) noexcept
{
    return ( fpos.p.x < ( -fpos.w + Float{GAME_X_OFFSET} ) || fpos.p.x > flimits.max_x
             || fpos.p.y < ( -fpos.h - Float{GAME_Y_OFFSET} )
             || fpos.p.y > flimits.max_y + Float{GAME_Y_OFFSET} );
}


Float Engine::getMinXlim() noexcept
{
    return flimits.min_x;
}

Float Engine::getMaxXlim() noexcept
{
    return flimits.max_x;
}

Float Engine::getMinYlim() noexcept
{
    return flimits.min_y;
}

Float Engine::getMaxYlim() noexcept
{
    return flimits.max_y;
}


void Engine::createPlayer( unsigned int hp, unsigned int att, unsigned int sh,
                           unsigned int critic, GamepadHandler& gamepadhdl )
{
    PlayerParam param( gamepadhdl );

    param.hp = hp;
    param.att = att;
    param.sh = sh;
    param.critic = critic;
    param.x = flimits.max_x / 2 - Player::PLAYER_WIDTH / 2;
    param.y = flimits.max_y / 2 - Player::PLAYER_HEIGHT / 2;
    param.w = Player::PLAYER_WIDTH;
    param.h = Player::PLAYER_HEIGHT;

    playerhdl.setPlayer( param );
}


bool Engine::loadLevel( const unsigned int lvl, GamepadHandler& gamepadhdl )
{
    unsigned int hp, att, def, critic;

    // Load ressources first, otherwise the game will crash !!!
    ResourceManager::getInstance()->loadResources();
    end_of_level = false;

    hp = MIN_HEALTH_POINTS;
    att = MIN_ATTACK;
    def = MIN_DEFENSE;
    critic = MIN_CRITIC;

    level = new Level( lvl );
    bgm = new BGM( lvl );
    hudhdl.setBGM( *bgm );
    setBackground( lvl );
    audiohdl = AudioHDL::init();
    audiohdl->setLevel( lvl );

    {
        GameEnv env{ level, bg };
        entityhdl.setGameEnv( env );
    }

    if ( lvl != 0 )
    {
        hp *= lvl;
        att *= lvl;
        def *= lvl;
        critic *= lvl;
    }

    createPlayer( hp, att, def, critic, gamepadhdl );
    return true;
}


void Engine::endLevel()
{
    delete bg;
    delete bgm;
    delete level;
    delete game_item;

    game_item = nullptr;
    bg = nullptr;
    level = nullptr;
    audiohdl = nullptr;

    AudioHandler::AudioHDL::haltAudio();
    ResourceManager::getInstance()->freeResources();
}


void Engine::beforeLoop() noexcept
{
    score->resetScore();
    lx::Device::mouseCursorDisplay( lx::Device::MouseToggle::HIDE );

    /// Debug mode
    if ( lx::Log::isDebugMode() )
    {
        lx::Mixer::setOverallVolume( OV_VOLUME );
        lx::Mixer::setMusicVolume( MUSIC_VOLUME );
        lx::Mixer::setFXVolume( FX_VOLUME );
    }

    audiohdl->playMainMusic();

    lx::Log::logDebug( lx::Log::LogType::APPLICATION, "Allocated channels: %d",
                       lx::Mixer::allocateChannels( -1 ) );
    lx::Log::logDebug( lx::Log::LogType::APPLICATION, "Number of enemies: %u",
                       level->numberOfEnemies() + ( level->hasBossParts() ? 1 : 0 ) );
}

EngineStatus Engine::loop( ResultInfo& info )
{
    bool done = false;

    while ( !done && !end_of_level )
    {
        if ( ( done = input() ) )
            continue;

        createItem();
        physics();
        status();
        clean();
        display();
        while ( entityhdl.generateEnemy() );

        Framerate::regulate();
        Framerate::cycle();
    }

    EngineStatus game_status;

    // A this point, the game is over
    if ( end_of_level )
    {
        game_status = GAME_FINISH;
        generateResult( info );
        info.max_nb_enemies = level->numberOfEnemies() + ( level->hasBossParts() ? 1 : 0 );
    }
    else
        game_status = GAME_QUIT;

    return game_status;
}

void Engine::afterLoop() noexcept
{
    audiohdl->stopMainMusic();
    entityhdl.clearAll();
    lx::Device::mouseCursorDisplay( lx::Device::MouseToggle::SHOW );
    endLevel();
}


EngineStatus Engine::play( ResultInfo& info, GamepadHandler& ghdl, unsigned int lvl )
{
    if ( loadLevel( lvl, ghdl ) )
    {
        beforeLoop();
        game_state = loop( info );
        afterLoop();
    }
    else
        lx::Log::logCritical( lx::Log::LogType::APPLICATION,
                              "Cannot load the level #%u", lvl );
    return game_state;
}


void Engine::generateResult( ResultInfo& info )
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
    PlayerInput::input( playerhdl.getPlayer(), is_done );
    return is_done;
}


void Engine::physics()
{
    Player& player = playerhdl.getPlayer();

    if ( !player.isDead() && !player.isDying() )
    {
        if ( game_item != nullptr )
            player.collision( game_item );
    }

    entityhdl.physics( player );
}

void Engine::status()
{
    if ( game_item->getX() <= ( -( game_item->getWidth() ) - 1 ) )
    {
        game_item->die();
    }
    else if ( !game_item->isDead() )
        game_item->move();

    entityhdl.updateStatus( playerhdl.getPlayer() );
}

void Engine::clean()
{
    destroyItem();
    entityhdl.cleanEntities();
}

// In loop
void Engine::display()
{
    gw.clearWindow();
    bg->update();

    entityhdl.displayEntities();

    // Display the item
    if ( game_item != nullptr )
        game_item->draw();

    playerhdl.getPlayer().draw();

    if ( entityhdl.nbEnemies() == 0 && level->numberOfEnemies() == 0 )
        hudhdl.fadeOut( end_of_level );
    else
        hudhdl.displayHUD();

    gw.update();
}


// Create a new item only if it does not exist
void Engine::createItem()
{
    if ( game_item == nullptr )
        game_item = new Item();
}

// Destroy the item
void Engine::destroyItem()
{
    if ( game_item->isDead() || game_item->getPowerUp() == ItemType::NOPOW )
    {
        delete game_item;
        game_item = nullptr;
    }
}


void Engine::setBackground( unsigned int lvl )
{
    const int SPEED_BG = -4;
    lx::Graphics::ImgRect box = {0, 0, BG_WIDTH, static_cast<int>( flimits.max_y )};
    bg = new Background( lvl, box, SPEED_BG );
}

void Engine::targetPlayer( EnemyRocket * m )
{
    entityhdl.targetPlayer( playerhdl.getPlayer(), *m );
}

Score * Engine::getScore() const
{
    return score;
}

Engine::~Engine()
{
    hudhdl.removeHUD( *score );
    delete score;
}
