
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

#include <queue>
#include <vector>
#include <cstdlib>
#include <cstdint>

class HUD;
class Player;
class Missile;
class PlayerRocket;
class EnemyRocket;
class Enemy;
class Item;
class Level;
class Score;
class Background;
class ResourceManager;

namespace AudioHandler
{
class AudioHDL;
}

namespace LX_Win
{
class LX_Window;
}

struct ResultInfo;
struct EnemyData;
class BGM;

// This enum defines the status of the game
enum EngineStatusV: short {GAME_RUNNING, GAME_QUIT, GAME_FINISH};


// The core of the game
class Engine
{
    const size_t DEFAULT_RESERVE = 16;
    const size_t ENEMY_MISSILES_RESERVE = 64;
    const size_t ENEMY_RESERVE = 32;
    const unsigned short OV_VOLUME = 50;
    const unsigned short MUSIC_VOLUME = 50;
    const unsigned short FX_VOLUME = 32;

    static int game_minXlimit;
    static int game_maxXlimit;
    static int game_minYlimit;
    static int game_maxYlimit;
    static uint8_t fade_out_counter;    // The counter to fade out the screen

    EngineStatusV game_state;
    uint32_t start_point;               // Point where the game time start
    bool end_of_level;

    // The entities
    Player *player;
    Item *game_item;
    std::vector<Missile *> player_missiles;
    std::vector<Missile *> enemies_missiles;
    std::queue<Missile *> emissiles_queue;
    std::vector<Enemy *> enemies;
    std::vector<Item *> items;
    std::vector<HUD *> huds;

    // Game information
    BGM *bgm;
    Level *level;
    Score *score;
    Background *bg;
    AudioHandler::AudioHDL *audiohdl;

    // Resources and window
    ResourceManager *resources;
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

    // Clean up
    void clearVectors();
    void clearPlayerMissiles();
    void clearEnemyMissiles();
    void clearEnemies();
    void clearItems();
    // Debug information
    void debugInfo();
    // Display
    void displayHUD() const;
    // Screen cancel
    void missileToScore();
    // Item
    void createItem();
    void destroyItem();
    // Push enemies in the game
    bool generateEnemy();
    // End of the game
    void updateHUD();

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

    void acceptEnemyMissile(Missile * m);
    void acceptEnemy(Enemy *e);
    void acceptPlayerMissile(Missile * m);
    void targetEnemy(PlayerRocket * m);
    void targetPlayer(EnemyRocket * m);
    void acceptItem(Item * y);
    void acceptHUD(HUD * h);

    void bulletCancel();
    Score *getScore() const;

    ~Engine();
};

#endif // GAME_ENGINE_H_INCLUDED
