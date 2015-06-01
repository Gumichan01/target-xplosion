

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
*	@file Game.hpp
*	@brief The game class
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/


#include<vector>

// Including all specialized engines
#include "../engine/LX_Graphics.hpp"
#include "../engine/LX_Window.hpp"
#include "../engine/LX_TrueTypeFont.hpp"
#include "../engine/LX_Physics.hpp"
#include "../engine/LX_Mixer.hpp"

#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../entities/Missile.hpp"

#include "Background.hpp"
#include "scoring.hpp"


#define FRAMERATE 60.000    // the desired framerate
#define SECOND 1000.000     // One second in millisecond

#define RESERVE 16


class Item;

class Game{

    //The LX_engine
    LX_Graphics * graphics_engine;
    LX_TrueTypeFont *ttf_engine;

    //The entities
    Player *player1;
    Item *game_item;
    std::vector<Missile *> player_missiles;     // The player's missiles vector
    std::vector<Missile *> enemies_missiles;    // The ennemies' missiles vector
    std::vector<Enemy *> enemies;               // The ennemies vector

    //The background image of the game
    Background *bg;

    //Scoring system
    Score *score;

    //The limits
    static int game_Xlimit;
    static int game_Ylimit;


    void setBackground();
    void clean_up(void);

    Game();

    bool input();

    void createItem();
    void destroyItem();

    public:


    static Game * init();
    static Game * getInstance();
    static void destroy();

    void createPlayer(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic,
                        SDL_Texture *image, LX_Chunk *audio,
                            int x, int y, int w, int h,int dX, int dY);

    bool play();

    void addEnemyMissile(Missile * m);
    void addPlayerMissile(Missile * m);


    static int getGameW(){ return game_Xlimit;}
    static int getGameH(){ return game_Ylimit;}


    ~Game();

};











#endif // GAME_ENGINE_H_INCLUDED























