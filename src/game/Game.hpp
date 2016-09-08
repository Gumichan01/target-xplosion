
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
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
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/

#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#include <vector>
#include <LunatiX/LX_Gamepad.hpp>


class HUD;
class Player;
class Missile;
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
enum GAME_STATUS: short {GAME_RUNNING,GAME_PAUSE,GAME_QUIT,GAME_FINISH};


// The core of the game
class Game
{
    const std::vector<Missile *>::size_type DEFAULT_RESERVE = 16;
    const std::vector<Missile *>::size_type ENEMY_MISSILES_RESERVE = 64;
    const std::vector<Enemy *>::size_type ENEMY_RESERVE = 32;
    const int ALARM_STR_ID = 4;
    const int CHANNELS = 64;
    const int OV_VOLUME = 64;
    const int FX_VOLUME = 50;

    static int game_Xlimit;
    static int game_Ylimit;
    static uint8_t fade_out_counter;    // The counter to fade out the screen

    GAME_STATUS game_state;
    bool pause_allowed;                 // Prevent the pause during a boss fight
    uint32_t start_point;               // Point where the game time start
    uint32_t game_duration;                // Point where the game time is saved
    bool end_of_level;
    unsigned int window_id;

    // The entities
    HUD * hud;
    Player *player;
    Item *game_item;
    std::vector<Missile *> player_missiles;
    std::vector<Missile *> enemies_missiles;
    std::vector<Enemy *> enemies;
    std::vector<Item *> items;

    Level *level;
    Score *score;
    Background *bg;
    LX_Device::LX_Gamepad *gamepad;
    LX_Mixer::LX_Music *main_music;
    LX_Mixer::LX_Music *boss_music;
    LX_Mixer::LX_Sound *alarm;

    ResourceManager *resources;
    LX_Win::LX_Window * current_window;


    Game();
    Game(const Game& g);

    // To set the background during the level loading
    void setBackground(unsigned int lvl=0);

    // Load the level and play
    bool loadLevel(const unsigned int lvl);
    GAME_STATUS loop(ResultInfo& info);

    // The game logic
    bool input();
    void physics();
    void status();
    void clean();
    void display();

    // Finish the level
    void endLevel();
    void generateResult(ResultInfo& info);

    // Clear the content of all vectors
    void clearVectors();
    void clearPlayerMissiles();
    void clearEnemyMissiles();
    void clearEnemies();
    void clearItems();

    // Ressources
    void loadRessources();
    void freeRessources();

    // Calculate the FPS
    void cycle();

    // Display
    void scrollAndDisplayBackground();
    void displayPlayerMissiles();
    void displayItems();
    void displayEnemies();
    void displayEnemyMissiles();

    // The shots of the player
    void playerShot();
    void missileToScore();

    // Item
    void createItem();
    void destroyItem();

    // Launch enemy
    bool generateEnemy();

    void takeScreenshot();
    void screenFadeOut();

public:

    static Game * init();
    static Game * getInstance();
    static void destroy();

    static int getXlim();
    static int getYlim();
    static void pause(uint32_t& tstart_point, uint32_t& tduration);
    static void resume(uint32_t& tstart_point, uint32_t& tduration);

    void createPlayer(unsigned int hp, unsigned int att, unsigned int sh,
                      unsigned int critic, LX_Graphics::LX_Sprite *image,
                      LX_Mixer::LX_Sound *audio,
                      int x, int y, int w, int h,float vx, float vy);

    GAME_STATUS play(ResultInfo& info,unsigned int lvl=0);
    void pause();
    void resume();
    void stopBossMusic();

    void acceptEnemyMissile(Missile * m);
    void acceptPlayerMissile(Missile * m);
    void acceptMissile(Missile * m);
    void acceptEnemy(Enemy *e);
    void acceptItem(Item * y);
    void acceptPlayerInput();

    void screenCancel();
    Score *getScore() const;

    ~Game();
};

#endif // GAME_ENGINE_H_INCLUDED
