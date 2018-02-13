
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017  Luxon Jean-Pierre
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
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#include <LunatiX/LX_AABB.hpp>

class HudHandler;
class Player;
class EnemyRocket;
class Item;
class Level;
class Score;
class Background;
class EntityHandler;

namespace AudioHandler
{
class AudioHDL;
}

namespace LX_Win
{
class LX_Window;
}

struct ResultInfo;
class BGM;

// This enum defines the status of the game
enum EngineStatusV: short {GAME_RUNNING, GAME_QUIT, GAME_FINISH};


struct FrameLimits
{
    int min_x = 0;
    int max_x = 0;
    int min_y = 0;
    int max_y = 0;
};


// The core of the game
class Engine
{
    const size_t DEFAULT_RESERVE = 16;
    const size_t ENEMY_MISSILES_RESERVE = 64;
    const size_t ENEMY_RESERVE = 32;
    const unsigned short OV_VOLUME = 50;
    const unsigned short MUSIC_VOLUME = 50;
    const unsigned short FX_VOLUME = 32;

    static FrameLimits flimits;

    EngineStatusV game_state;
    bool end_of_level;

    // The entities
    Player *player; /// @todo create a class that handle the player
    Item *game_item;

    // Game information
    BGM *bgm;
    Score *score;
    HudHandler& hudhdl;
    EntityHandler& entityhdl;
    AudioHandler::AudioHDL *audiohdl;

    // Level
    Level *level;
    Background *bg;

    // Wwindow
    LX_Win::LX_Window * gw;

    Engine();
    Engine(const Engine& g);
    Engine& operator =(const Engine& g);

    void createPlayer(unsigned int hp, unsigned int att, unsigned int sh,
                      unsigned int critic);

    // To set the background during the level loading
    void setBackground(unsigned int lvl = 0);
    // Load the level and play
    bool loadLevel(const unsigned int lvl);
    EngineStatusV loop(ResultInfo& info);

    // The game logic
    bool input();
    void physics();
    void status();
    void clean();
    void display();

    // Finish the level
    void endLevel();
    void generateResult(ResultInfo& info) const;

    // Item
    void createItem();
    void destroyItem();

public:

    static Engine * init();
    static Engine * getInstance();
    static void destroy();

    static bool outOfBound(const LX_AABB& pos);
    static int getMinXlim();
    static int getMaxXlim();
    static int getMinYlim();
    static int getMaxYlim();

    EngineStatusV play(ResultInfo& info, unsigned int lvl=0);
    void targetPlayer(EnemyRocket * m);
    void bulletCancel();

    Score *getScore() const;

    ~Engine();
};

#endif // GAME_ENGINE_H_INCLUDED
