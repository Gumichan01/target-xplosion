
/*
*	Target_Xplosion - The classic shoot'em up video game
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


#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED


/**
*	@file Game.hpp
*	@brief The game class
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/


#include <vector>
#include <LunatiX/LX_Gamepad.hpp>

#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../entities/Missile.hpp"

#include "Background.hpp"
#include "scoring.hpp"

#define DEBUG_TX

#define RESERVE 16          // The minimum number of enemies in the vector


#define JOYSTICK_DEAD_ZONE 8000
#define JOYSTICK_HIGH_ZONE 24000


struct SDL_Texture;
struct SDL_Rect;
union SDL_Event;

class Item;
class Level;


namespace LX_Mixer
{

class LX_Chunk;
class LX_Music;

};


struct ResultInfo;


// This enum defines the status of the game
typedef enum GAME_STATUS {GAME_QUIT,GAME_FINISH} GAME_STATUS;


class Game
{
    //The entities
    Player *player1;
    Item *game_item;
    std::vector<Missile *> player_missiles;     // The player's missiles vector
    std::vector<Missile *> enemies_missiles;    // The ennemies' missiles vector
    std::vector<Enemy *> enemies;               // The ennemies vector
    unsigned int windowID;
    bool endOfLevel;

    Level *level;
    LX_Music *mainMusic;
    LX_Music *bossMusic;
    LX_Chunk *alarm;
    Background *bg;
    LX_Device::LX_Gamepad *gamepad;

    void setBackground(int lvl=0);
    void clean_up(void);

    Game();

    bool loadLevel(const unsigned int lvl);
    GAME_STATUS loop(ResultInfo *info);
    void cycle(void);
    void endLevel(void);

    bool input(void);
    void inputJoystickAxis(SDL_Event *event);
    void inputJoystickButton(SDL_Event *event);
    void createItem();
    void destroyItem();

    void physics(void);
    void status(void);
    void clean(void);
    void display(void);
    bool generateEnemy(void);
    void player_shoot(void);

public:

    static int game_Xlimit;
    static int game_Ylimit;

    static Score *score;

    static Game * init();
    static Game * getInstance();
    static void destroy();

    void createPlayer(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic,
                      SDL_Texture *image, LX_Chunk *audio,
                      int x, int y, int w, int h,int dX, int dY);

    GAME_STATUS play(ResultInfo *info,unsigned int lvl=0);
    void stopBossMusic(void);

    void addEnemyMissile(Missile * m);
    void addPlayerMissile(Missile * m);

    ~Game();

};


#endif // GAME_ENGINE_H_INCLUDED


