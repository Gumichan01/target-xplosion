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
*	@file Game_engine.h
*	@brief The game engine library
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date July 23th, 2014
*
*/


#include<vector>

// Including all specialized engines
#include "TX_graphics.h"
#include "TX_ttf.h"
#include "TX_physics.h"
#include "TX_audio.h"

#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../entities/Missile.h"

#define FRAMERATE 33.000
#define SECOND 1000.000


class Game_engine{

    //The TX_engine
    TX_graphics * graphics_tx;
    TX_ttf *ttf_tx;
    TX_physics *physics_tx;
    TX_Audio *audio_tx;

    //The datas for the game
    Player *player1;
    std::vector<Missile *> player_missiles;    // The player_missiles vector
    //std::vector<Missile *> enemies_missiles;    // The ennemies vector
    std::vector<Enemy *> enemies;    // The ennemies vector

    public:

    Game_engine(unsigned int width, unsigned int height, unsigned int bpp,
                    std::string ttf_filename, unsigned int size, SDL_Color *color)
    {

        try{    // Initialize the graphic engine

            graphics_tx = new TX_graphics(width,height,bpp);

        }catch(int e){

            fprintf(stderr,"\nException occured in Game_engine constructor during the graphic engine initialization : %d \n", e);
            throw e;
        }

        try{    // Initialize the TTF engine

            ttf_tx = new TX_ttf(ttf_filename, size,color, graphics_tx->getScreen());

        }catch(int ex){

            fprintf(stderr,"\nException occured in Game_engine constructor during the TTF engine initialization : %d \n", ex);
            throw ex;
        }

        // Initialize the physic and sound engines (no exception can be occurred)
        physics_tx = new TX_physics();
        audio_tx = new TX_Audio();

        player1 = NULL;

    }

    void createPlayer(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic, unsigned int bombs,
                        SDL_Surface *image, Mix_Chunk *audio,
                            int x, int y, int w, int h,int dX, int dY);

    bool play();
    bool input();
    bool out_of_screen(Entity *charac);
    void check_player_action();

    ~Game_engine()
    {
        delete player1;
        delete audio_tx;
        delete physics_tx;
        delete ttf_tx;
        delete graphics_tx;
    }

};











#endif // GAME_ENGINE_H_INCLUDED























