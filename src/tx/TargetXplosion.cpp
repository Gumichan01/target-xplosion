
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

#include "TargetXplosion.hpp"

#include "../asset/TX_Asset.hpp"
#include "../level/Level.hpp"
#include "../game/Result.hpp"
#include "../game/engine/Engine.hpp"
#include "../option/GamepadControl.hpp"
#include "../game/engine/AudioHandler.hpp"
#include "../resources/ResourceManager.hpp"
#include "../resources/WinID.hpp"
#include "../ui/Menu.hpp"

#include <Lunatix/Library.hpp>
#include <Lunatix/Window.hpp>
#include <Lunatix/WindowManager.hpp>
#include <Lunatix/MessageBox.hpp>
#include <Lunatix/Device.hpp>
#include <Lunatix/Gamepad.hpp>
#include <Lunatix/Random.hpp>
#include <Lunatix/Version.hpp>
#include <Lunatix/Log.hpp>

#include <SDL2/SDL.h>

#include <iostream>

namespace
{

const std::string TITLE( "Target Xplosion v1.0-beta" );
const std::string TITLE_DEBUG( "Target Xplosion - Level Debug" );
constexpr unsigned int ERRID = static_cast<unsigned int>( -1 );
const int WIDTH  = 1280;
const int HEIGHT = 720;


void registerWindow_( lx::Win::Window& window )
{
    using lx::Win::WindowManager;
    bool ok = WindowManager::getInstance().addWindow( window );

    if ( !ok )
    {
        lx::Log::logCritical( lx::Log::LogType::APPLICATION, "Internal error: %s",
                              lx::getError() );
        TX_Asset::destroy();
        lx::quit();
        throw std::string( "A critical error occured. Please contact the developper!" );
    }

    WinID::setWinID( window.getID() );
}

unsigned int selectLevel_() noexcept
{
    using std::cin;
    using std::cerr;
    unsigned int id_lvl;

    cerr.flush();
    cerr << "\n";
    cerr << " ====================================" << "\n";
    cerr << "     Target Xplosion - Debug mode    " << "\n";
    cerr << " ====================================" << "\n\n";
    // Select the level
    cerr << "Select the level ID: ";
    cin >> id_lvl;

    if ( id_lvl > Level::MAX_LEVEL )
    {
        cerr << "Invalid level ID: " << id_lvl << "\n";
        return ERRID;
    }

    return id_lvl;
}

// It is not working on Windows
// @todo implement it on Windows
void getSystemLanguage( std::string& language )
{
    const std::string ENV_LANG = "LANG";
    const char * const env_p = std::getenv( ENV_LANG.c_str() );

    if ( env_p != nullptr )
    {
        language = std::string( env_p );
    }
}

bool isDefaultLanguage( const std::string& language )
{
    const std::string DEFAULT_LANGUAGE = "en";
    return DEFAULT_LANGUAGE == language;
}

}   // namespace

bool TargetXplosion::debug_mode = false;


void TargetXplosion::loadI18nFile( const std::string& language ) noexcept
{
    // translation file format: tx_<lang>.mo
    const std::string CONFIG_DIR = "config/";
    const std::string PREFIX = "tx_";
    const std::string EXT = ".mo";
    const std::string LANG_FILE = CONFIG_DIR + PREFIX + language + EXT;
    lx::Log::log( "i18n file: %s", LANG_FILE.c_str() );

    try
    {
        translation.parser.parse_file( LANG_FILE.c_str() );
        translation.install();
    }
    catch (const std::exception& e)
    {
        lx::Log::logError( lx::Log::LogType::APPLICATION,
                           "Error while parsing %s: %s", LANG_FILE.c_str(), e.what() );
    }
}

void TargetXplosion::i18n() noexcept
{
    const int START_SUBSTRING = 0;
    const int LENGTH_SUBSTRING = 2;
    std::string env_value;

    getSystemLanguage( env_value );
    lx::Log::log( "LANG=%s", env_value.c_str() );
    const std::string LANGUAGE = env_value.substr( START_SUBSTRING, LENGTH_SUBSTRING );

    if ( !isDefaultLanguage( LANGUAGE ) )
    {
        loadI18nFile( LANGUAGE );
    }
}

