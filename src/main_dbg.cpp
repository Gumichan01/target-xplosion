
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
*   website: gumichan01.kappatau.fr
*   mail: luxon.jean.pierre@gmail.com
*/

#include "game/Game.hpp"
#include "game/Result.hpp"
#include "asset/TX_Asset.hpp"
#include "debug/TX_Debug.hpp"

#include <LunatiX/Lunatix.hpp>

using namespace std;
using namespace LX_Random;
using namespace Result;

#if defined(__WIN32__)
int main(int argc, char** argv)
#else
int main()
#endif
{
    int err = 0;

    //Initialize The engine
    if(LX_Init() == false)
    {
        string crit_msg = string("Cannot initialize the game engine: ") + LX_GetError();
        LX_SetError(crit_msg);
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s",crit_msg.c_str());
        LX_MSGBox::showMSG(LX_MSG_ERR,"Critical Error",LX_GetError());
        return EXIT_FAILURE;
    }

    LX_Log::setDebugMode();
    TX_Asset::init();
    initRand();

    if(TX_Asset::getInstance()->readXMLFile() != 0)
    {
        string err_msg = "Cannot load the configuration data: \"" +
                         TX_Asset::getInstance()->getfileName() + "\" ";

        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,"%s",err_msg.c_str());
        LX_MSGBox::showMSG(LX_MSG_ERR,"XML file configuration error",err_msg.c_str());
        TX_Asset::destroy();
        LX_Quit();
        return EXIT_FAILURE;
    }

    TX_Debug::debug_mode();
    TX_Asset::destroy();
    LX_Quit();
    return err ? EXIT_FAILURE : EXIT_SUCCESS;
}
