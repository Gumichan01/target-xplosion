
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
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
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/


#include "TX_Debug.hpp"
#include "../game/Game.hpp"
#include "../game/Rank.hpp"
#include "../game/Result.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/Lunatix.hpp>

using namespace std;
using namespace Result;
using namespace LX_Win;

namespace TX_Debug
{

// Laucnh the game in debug mode
void debug_mode()
{
    int id;
    unsigned int debug_lvl;
    unsigned int debug_rank;

    Game * target_xplosion = nullptr;
    ResultInfo info = {0,0,59999,0,250,256};  // Default values of the result

    cerr.flush();
    cerr << "\n ====================================" << endl;
    cerr << "     Target Xplosion - Debug mode    "   << endl;
    cerr << " ====================================\n" << endl;
    cerr.flush();
    // Select the level
    cerr << "Select the level ID: ";
    cin >> debug_lvl;
    cerr << "Rank (0: C rank, 1: B rank, 2: A rank, 3: S rank): ";
    cin >> debug_rank;

    if(debug_lvl > S_RANK)
    {
        cerr << "Invalid level ID: " << debug_lvl << endl;
        return;
    }
    else if(debug_rank > S_RANK)
    {
        cerr << "Invalid rank: " << debug_rank << endl;
        return;
    }

    LX_WindowInfo winfo;
    LX_initWindowInfo(winfo);
    winfo.title = "Target Xplosion - Level Debug";
    winfo.w = 1280;
    winfo.h = 768;
    LX_Window window(winfo);
    id = LX_WindowManager::getInstance()->addWindow(&window);

    if(id == -1)
    {
        cerr << "Cannot store the window in the window manager" << endl;
        return;
    }

    ResourceManager::init();
    target_xplosion = Game::init();

    // Play the level defined by the player
    Rank::setRank(debug_rank);
    if(target_xplosion->play(info,debug_lvl) == GAME_FINISH)
    {
        displayResultConsole(info);
        displayResult(info);
    }

    Game::destroy();
    ResourceManager::destroy();
}

};
