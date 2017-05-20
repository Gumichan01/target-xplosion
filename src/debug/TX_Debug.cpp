
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
*   website: gumichan01.kappatau.fr
*   mail: luxon.jean.pierre@gmail.com
*/

#include "TX_Debug.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/Result.hpp"
#include "../level/Level.hpp"
#include "../resources/ResourceManager.hpp"
#include "../resources/WinID.hpp"

#include <LunatiX/Lunatix.hpp>

using namespace std;
using namespace Result;
using namespace LX_Win;

namespace TX_Debug
{

// Laucnh the game in debug mode
void debug_mode()
{
    unsigned int debug_lvl;

    Engine * target_xplosion = nullptr;
    ResultInfo info = {0,0,0,0,0,0,0};

    cerr.flush();
    cerr << "\n ====================================" << endl;
    cerr << "     Target Xplosion - Debug mode    "   << endl;
    cerr << " ====================================\n" << endl;
    cerr.flush();
    // Select the level
    cerr << "Select the level ID: ";
    cin >> debug_lvl;

    if(debug_lvl > Level::MAX_LEVEL)
    {
        cerr << "Invalid level ID: " << debug_lvl << endl;
        return;
    }

    LX_WindowInfo winfo;
    LX_initWindowInfo(winfo);
    winfo.title = "Target Xplosion - Level Debug";
    winfo.w = 1280;
    winfo.h = 768;
    LX_Window window(winfo);
    window.setDrawBlendMode(LX_BLENDMODE_BLEND);
    uint32_t id = LX_Win::LX_WindowManager::getInstance()->addWindow(&window);

    if(id == static_cast<uint32_t>(-1))
    {
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Internal error: %s",
                         LX_GetError());
        return;
    }

    WinID::setWinID(id);
    ResourceManager::init();
    LX_Device::LX_Gamepad gamepad;
    target_xplosion = Engine::init();

    if(LX_Device::numberOfDevices() > 0)
        gamepad.open(0);

    // Play the level defined by the player
    if(target_xplosion->play(info, debug_lvl) == GAME_FINISH)
        displayResult(info);

    gamepad.close();
    Engine::destroy();
    ResourceManager::destroy();
}

}
