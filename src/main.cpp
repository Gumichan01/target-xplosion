

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

#include <LunatiX/Lunatix_engine.hpp>

#include "game/Game.hpp"
#include "game/Rank.hpp"
#include "game/Result.hpp"
#include "xml/XMLReader.hpp"

#ifdef DEBUG_TX
#include "debug/TX_Debug.hpp"
#endif

using namespace std;
using namespace LX_Graphics;
using namespace LX_Random;
using namespace Result;


int main (int argc, char** argv)
{
    LX_Window *window = NULL;
    int id;     // The ID of the window

    //Initialize The engine
    if(LX_Init() == false)
    {
        cerr << "Fail during the engine initialization" << endl;
        return EXIT_FAILURE;
    }


    TX_Asset::init();

    if(TX_Asset::getInstance()->readXMLFile() != 0)
    {
        LX_MSGBox::showMSG(SDL_MESSAGEBOX_ERROR,"XML file error","Cannot load the configuration data",NULL);
        TX_Asset::destroy();
        LX_Quit();
        return EXIT_FAILURE;
    }

    // Intialize the RNG
    initRand();

#ifdef DEBUG_TX
    id = TX_Debug::debug_mode(window);
#else

    Game *target_xplosion = NULL;
    ResultInfo info;

    window = new LX_Window("Target Xplosion v0.5-dev",LX_WINDOW_RENDERING);
    id = LX_Graphics::LX_WindowManager::getInstance()->addWindow(window);

    if(id == -1)
    {
        delete window;
        LX_Quit();
        return EXIT_FAILURE;
    }

    //Initialize the game
    target_xplosion = Game::init();             // loading the game instance
    Rank::init();

    for(int i = 0;i < 2;i++)
    {
        Rank::setRank(C_RANK);
        if(target_xplosion->play(info,i) == GAME_FINISH)
        {
            displayResult(info);
        }
    }

    Game::destroy();

#endif

    LX_Graphics::LX_WindowManager::getInstance()->removeWindow(id);
    delete window;

    TX_Asset::destroy();
    LX_Quit();

    return EXIT_SUCCESS;
}


