
#include "engine/LX_lib.h"
#include "game/Game.h"

int main ( int argc, char** argv )
{
    Game *target_xplosion = NULL;

    if(!LX_Init())
    {
        std::cout << "Fail during the engine initialization" << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        target_xplosion = new Game();
    }
    catch(std::exception & game_ex)
    {
        std::cerr << "Exception occured while creating the Target_Xplosion instance : " << game_ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    target_xplosion->play();

    delete target_xplosion;


    return EXIT_SUCCESS;
}

