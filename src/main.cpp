

/*
*   Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
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


int main(int argc, char **argv)
{
    LX_Window *window = nullptr;
    int id;     // The ID of the window

#ifdef DEBUG_TX
    LX_Log::setDebugMode();
#endif

    //Initialize The engine
    if(LX_Init() == false)
    {
        string crit_msg = string("Cannot initialize the game engine: ") + LX_GetError();
        LX_SetError(crit_msg);
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s",crit_msg.c_str());
        LX_MSGBox::showMSG(SDL_MESSAGEBOX_ERROR,"Critical Error",LX_GetError(),
                           nullptr);
        return EXIT_FAILURE;
    }

    TX_Asset::init();

    if(TX_Asset::getInstance()->readXMLFile() != 0)
    {
        string err_msg = "Cannot load the configuration data: \"" +
                            TX_Asset::xml_filename + "\" ";

        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,"%s",err_msg.c_str());
        LX_MSGBox::showMSG(SDL_MESSAGEBOX_ERROR,"XML file configuration error",
                           err_msg.c_str(),nullptr);
        TX_Asset::destroy();
        LX_Quit();
        return EXIT_FAILURE;
    }

    // Intialize the RNG
    initRand();

#ifdef DEBUG_TX
    id = TX_Debug::debug_mode(window);
#else
    Game *target_xplosion = nullptr;
    ResultInfo info;

    window = new LX_Window("Target Xplosion v0.5-dev",LX_WINDOW_RENDERING);
    id = LX_WindowManager::getInstance()->addWindow(window);

    if(id == -1)
    {
        delete window;
        LX_Quit();
        return EXIT_FAILURE;
    }

    //Initialize the game
    target_xplosion = Game::init();             // Load the game instance
    Rank::init();

    for(int i = 0; i < 2; i++)
    {
        Rank::setRank(C_RANK);
        if(target_xplosion->play(info,i) == GAME_FINISH)
        {
            displayResult(info);
        }
    }
    Game::destroy();
#endif

    unsigned int idw = static_cast<unsigned int>(id);
    LX_WindowManager::getInstance()->removeWindow(idw);
    delete window;
    TX_Asset::destroy();
    LX_Quit();

    return EXIT_SUCCESS;
}

