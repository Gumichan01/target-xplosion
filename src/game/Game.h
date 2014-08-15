#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED



/*
*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*
*/

/**
*	@file Game.h
*	@brief The game class
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date July 23th, 2014
*
*/


#include<vector>

// Including all specialized engines
#include "../engine/LX_graphics.h"
#include "../engine/LX_ttf.h"
#include "../engine/LX_physics.h"
#include "../engine/LX_audio.h"

#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../entities/Missile.h"

#define FRAMERATE 33.000
#define SECOND 1000.000


class Game{

    //The LX_engine
    LX_graphics * graphics_engine;
    LX_ttf *ttf_engine;
    LX_physics *physics_engine;
    LX_Audio *audio_engine;

    //The datas for the game
    Player *player1;
    std::vector<Missile *> player_missiles;    // The player_missiles vector
    //std::vector<Missile *> enemies_missiles;    // The ennemies vector
    std::vector<Enemy *> enemies;    // The ennemies vector

    int game_Xlimit;
    int game_Ylimit;

    public:

    Game()
    {
        // Initialize the graphic engine
        graphics_engine = LX_graphics::getInstance();

        LX_window *win = LX_window::getInstance();

        game_Xlimit = win->getWidth();
        game_Ylimit = win->getHeight();

        delete win;

        try
        {
            // Initialize the TTF engine
            SDL_Color color = {255,255,255};
            ttf_engine = new LX_ttf("font/AozoraMinchoMedium.ttf",48,&color);

        }
        catch(std::exception ex)
        {
            std::cerr << "Exception occured in LX_ttf : " << ex.what() << std::endl;
            throw ;
        }

        // Initialize the physic and sound engines (no exception can be occurred)
        physics_engine = new LX_physics();
        audio_engine = new LX_Audio();

        player1 = NULL;

    }

    void createPlayer(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic, unsigned int bombs,
                        SDL_Surface *image, Mix_Chunk *audio,
                            int x, int y, int w, int h,int dX, int dY);

    bool play();
    bool input();
    bool out_of_screen(Entity *charac);

    ~Game()
    {
        delete player1;
        delete audio_engine;
        delete physics_engine;
        delete ttf_engine;
        delete graphics_engine;
    }

};











#endif // GAME_ENGINE_H_INCLUDED























