

/*
*   Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2015  Luxon Jean-Pierre
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Luxon Jean-Pierre (Gumichan01)
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file TX_Debug.hpp
*	@brief The debuging file
*	@author Luxon Jean-Pierre(Gumichan01)
*/

#include <iostream>
#include <LunatiX/Lunatix_engine.hpp>

#include "../game/Game.hpp"
#include "../game/Rank.hpp"
#include "../game/Result.hpp"
#include "../xml/XMLReader.hpp"

#include "TX_Debug.hpp"

using namespace std;
using namespace Result;
using namespace LX_Graphics;

namespace TX_Debug{

// Laucnh the game in debug mode
int debug_mode(LX_Window *window)
{
    int id;
    unsigned int debug_lvl;
    unsigned int debug_rank;

    Game * target_xplosion = nullptr;
    ResultInfo info = {0,0,59999,250,256};  // Default values of the result

    // Select the level
    cout << "Select the level ID:" << endl;
    cin >> debug_lvl;

    cout << "Rank (0: C rank, 1: B rank, 2: A rank, 3: S rank):" << endl;
    cin >> debug_rank;

    if(debug_lvl > S_RANK)
    {
        cerr << "Invalid level ID: " << debug_lvl << endl;
        return -1;
    }

    if(debug_rank > S_RANK)
    {
        cerr << "Invalid rank: " << debug_rank << endl;
        return -1;
    }

    window = new LX_Window("Target Xplosion - Level Debug",LX_WINDOW_RENDERING);
    id = LX_Graphics::LX_WindowManager::getInstance()->addWindow(window);

    if(id == -1)
    {
        cerr << "Cannot store the window in the window manager" << endl;
        delete window;
        return id;
    }

    target_xplosion = Game::init();

    // Play the level defined by the player
    Rank::setRank(debug_rank);
    if(target_xplosion->play(info,debug_lvl) == GAME_FINISH)
    {
        displayResultConsole(info);
        displayResult(info);
    }

    Game::destroy();

    return id;
}


};



