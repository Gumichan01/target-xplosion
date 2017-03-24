
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

#include <LunatiX/LX_Gamepad.hpp>
#include <vector>

class HUD;
class Player;
class Missile;
class EnemyRocket;
class Enemy;
class Item;
class Level;
class Score;
class Background;
class ResourceManager;

namespace LX_Mixer
{
class LX_Sound;
class LX_Music;
};

namespace LX_Graphics
{
class LX_Sprite;
};

namespace LX_Win
{
class LX_Window;
};

struct ResultInfo;
struct EnemyData;

// This enum defines the status of the game
enum GameStatusV: short {GAME_RUNNING, GAME_QUIT, GAME_FINISH};


// The core of the game
class Game
{
    const size_t DEFAULT_RESERVE = 16;
    const size_t ENEMY_MISSILES_RESERVE = 64;
    const size_t ENEMY_RESERVE = 32;
    const int ALARM_STR_ID = 4;
    const int CHANNELS = 64;
    const int NORMAL_CHANNELS = 8;
    const int OV_VOLUME = 64;
    const int MUSIC_VOLUME = 50;
    const int FX_VOLUME = 32;

    static int game_minXlimit;
    static int game_maxXlimit;
    static int game_minYlimit;
    static int game_maxYlimit;
    static uint8_t fade_out_counter;    // The counter to fade out the screen

    GameStatusV game_state;
    uint32_t start_point;               // Point where the game time start
    bool end_of_level;

    // The entities
    Player *player;
    Item *game_item;
    std::vector<Missile *> player_missiles;
    std::vector<Missile *> enemies_missiles;
    std::vector<Enemy *> enemies;
    std::vector<Item *> items;
    std::vector<HUD *> huds;

    Level *level;
    Score *score;
    Background *bg;
    LX_Device::LX_Gamepad gamepad;
    /// @todo (#1#) Create a music handler
    LX_Mixer::LX_Music *main_music;
    LX_Mixer::LX_Music *boss_music;
    LX_Mixer::LX_Sound *alarm;

    // Resources and window
    ResourceManager *resources;
    LX_Win::LX_Window * gw;

    Game();
    Game(const Game& g);

    // To set the background during the level loading
    void setBackground(unsigned int lvl=0);
    // Load the level and play
    bool loadLevel(const unsigned int lvl);
    GameStatusV loop(ResultInfo& info);

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
    // Calculate the FPS
    void cycle();
    // Display
    void displayPlayerMissiles() const;
    void displayItems() const;
    void displayEnemies() const;
    void displayEnemyMissiles() const;
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

    static Game * init();
    static Game * getInstance();
    static void destroy();

    static int getMinXlim();
    static int getMaxXlim();
    static int getMinYlim();
    static int getMaxYlim();

    void createPlayer(unsigned int hp, unsigned int att, unsigned int sh,
                      unsigned int critic, LX_Graphics::LX_Sprite *image,
                      LX_Mixer::LX_Sound *audio,
                      int x, int y, int w, int h, float vx, float vy);

    GameStatusV play(ResultInfo& info, unsigned int lvl=0);
    void stopBossMusic();

    void acceptEnemyMissile(Missile * m);
    void acceptEnemy(Enemy *e);
    void acceptPlayerMissile(Missile * m);
    void targetEnemy(Missile * m);
    void targetPlayer(EnemyRocket * m);
    void acceptItem(Item * y);
    void acceptHUD(HUD * h);

    void screenCancel();
    Score *getScore() const;

    ~Game();
};

#endif // GAME_ENGINE_H_INCLUDED
