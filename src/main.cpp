
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

#include "asset/TX_Asset.hpp"
#include "resources/ResourceManager.hpp"
#include "resources/WinID.hpp"
#include "ui/Menu.hpp"

#include <LunatiX/LX_Library.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_MessageBox.hpp>
#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Version.hpp>

/// @todo create a class that handle the entry point of the program

using namespace std;
using namespace LX_Random;

const char * TITLE = "Target Xplosion v0.5.2-alpha";
const int WIDTH  = 1280;
const int HEIGHT = 768;

#if defined(__WIN32__)
int main(int argc, char** argv)
#else
int main()
#endif
{
    if(!LX_Init())
    {
        string crit_msg = string("Cannot initialize the game engine: ") + LX_GetError();
        LX_SetError(crit_msg);
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", crit_msg.c_str());
        LX_MSGBox::showMSG(LX_MSGBox::LX_MSG_ERR,"Critical Error", LX_GetError());
        return EXIT_FAILURE;
    }

    LX_Log::setDebugMode();     /// @note Comment this line in release mode

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

    TX_Asset::init();       // Asset
    LX_Random::initRand();  // Initialize the Random Number Generator (RNG)

    if(TX_Asset::getInstance()->readXMLFile() != 0)
    {
        string err_msg = "Cannot load the configuration data: \"" +
                         TX_Asset::getInstance()->getfileName() + "\" ";

        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", err_msg.c_str());
        LX_MSGBox::showMSG(LX_MSGBox::LX_MSG_ERR,"XML file configuration error",
                           err_msg.c_str());
        TX_Asset::destroy();
        LX_Quit();
        return EXIT_FAILURE;
    }

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
        return EXIT_FAILURE;
    }

    LX_VersionInfo::info();
    WinID::setWinID(id);
    ResourceManager::init();
    MainMenu(window).event();

    // Quit the game
    LX_Win::LX_WindowManager::getInstance()->removeWindow(id);
    ResourceManager::destroy();
    TX_Asset::destroy();
    LX_Quit();
    return EXIT_SUCCESS;
}
