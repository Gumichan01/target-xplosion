

/*
*   Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2014  Luxon Jean-Pierre
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


#include "engine/LX_lib.h"
#include "game/Game.h"

int main ( int argc, char** argv )
{
    Game *target_xplosion = NULL;
    LX_window *game_window = NULL;

    //Initialize the LunatiX_engine
    if(!LX_Init())
    {
        std::cout << "Fail during the engine initialization" << std::endl;
        return EXIT_FAILURE;
    }

    //Initializie the game
    try
    {
        game_window = LX_window::getInstance();     // open a new window
        target_xplosion = Game::init();             // loading the game instance
    }
    catch(std::exception & game_ex)
    {
        std::cerr << "Exception occured while creating the Target_Xplosion instance : " << game_ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    target_xplosion->play();

    target_xplosion->destroy();
    game_window->destroy();
    LX_Quit();

    return EXIT_SUCCESS;
}