bool TargetXplosion::isDebugged() noexcept
{
    return debug_mode;
}

TargetXplosion::TargetXplosion( bool gui, bool todebug )
    : gui_mode( gui ), translation()
{
    debug_mode = todebug;
    lx::Log::setDebugMode( debug_mode );

    if ( !lx::init() )
    {
        using std::string;
        const string crit_msg{string( "Cannot initialize the game engine: " ) + lx::getError()};
        lx::setError( crit_msg );
        lx::Log::logCritical( lx::Log::LogType::APPLICATION, "%s", crit_msg.c_str() );
        lx::MSGBox::showMSG( lx::MSGBox::MsgType::ERR, "Critical Error", lx::getError() );
        throw crit_msg;
    }

    sdlConfig();
    TX_Asset::init();
    lx::Random::initRand();
    xmlConfig();
    lx::VersionInfo::info();
    i18n();
}

void TargetXplosion::sdlConfig() noexcept
{
    if ( !lx::setSDLConfig( SDL_HINT_RENDER_SCALE_QUALITY, "best" ) )
    {
        lx::Log::logWarning( lx::Log::LogType::APPLICATION,
                             "cannot get the anisotropic filtering, trying the linear filtering" );

        if ( !lx::setSDLConfig( SDL_HINT_RENDER_SCALE_QUALITY, "linear" ) )
        {
            lx::Log::logWarning( lx::Log::LogType::APPLICATION,
                                 "cannot get the linear filtering" );
            lx::setSDLConfig( SDL_HINT_RENDER_SCALE_QUALITY, "nearest" );
        }
    }
}

void TargetXplosion::xmlConfig()
{
    using std::string;
    if ( TX_Asset::getInstance()->readXMLFile() != 0 )
    {
        const string err_msg = "Cannot load the configuration data: \"" +
                               TX_Asset::getInstance()->getfileName() + "\" ";

        lx::Log::logCritical( lx::Log::LogType::APPLICATION, "%s", err_msg.c_str() );
        lx::MSGBox::showMSG( lx::MSGBox::MsgType::ERR,
                             "XML file configuration error", err_msg.c_str() );
        TX_Asset::destroy();
        lx::quit();
        throw err_msg;
    }
}

void TargetXplosion::debug()
{
    using lx::Win::Window;
    using lx::Win::WindowManager;
    unsigned int id_level = selectLevel_();

    Window& w = WindowManager::getInstance().getWindow( WinID::getWinID() );
    w.setDrawBlendMode( lx::Win::BlendMode::BLEND );
    w.show();

    if ( id_level != ERRID )
    {
        ResultInfo info;
        lx::Device::Gamepad gamepad;

        if ( lx::Device::numberOfDevices() > 0 )
            gamepad.open( 0 );

        GPconfig::GamepadHandler gamepadhdl( gamepad );
        if ( Engine::getInstance()->play( info, gamepadhdl, id_level ) == GAME_FINISH )
            Result::displayResult( info );

        gamepad.close();
    }
}

void TargetXplosion::release()
{
    using lx::Win::Window;
    using lx::Win::WindowManager;
    Window& w = WindowManager::getInstance().getWindow( WinID::getWinID() );
    w.show();
    MainMenu( w ).event();
}

void TargetXplosion::run()
{
    lx::Win::WindowInfo winfo;
    lx::Win::initWindowInfo( winfo );
    winfo.title = debug_mode ? TITLE_DEBUG : TITLE;
    winfo.w = WIDTH;
    winfo.h = HEIGHT;
    lx::Win::Window window( winfo );
    window.hide();

    registerWindow_( window );
    ResourceManager::init();
    AudioHandler::AudioHDL::init();

    if ( debug_mode && !gui_mode )
    {
        debug();
    }
    else
        release();

    AudioHandler::AudioHDL::destroy();
    ResourceManager::destroy();
    lx::Win::WindowManager::getInstance().removeWindow( window.getID() );
}

TargetXplosion::~TargetXplosion()
{
    TX_Asset::destroy();
    lx::quit();
}
