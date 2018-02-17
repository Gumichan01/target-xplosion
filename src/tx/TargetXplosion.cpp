
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
#include "../resources/ResourceManager.hpp"
#include "../resources/WinID.hpp"
#include "../ui/Menu.hpp"

#include <LunatiX/LX_Library.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_MessageBox.hpp>
#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Version.hpp>
#include <LunatiX/LX_Log.hpp>


namespace
{
const std::string TITLE("Target Xplosion v0.5.2-alpha");
const std::string TITLE_DEBUG("Target Xplosion - Level Debug");
const int WIDTH  = 1280;
const int HEIGHT = 768;
}

TargetXplosion::TargetXplosion()
{
    using std::string;
    if(!LX_Init())
    {
        const string crit_msg{string("Cannot initialize the game engine: ") + LX_GetError()};
        LX_SetError(crit_msg);
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION, "%s", crit_msg.c_str());
        LX_MSGBox::showMSG(LX_MSGBox::LX_MSG_ERR, "Critical Error", LX_GetError());
        throw crit_msg;
    }

    sdlConfig();
    TX_Asset::init();
    LX_Random::initRand();

    if(TX_Asset::getInstance()->readXMLFile() != 0)
    {
        const string err_msg = "Cannot load the configuration data: \"" +
                               TX_Asset::getInstance()->getfileName() + "\" ";

        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", err_msg.c_str());
        LX_MSGBox::showMSG(LX_MSGBox::LX_MSG_ERR,"XML file configuration error",
                           err_msg.c_str());
        TX_Asset::destroy();
        LX_Quit();
        throw err_msg;
    }

    LX_VersionInfo::info();
}

void TargetXplosion::sdlConfig() noexcept
{
    if(!setSDLConfig(SDL_HINT_RENDER_SCALE_QUALITY,"best"))
    {
        LX_Log::logWarning(LX_Log::LX_LOG_APPLICATION,
                           "cannot get the anisotropic filtering, trying the linear filtering");

        if(!setSDLConfig(SDL_HINT_RENDER_SCALE_QUALITY,"linear"))
        {
            LX_Log::logWarning(LX_Log::LX_LOG_APPLICATION,
                               "cannot get the linear filtering");
            setSDLConfig(SDL_HINT_RENDER_SCALE_QUALITY,"nearest");
        }
    }
}

void TargetXplosion::main()
{
    LX_Win::LX_WindowInfo winfo;
    LX_Win::LX_initWindowInfo(winfo);
    winfo.title = TITLE;
    winfo.w = WIDTH;
    winfo.h = HEIGHT;
    LX_Win::LX_Window window(winfo);

    uint32_t id = LX_Win::LX_WindowManager::getInstance()->addWindow(&window);

    if(id == static_cast<uint32_t>(-1))
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"Internal error: %s",
                            LX_GetError());
        TX_Asset::destroy();
        LX_Quit();
        throw std::string("A critical eror occured. Please contact the developper!");
    }

    WinID::setWinID(id);
    ResourceManager::init();
    MainMenu(window).event();
    LX_Win::LX_WindowManager::getInstance()->removeWindow(id);
    ResourceManager::destroy();
}

TargetXplosion::~TargetXplosion()
{
    TX_Asset::destroy();
    LX_Quit();
}
