

#include "engine/Game_engine.h"

int main ( int argc, char** argv )
{
    Game_engine *engine;

    std::string font_file = "font/AozoraMinchoMedium.ttf";
    SDL_Color color = {255,255,255};
    int size = 48;


    try{

        //engine = new Game_engine(1000,600,32,font_file.c_str(),size,&color);
        //engine = new Game_engine(1600,900,32,font_file.c_str(),size,&color);
        engine = new Game_engine(1280,800,32,font_file.c_str(),size,&color);

    }catch(int){

        return EXIT_FAILURE;
    }

    engine->play();

    delete engine;



    return EXIT_SUCCESS;
}

