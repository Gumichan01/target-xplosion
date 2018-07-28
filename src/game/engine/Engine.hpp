
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#include <Lunatix/ImgRect.hpp>

namespace lx
{

namespace Win
{
class Window;
}

}

namespace AudioHandler
{
class AudioHDL;
}

class HudHandler;
class Player;
class EnemyRocket;
class Item;
class Level;
class Score;
class Background;
class EntityHandler;
class PlayerHandler;
class BGM;
struct ResultInfo;

// This enum defines the status of the game
enum EngineStatus
{
    GAME_RUNNING,
    GAME_QUIT,
    GAME_FINISH
};


struct FrameLimits final
{
    Float min_x = { 0.0f };
    Float max_x = { 0.0f };
    Float min_y = { 0.0f };
    Float max_y = { 0.0f };
};


// The core of the game
class Engine final
{
    const unsigned int DEFAULT_RESERVE = 16;
    const unsigned int ENEMY_MISSILES_RESERVE = 64;
    const unsigned int ENEMY_RESERVE = 32;
    const unsigned short OV_VOLUME = 50;
    const unsigned short MUSIC_VOLUME = 50;
    const unsigned short FX_VOLUME = 32;

    static FrameLimits flimits;

    EngineStatus game_state;
    bool end_of_level;

    // The entities
    Item * game_item;

    // Game information
    BGM * bgm;
    Score * score;
    HudHandler& hudhdl;
    EntityHandler& entityhdl;
    PlayerHandler& playerhdl;
    AudioHandler::AudioHDL * audiohdl;

    // Level
    Level * level;
    Background * bg;

    // Wwindow
    lx::Win::Window& gw;

    Engine();
    Engine( const Engine& g ) = delete;
    Engine& operator =( const Engine& g ) = delete;

    void createPlayer( unsigned int hp, unsigned int att, unsigned int sh,
                       unsigned int critic );

    // To set the background during the level loading
    void setBackground( unsigned int lvl = 0 );
    // Load the level and play
    bool loadLevel( const unsigned int lvl );
    EngineStatus loop( ResultInfo& info );

    // The game logic
    bool input();
    void physics();
    void status();
    void clean();
    void display();

    // Finish the level
    void endLevel();
    void generateResult( ResultInfo& info ) const;

    // Item
    void createItem();
    void destroyItem();

public:

    static Engine * getInstance();
    static void destroy();

    static bool outOfBound( const lx::Physics::FloatingBox& fpos ) noexcept;
    static Float getMinXlim() noexcept;
    static Float getMaxXlim() noexcept;
    static Float getMinYlim() noexcept;
    static Float getMaxYlim() noexcept;

    EngineStatus play( ResultInfo& info, unsigned int lvl = 0 );
    void targetPlayer( EnemyRocket * m );
    void bulletCancel();

    Score * getScore() const;

    ~Engine() = default;
};

#endif // GAME_ENGINE_H_INCLUDED
