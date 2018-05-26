
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
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#include "../game/engine/AudioHandler.hpp"
#include "../resources/ResourceManager.hpp"
#include "../resources/WinID.hpp"
#include "../ui/Menu.hpp"

#include <LunatiX/LX_Library.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_MessageBox.hpp>
#include <LunatiX/LX_Device.hpp>
#include <LunatiX/LX_Gamepad.hpp>
#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Version.hpp>
#include <LunatiX/LX_Log.hpp>

#include <iostream>

namespace
{


const std::string TITLE("Target Xplosion v0.5.3-alpha");
const std::string TITLE_DEBUG("Target Xplosion - Level Debug");
constexpr unsigned int ERRID = static_cast<unsigned int>(-1);
const int WIDTH  = 1280;
const int HEIGHT = 768;


void registerWindow_(LX_Win::LX_Window& window)
{
    using LX_Win::LX_WindowManager;
    bool ok = LX_WindowManager::getInstance().addWindow(window);

    if(!ok)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,"Internal error: %s",
                            LX_getError());
        TX_Asset::destroy();
        LX_Quit();
        throw std::string("A critical error occured. Please contact the developper!");
    }

    WinID::setWinID(window.getID());
}

unsigned int selectLevel_() noexcept
{
    using std::cin;
    using std::cerr;
    unsigned int id_lvl;

    cerr.flush();
    cerr << '\n';
    cerr << " ====================================" << "\n";
    cerr << "     Target Xplosion - Debug mode    " << "\n";
    cerr << " ====================================" << "\n\n";
    // Select the level
    cerr << "Select the level ID: ";
    cin >> id_lvl;

    if(id_lvl > Level::MAX_LEVEL)
    {
        cerr << "Invalid level ID: " << id_lvl << "\n";
        return ERRID;
    }

    return id_lvl;
}

}   // namespace

bool TargetXplosion::debug_mode = false;

bool TargetXplosion::isDebugged() noexcept
{
    return debug_mode;
}

TargetXplosion::TargetXplosion(bool gui, bool todebug) : gui_mode(gui)
{
    debug_mode = todebug;
    LX_Log::setDebugMode(debug_mode);

    if(!LX_Init())
    {
        using std::string;
        const string crit_msg{string("Cannot initialize the game engine: ") + LX_getError()};
        LX_setError(crit_msg);
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION, "%s", crit_msg.c_str());
        LX_MSGBox::showMSG(LX_MSGBox::LX_MsgType::ERR, "Critical Error", LX_getError());
        throw crit_msg;
    }

    sdlConfig();
    TX_Asset::init();
    LX_Random::initRand();
    xmlConfig();
    LX_VersionInfo::info();
}

void TargetXplosion::sdlConfig() noexcept
{
    if(!setSDLConfig(SDL_HINT_RENDER_SCALE_QUALITY,"best"))
    {
        LX_Log::logWarning(LX_Log::LX_LogType::APPLICATION,
                           "cannot get the anisotropic filtering, trying the linear filtering");

        if(!setSDLConfig(SDL_HINT_RENDER_SCALE_QUALITY,"linear"))
        {
            LX_Log::logWarning(LX_Log::LX_LogType::APPLICATION,
                               "cannot get the linear filtering");
            setSDLConfig(SDL_HINT_RENDER_SCALE_QUALITY,"nearest");
        }
    }
}

void TargetXplosion::xmlConfig()
{
    using std::string;
    if(TX_Asset::getInstance()->readXMLFile() != 0)
    {
        const string err_msg = "Cannot load the configuration data: \"" +
                               TX_Asset::getInstance()->getfileName() + "\" ";

        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION, "%s", err_msg.c_str());
        LX_MSGBox::showMSG(LX_MSGBox::LX_MsgType::ERR,
                           "XML file configuration error", err_msg.c_str());
        TX_Asset::destroy();
        LX_Quit();
        throw err_msg;
    }
}

void TargetXplosion::debug()
{
    using LX_Win::LX_Window;
    using LX_Win::LX_WindowManager;
    unsigned int id_level = selectLevel_();

    LX_Window& w = LX_WindowManager::getInstance().getWindow(WinID::getWinID());
    w.setDrawBlendMode(LX_Win::LX_BlendMode::LX_BLENDMODE_BLEND);
    w.show();

    if(id_level != ERRID)
    {
        LX_Device::LX_Gamepad gamepad;
        ResultInfo info;

        if(LX_Device::numberOfDevices() > 0)
            gamepad.open(0);

        // Play the level defined by the player
        if(Engine::getInstance()->play(info, id_level) == GAME_FINISH)
            Result::displayResult(info);

        gamepad.close();
    }
}

void TargetXplosion::release()
{
    using LX_Win::LX_Window;
    using LX_Win::LX_WindowManager;
    LX_Window& w = LX_WindowManager::getInstance().getWindow(WinID::getWinID());
    w.show();
    MainMenu(w).event();
}

void TargetXplosion::run()
{
    LX_Win::LX_WindowInfo winfo;
    LX_Win::LX_initWindowInfo(winfo);
    winfo.title = debug_mode ? TITLE : TITLE_DEBUG;
    winfo.w = WIDTH;
    winfo.h = HEIGHT;
    LX_Win::LX_Window window(winfo);
    window.hide();

    registerWindow_(window);
    ResourceManager::init();
    AudioHandler::AudioHDL::init();

    if(debug_mode && !gui_mode)
    {
        debug();
    }
    else
        release();

    AudioHandler::AudioHDL::destroy();
    ResourceManager::destroy();
    LX_Win::LX_WindowManager::getInstance().removeWindow(window.getID());
}

TargetXplosion::~TargetXplosion()
{
    TX_Asset::destroy();
    LX_Quit();
}
