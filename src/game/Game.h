

/*
*	Target_Xplosion - The classic shoot'em up video game
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


#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED


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

#define FRAMERATE 33.000    // the desired framerate
#define SECOND 1000.000     // One second in millisecond

#define DEFAULT_MOUSE_SENSIBILITY 1


class Game{

    //The LX_engine
    LX_graphics * graphics_engine;
    LX_ttf *ttf_engine;
    LX_physics *physics_engine;
    LX_Audio *audio_engine;

    //The datas for the game
    Player *player1;
    std::vector<Missile *> player_missiles;     // The player's missiles vector
    std::vector<Missile *> enemies_missiles;    // The ennemies' missiles vector
    std::vector<Enemy *> enemies;               // The ennemies vector

    int game_Xlimit;
    int game_Ylimit;


    Game()
    {
        // Initialize the graphic engine
        graphics_engine = LX_graphics::getInstance();
        LX_window *win = LX_window::getInstance();

        game_Xlimit = win->getWidth();
        game_Ylimit = win->getHeight();

        // Initialize the TTF engine
        SDL_Color color = {255,255,255};

        ttf_engine = new LX_ttf(&color);

        // Initialize the physic and sound engines (no exception can be occurred)
        physics_engine = new LX_physics();

        audio_engine = new LX_Audio();

        player1 = NULL;

    }

    public:


    static Game * init();
    static Game * getInstance();
    static void destroy();

    void createPlayer(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic, unsigned int bombs,
                        SDL_Surface *image, Mix_Chunk *audio,
                            int x, int y, int w, int h,int dX, int dY);

    bool play();
    bool input();

    void addEnemyMissile(Missile * m);


    ~Game()     ///@todo destroy all instances in the vectors
    {
        delete player1;
        delete audio_engine;
        delete physics_engine;
        delete ttf_engine;
        graphics_engine->destroy();
    }

};











#endif // GAME_ENGINE_H_INCLUDED























