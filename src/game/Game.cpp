

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

/**
*	@file Game.cpp
*	@brief The game file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

// Including some header files of the engine
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_TrueTypeFont.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Mixer.hpp>
#include <LunatiX/LX_Music.hpp>
#include <LunatiX/LX_FileBuffer.hpp>
#include <LunatiX/LX_Device.hpp>

// Game
#include "Game.hpp"
#include "Rank.hpp"
#include "Result.hpp"

// Enemies
#include "../entities/BasicEnemy.hpp"
#include "../entities/Bachi.hpp"
#include "../entities/Shooter.hpp"
#include "../entities/Tower.hpp"
#include "../entities/boss/SemiBoss01.hpp"
#include "../entities/boss/Boss01.hpp"

// Bullets and item
#include "../entities/Rocket.hpp"
#include "../entities/Bullet.hpp"
#include "../entities/Item.hpp"
#include "../entities/Bomb.hpp"

// Data
#include "../level/Level.hpp"
#include "../level/EnemyData.hpp"
#include "../xml/XMLReader.hpp"

#ifdef DEBUG_TX
#include <iostream>
#endif // DEBUG_TX


using namespace LX_Graphics;
using namespace LX_Physics;
using namespace LX_Mixer;
using namespace LX_FileIO;
using namespace LX_Device;
using namespace Result;

int Game::game_Xlimit = 0;
int Game::game_Ylimit = 0;

static Game *game_instance = nullptr;
static LX_FileBuffer ** spriteRessources = nullptr;
static int fade_out_counter = 0;    // The counter to fade out the screen

const unsigned int SCREEN_FPS = 60;
const Uint32 FRAME_DELAY = (1000 / SCREEN_FPS) + 1;


Game::Game()
    : begin(0), end_of_level(false),window_id(0),
    player1(nullptr), game_item(nullptr),
    level(nullptr), score(nullptr), bg(nullptr), gamepad(nullptr),
    main_music(nullptr), boss_music(nullptr), alarm(nullptr)
{
    LX_Window *g = LX_WindowManager::getInstance()->getWindow(window_id);
    game_Xlimit = g->getWidth();
    game_Ylimit = g->getHeight();
    channelVolume(-1,channelVolume(-1,-1)/2);   // Set the volume

    if(numberOfDevices() > 0)
    {
        gamepad = new LX_Gamepad();

#ifdef DEBUG_TX
        if(gamepad->isConnected())
        {
            char s[64];
            gamepad->toString(s);
            std::cout << s << std::endl;
        }
#endif
    }
}


Game * Game::init()
{
    if(game_instance == nullptr)
    {
        game_instance = new Game();
    }
    return game_instance;
}

Game * Game::getInstance()
{
    return game_instance;
}

void Game::destroy()
{
    delete game_instance;
    game_instance = nullptr;
}

Game::~Game()
{
    delete gamepad;
    delete score;
    delete game_item;
    delete player1;
}


void Game::createPlayer(unsigned int hp, unsigned int att, unsigned int sh,
                        unsigned int critic,
                        SDL_Texture *image, LX_Chunk *audio,
                        int x, int y, int w, int h,float vx, float vy)
{
    SDL_Rect new_pos = {x,y,w,h};
    LX_Vector2D new_speed(vx,vy);

    delete player1;
    player1 = new Player(hp,att,sh,critic,image,audio,
                         new_pos,new_speed,game_Xlimit,game_Ylimit);
}

// Load the important ressources
void Game::loadRessources(void)
{
    Enemy::createMissileRessources();
    Bomb::createExplosionBuffer();
    Bullet::createBulletBuffer();
    Rocket::createParticlesRessources();
    Item::createItemRessources();
    loadEnemySpritesRessources();
}

// Free all ressources
void Game::freeRessources(void)
{
    freeEnemySpritesRessources();
    Item::destroyItemRessources();
    Rocket::destroyParticlesRessources();
    Bullet::destroyBulletBuffer();
    Bomb::destroyExplosionBuffer();
    Enemy::destroyMissileRessources();
}

// Load ressources of enemies (sprites)
void Game::loadEnemySpritesRessources(void)
{
    TX_Asset *asset = TX_Asset::getInstance();
    spriteRessources = new LX_FileBuffer*[ENEMY_SPRITES];

    // Set all the places to nullptr
    for(int i=0;i < ENEMY_SPRITES;i++)
        spriteRessources[i] = nullptr;

    spriteRessources[0] = new LX_FileBuffer(asset->getEnemySpriteFile(0).c_str());
    spriteRessources[1] = new LX_FileBuffer(asset->getEnemySpriteFile(1).c_str());
    spriteRessources[23] = new LX_FileBuffer(asset->getEnemySpriteFile(23).c_str());
    spriteRessources[50] = new LX_FileBuffer(asset->getEnemySpriteFile(50).c_str());
    spriteRessources[100] = new LX_FileBuffer(asset->getEnemySpriteFile(100).c_str());
    spriteRessources[101] = new LX_FileBuffer(asset->getEnemySpriteFile(101).c_str());
    spriteRessources[102] = new LX_FileBuffer(asset->getEnemySpriteFile(102).c_str());
    spriteRessources[103] = new LX_FileBuffer(asset->getEnemySpriteFile(103).c_str());
}


void Game::freeEnemySpritesRessources(void)
{
    for(int i=0;i < ENEMY_SPRITES;i++)
    {
        delete spriteRessources[i];
        spriteRessources[i] = nullptr;
    }

    delete [] spriteRessources;
    spriteRessources = nullptr;
}


bool Game::loadLevel(const unsigned int lvl)
{
    unsigned int hp, att, def, critic;
    std::string str;
    std::string str_music;

    TX_Asset *tx = TX_Asset::getInstance();
    str = tx->getPlayerFile();

    level = new Level(lvl);
    end_of_level = false;

    // The player skills
    hp = 100;
    att = 20;
    def = 12;
    critic = 3;

    att = Rank::attackPlayerUp(att);

    const char * tmp = tx->getLevelMusic(lvl);

    if(tmp == nullptr)
    {
#ifdef DEBUG_TX
        std::cerr << "Cannot load the audio file" << std::endl;
#endif
        return false;
    }

    // It is OK. The music file path was found
    str_music = tmp;

    if(level->isLoaded())
    {
        setBackground();
        loadRessources();

        main_music = loadMusic(str_music);
        alarm = loadSample("audio/alarm.wav");
        SDL_Texture *player_sprite = loadTextureFromFile(str.c_str(),window_id);

        if(lvl != 0)
        {
            hp *= lvl;
            att *= lvl;
            def *= lvl;
            critic *= lvl;
        }

        createPlayer(hp,att,def,critic,player_sprite,nullptr,
                     (game_Xlimit/2)-(PLAYER_WIDTH/2),
                     (game_Ylimit/2)-(PLAYER_HEIGHT/2),
                     PLAYER_WIDTH,PLAYER_HEIGHT,0,0);

        player_missiles.reserve(DEFAULT_RESERVE);
        enemies_missiles.reserve(ENEMY_MISSILES_RESERVE);
        enemies.reserve(ENEMY_RESERVE);

        return true;
    }
    return false;
}


void Game::endLevel(void)
{
    delete boss_music;
    delete alarm;
    delete main_music;
    delete bg;
    delete level;
    delete score;
    delete game_item;

    game_item = nullptr;
    score = nullptr;
    bg = nullptr;
    level = nullptr;
    main_music = nullptr;
    alarm = nullptr;
    boss_music = nullptr;

    freeRessources();
}


GAME_STATUS Game::loop(ResultInfo& info)
{
    GAME_STATUS state = GAME_QUIT;
    bool go = true;
    Uint32 prev_time;                 // The time for the framerate regulation
    Uint32 ticks;

    main_music->volume(MIX_MAX_VOLUME - 32);
    main_music->play();
    allocateChannels(64);
    const int nb_enemies = level->numberOfEnemies();

    LX_Device::mouseCursorDisplay(LX_MOUSE_HIDE);
#ifdef DEBUG_TX
    std::cout << "Number of enemies : " << nb_enemies << std::endl;
#endif
    // Integrate it in LunatiX Engine
    {
        LX_Window *win = LX_WindowManager::getInstance()->getWindow(0);
        SDL_SetRenderDrawBlendMode(win->getRenderer(),SDL_BLENDMODE_BLEND);
    }

    prev_time = SDL_GetTicks();

    while(go && !end_of_level)
    {
        if((go = input()) == false)
            continue;

        // create item
        createItem();

        // The important part
        physics();
        status();
        clean();
        display();
        while(generateEnemy());

        // Framerate regulation
        ticks = (SDL_GetTicks() - prev_time);

        if(ticks < FRAME_DELAY)
        {
            SDL_Delay(FRAME_DELAY - ticks);
        }

        prev_time = SDL_GetTicks();
#ifdef DEBUG_TX
        cycle();
#endif
    }


    // Store information into the result
    generateResult(info);
    info.max_nb_enemies = nb_enemies;

    LX_Device::mouseCursorDisplay(LX_MOUSE_SHOW);
    main_music->stop();
    clearVectors();
    allocateChannels(0);

    // Status of the game
    if(end_of_level)
        state = GAME_FINISH;

    return state;
}


GAME_STATUS Game::play(ResultInfo& info,unsigned int lvl)
{
    GAME_STATUS game_state = GAME_QUIT;

    fade_out_counter = 0;

    if(loadLevel(lvl) == true)
    {
        score = new Score(0);
        begin = SDL_GetTicks();
        game_state = loop(info);
        endLevel();
    }
    else
        std::cerr << "Cannot load the level" << std::endl;

    return game_state;
}

#ifdef DEBUG_TX
void Game::cycle(void)
{
    static long previous_time = 0;
    static int n = 0;
    static int fps = 0;

    n++;

    if(static_cast<long>(SDL_GetTicks() - previous_time) >= 1000)
    {
        fps = n;
        n = 0;
        previous_time = SDL_GetTicks();

        std::cout << "FPS : " << fps << std::endl;
        std::cout << "Enemies : " << enemies.size()
                  << std::endl
                  << "Enemy missiles : " << enemies_missiles.size()
                  << std::endl
                  << "player's missiles : " << player_missiles.size()
                  << std::endl
                  << "items of score : " << items.size()
                  << std::endl
                  << "Death : " << player1->nb_death() << std::endl;
    }
}
#endif


void Game::generateResult(ResultInfo& info)
{
    // Create the result and copy it
    ResultInfo res = {level->getLevelNum(),player1->nb_death(),
                       score->getCurrentScore(),
                       score->getKilledEnemies(),0};
    info = res;
}


bool Game::input(void)
{
    SDL_Event event;
    bool go_on = true;
    static const Uint8 *KEYS = SDL_GetKeyboardState(nullptr);
    static char freq = 1;
    static char continuous_shoot = 0;
    int player_sp = PLAYER_SPEED;

    if(KEYS[SDL_GetScancodeFromKey(SDLK_LSHIFT)])
        player_sp /= 2;

    if(KEYS[SDL_SCANCODE_UP])
        player1->setYvel(-player_sp);

    if(KEYS[SDL_SCANCODE_DOWN])
        player1->setYvel(player_sp);

    if(KEYS[SDL_SCANCODE_LEFT])
        player1->setXvel(-player_sp);

    if(KEYS[SDL_SCANCODE_RIGHT])
        player1->setXvel(player_sp);

    if(KEYS[SDL_GetScancodeFromKey(SDLK_w)] || continuous_shoot == 1)
    {
        // Simple and double Shot
        if(freq%6 == 0)
        {
            if(!player1->isDead())
            {
                playerShot();     // Specify the shot
                freq = 1;
            }
        }
        else
            freq += 1;
    }


    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                go_on = false;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE :
                        go_on = false;
                        break;

                    default :
                        break;
                }
                break;

            case SDL_KEYUP:
            {
                if(player1->isDead())
                {
                    return go_on;
                }

                switch(event.key.keysym.sym)
                {
                    case SDLK_RIGHT :
                        player1->setXvel(0);
                        break;

                    case SDLK_LEFT :
                        player1->setXvel(0);
                        break;

                    case SDLK_UP :
                        player1->setYvel(0);
                        break;

                    case SDLK_DOWN :
                        player1->setYvel(0);
                        break;

                    case SDLK_w :
                        playerShot();
                        break;

                    case SDLK_x :
                        player1->fire(MISSILE_TYPE::ROCKET_TYPE);
                        break;

                    case SDLK_c :
                        player1->fire(MISSILE_TYPE::BOMB_TYPE);
                        break;

                    default :
                        break;
                }
            }
            break;

            case SDL_JOYAXISMOTION :
            {
                inputJoystickAxis(&event);
            }
            break;

            case SDL_JOYBUTTONDOWN :
            {
                // Check the basic shoot button
                if(event.jbutton.button == 7)
                {
                    if(event.jbutton.which == 0) // The first joystick
                    {
                        if(event.jbutton.state == SDL_PRESSED)
                        {
                            continuous_shoot = 1;
                        }
                    }
                }
                // For other buttons
                inputJoystickButton(&event);
            }
            break;

            case SDL_JOYBUTTONUP :
            {
                // Check the basic shoot button
                if(event.jbutton.button == 7)
                {
                    if(event.jbutton.which == 0) // The first joystick
                    {
                        if(event.jbutton.state == SDL_RELEASED)
                        {
                            continuous_shoot = 0;
                        }
                    }
                }
            }
            break;

            default:
                break;
        }
    }

    return go_on;
}


void Game::inputJoystickAxis(SDL_Event *event)
{
    if(event->jaxis.which == 0) // The first joystick
    {
        if(event->jaxis.axis == 0)  // X axis
        {
            if(event->jaxis.value < -JOYSTICK_HIGH_ZONE)
            {
                player1->setXvel(-PLAYER_SPEED);
            }
            else if(event->jaxis.value > JOYSTICK_HIGH_ZONE)
            {
                player1->setXvel(PLAYER_SPEED);
            }
            else if(event->jaxis.value < -JOYSTICK_DEAD_ZONE)
            {
                player1->setXvel(-(PLAYER_SPEED/2));
            }
            else if(event->jaxis.value > JOYSTICK_DEAD_ZONE)
            {
                player1->setXvel(PLAYER_SPEED/2);
            }
            else
            {
                player1->setXvel(0);
            }
        }
        else if(event->jaxis.axis == 1) // Y axis
        {
            if(event->jaxis.value < -JOYSTICK_HIGH_ZONE)
            {
                player1->setYvel(-PLAYER_SPEED);
            }
            else if(event->jaxis.value > JOYSTICK_HIGH_ZONE)
            {
                player1->setYvel(PLAYER_SPEED);
            }
            else if(event->jaxis.value < -JOYSTICK_DEAD_ZONE)
            {
                player1->setYvel(-(PLAYER_SPEED/2));
            }
            else if(event->jaxis.value > JOYSTICK_DEAD_ZONE)
            {
                player1->setYvel(PLAYER_SPEED/2);
            }
            else
            {
                player1->setYvel(0);
            }
        }
    }   // If event->jaxis.which == 0

}


void Game::inputJoystickButton(SDL_Event *event)
{
    if(event->jbutton.which == 0)   // The first joystick
    {
        if(event->jbutton.button == 1)
        {
            if(event->jbutton.state == SDL_PRESSED)
            {
                player1->fire(ROCKET_TYPE);
            }
        }

        if(event->jbutton.button == 0)
        {
            if(event->jbutton.state == SDL_PRESSED)
            {
                player1->fire(BOMB_TYPE);
            }
        }
    }
}


// Add a missile of an enemy
void Game::addEnemyMissile(Missile *m)
{
    enemies_missiles.push_back(m);
}


// Add a missile of the player
void Game::addPlayerMissile(Missile *m)
{
    player_missiles.push_back(m);
}


// Add a new enemy
void Game::addEnemy(Enemy * e)
{
    enemies.push_back(e);
}


void Game::addItem(Item * y)
{
    items.push_back(y);
}


void Game::setBackground(int lvl)
{
    bg = new Background("image/level00-bgd.png",0,0,1600,game_Ylimit,-3);
}

// Create a new item only if it does not exist
void Game::createItem()
{
    if(game_item == nullptr)
    {
        game_item = new Item();
    }
}

// Destroy the item
void Game::destroyItem()
{
    if(game_item->isDead() || game_item->getPowerUp() == POWER_UP::NO_POWER_UP)
    {
        delete game_item;
        game_item = nullptr;
    }
}


void Game::clearVectors(void)
{
    clearPlayerMissiles();
    clearEnemyMissiles();
    clearEnemies();
    clearItems();
}


void Game::clearPlayerMissiles(void)
{
    // Player's missiles
    for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size(); i++)
    {
        if(player_missiles[i] != nullptr)
            delete player_missiles[i];

        player_missiles.erase(player_missiles.begin() + i);
        i--;
    }
}


void Game::clearEnemyMissiles(void)
{
    // Enemies missiles
    for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size(); k++)
    {
        if(enemies_missiles[k] != nullptr)
            delete enemies_missiles[k];

        enemies_missiles.erase(enemies_missiles.begin() + k);
        k--;
    }
}


void Game::clearEnemies(void)
{
    // Enemies
    for(std::vector<Enemy *>::size_type j = 0; j != enemies.size(); j++)
    {
        if(enemies[j] != nullptr)
            delete enemies[j];

        enemies.erase(enemies.begin() + j);
        j--;
    }
}


void Game::clearItems(void)
{
    // Items
    for(std::vector<Item *>::size_type l = 0; l != items.size(); l++)
    {
        if(items[l] != nullptr)
            delete items[l];

        items.erase(items.begin() + l);
        l--;
    }
}

void Game::screenCancel(void)
{
    missileToScore();
    clearEnemyMissiles();
}


void Game::missileToScore(void)
{
    std::vector<Missile *>::size_type n = enemies_missiles.size();

    for(std::vector<Missile *>::size_type i = 0; i != n; i++)
    {
        items.push_back(new Item(enemies_missiles[i]->getX(),
                                 enemies_missiles[i]->getY()));
    }
}



void Game::physics(void)
{
    if(player1->isDead() == false)
    {
        if(collisionCircleRect(*player1->getHitbox(), *game_item->box()))
        {
            player1->takeBonus(game_item->getPowerUp());
            game_item->die();
        }

        for(std::vector<Item *>::size_type l = 0; l != items.size(); l++)
        {
            if(collisionCircleRect(*player1->getHitbox(), *items[l]->box()))
            {
                player1->takeBonus(items[l]->getPowerUp());
                items[l]->die();
            }
        }
    }

    for(std::vector<Enemy *>::size_type j = 0; j != enemies.size(); j++)
    {
        if(!player1->isDead())
        {
            // enemies/player collision
            enemies[j]->collision(player1);
        }

        if(enemies[j]->isDead())
            continue;

        // enemies/missiles collision
        for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size(); i++)
        {
            if(player_missiles[i] == nullptr)
            {
                continue;
            }

            enemies[j]->collision(player_missiles[i]);
        }
    }

    if(!player1->isDead())
    {
        for(std::vector<Missile *>::size_type k = 0; k!= enemies_missiles.size(); k++)
        {
            // enemies missiles/player collision
            player1->collision(enemies_missiles[k]);
        }
    }
}


void Game::status(void)
{
    static Uint32 death_start = 0;
    const Uint32 DELAY_TO_REBORN = 2000;

    if(game_item->getX() <= (-(game_item->getWidth()) - 1))
    {
        game_item->die();
    }
    else if(!game_item->isDead())
    {
        game_item->move();  // Item movement
    }

    // Move the items
    for(std::vector<Item *>::size_type l = 0; l != items.size(); l++)
    {
        if(items[l]->getX() > (-(items[l]->getWidth()) - 1))
        {
            items[l]->move();
        }
        else
        {
            items[l]->die();
        }

    }


    if(!player1->isDead())
    {
        if(player1->isLaserActivated())
        {
            player1->fire(MISSILE_TYPE::LASER_TYPE);
            screenCancel();
        }

        player1->move();
        death_start = SDL_GetTicks();   // Moment of possible death
    }
    else
    {
        if((SDL_GetTicks() - death_start) > DELAY_TO_REBORN)
            player1->reborn();
    }

    // The player's missiles movement
    for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size(); i++)
    {
        if(player_missiles[i] == nullptr)
            continue;

        if(player_missiles[i]->getX() >= game_Xlimit)
            player_missiles[i]->die();
        else
            player_missiles[i]->move();
    }

    // The enemies' missiles movement
    for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size(); k++)
    {
        if(enemies_missiles[k] == nullptr)
            continue;

        if(enemies_missiles[k]->getX() <= (-(enemies_missiles[k]->getWidth()) -1)
           || enemies_missiles[k]->getX() >= game_Xlimit
           || enemies_missiles[k]->getY() <= (-(enemies_missiles[k]->getHeight()) -1)
           || enemies_missiles[k]->getY() >= game_Ylimit)
            enemies_missiles[k]->die();
        else
            enemies_missiles[k]->move();

    }

    // The enemies strategy
    for(std::vector<Enemy *>::size_type j = 0; j != enemies.size() ; j++)
    {
        if(enemies[j]->getX() <= (-(enemies[j]->getWidth()) -1))
            enemies[j]->die();
        else
            enemies[j]->strategy();
    }
}


void Game::clean(void)
{
    destroyItem();     // Try to destroy a dead item

    // Items
    for(std::vector<Item *>::size_type l = 0; l != items.size(); l++)
    {
        if((items[l]->getX() < (-(items[l]->getWidth())) ) || items[l]->isDead())
        {
            delete items[l];
            items.erase(items.begin() + l);
            l--;
        }
    }

    // Missiles of the player
    for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size() ; i++)
    {
        if( player_missiles[i] == nullptr || player_missiles[i]->isDead() )
        {
            delete player_missiles[i];
            player_missiles.erase(player_missiles.begin() + i);
            i--;
        }
    }

    // Missiles of enemies
    for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size(); k++)
    {
        if( enemies_missiles[k] == nullptr || enemies_missiles[k]->isDead() )
        {
            delete enemies_missiles[k];
            enemies_missiles.erase(enemies_missiles.begin() + k);
            k--;
        }
    }

    // Enemies
    for(std::vector<Enemy *>::size_type j = 0; j != enemies.size(); j++)
    {
        if(enemies[j]->isDead())
        {
            if(enemies[j]->killed())
            {
                score->notify(enemies[j]->getMaxHP() + enemies[j]->getATT() + enemies[j]->getDEF(),true);
            }

            delete enemies[j];
            enemies.erase(enemies.begin() + j);
            j--;
        }
    }
}


void Game::display(void)
{
    LX_Window *currentWindow = LX_WindowManager::getInstance()->getWindow(0);

    if(currentWindow == nullptr)
    {
#ifdef DEBUG_TX
        std::cerr << "Cannot display anything " << std::endl;
#endif
        return;
    }

    currentWindow->clearWindow();

    bg->scroll();   // Scroll the brackground
    SDL_Rect tmp = {bg->getX_scroll(),bg->getY_scroll(),bg->getW(),bg->getH()};
    SDL_Rect tmp2 = {(tmp.x + tmp.w),0,tmp.w,tmp.h};

    currentWindow->putTexture(bg->getBackground(),nullptr,&tmp);
    currentWindow->putTexture(bg->getBackground(),nullptr,&tmp2);


    // display player's missiles
    for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size(); i++)
    {
        player_missiles[i]->displayAdditionnalData();
        SDL_Rect *area = player_missiles[i]->getAreaToDisplay();
        currentWindow->putTexture(player_missiles[i]->getTexture(),area, player_missiles[i]->getPos());
    }

    // display the player
    if(!player1->isDead())
    {
        currentWindow->putTexture(player1->getTexture(),nullptr, player1->getPos());
    }

    // Display the items
    for(std::vector<Item *>::size_type l = 0; l != items.size(); l++)
    {
        if(items[l] != nullptr)
            currentWindow->putTexture(items[l]->getTexture(),nullptr,items[l]->getPos());
    }


    // display enemies
    for(std::vector<Enemy *>::size_type j = 0; j != enemies.size(); j++)
    {
        if(enemies[j]->getX() < game_Xlimit)
        {
            SDL_Rect *area = enemies[j]->getAreaToDisplay();
            currentWindow->putTexture(enemies[j]->getTexture(),area, enemies[j]->getPos());
        }
    }

    // Display the item
    if(game_item != nullptr)
    {
        currentWindow->putTexture(game_item->getTexture(),nullptr,game_item->getPos());
    }

    // display enemies' missiles
    for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size(); k++)
    {
        enemies_missiles[k]->displayAdditionnalData();
        SDL_Rect *area = enemies_missiles[k]->getAreaToDisplay();
        currentWindow->putTexture(enemies_missiles[k]->getTexture(),area, enemies_missiles[k]->getPos());
    }

    // End of the level? No ennemy and no incoming ennemies
    if(enemies.size() == 0 && level->numberOfEnemies() == 0)
    {
        if(fade_out_counter < 256)
        {
            SDL_SetRenderDrawColor(currentWindow->getRenderer(),0,0,0,fade_out_counter);
            fade_out_counter++;
            SDL_RenderFillRect(currentWindow->getRenderer(),nullptr);
        }
        else
        {
            fade_out_counter = 0;
            end_of_level = true;
            currentWindow->clearWindow();
        }
    }

    // Display text
    score->display();
    player1->updateHUD();
    currentWindow->update();
}


bool Game::generateEnemy(void)
{
    EnemyData data;

    if(level->statEnemyData(data))
    {
        if((SDL_GetTicks() - begin) > data.time)
        {
            level->popData();
            selectEnemy(&data);

            return true;
        }
    }
    return false;
}


void Game::selectEnemy(EnemyData *data)
{
    SDL_Surface * surface = nullptr;

    if(data->type < ENEMY_SPRITES)
        surface = loadSurfaceFromFileBuffer(spriteRessources[data->type]);

    switch(data->type)
    {
        case 0 :
        {
            enemies.push_back(new SemiBoss01(data->hp,data->att,data->sh,
                                         loadTextureFromSurface(surface),
                                         loadSample("audio/explosion.wav"),
                                         game_Xlimit + 1,data->y,data->w,data->h,-1,1));
        }
        break;

        case 1 :
        {
            boss_music = loadMusic("audio/boss02.ogg");
            haltChannel(-1);
            boss_music->play(-1);
            enemies.push_back(new Boss01(data->hp,data->att,data->sh,
                                         loadTextureFromSurface(surface),
                                         loadSample("audio/explosion.wav"),
                                         game_Xlimit + 1,data->y,data->w,data->h,-4,0));
        }
        break;

        case 22 :
        {
            // Boss is comming ( T_T)
            alarm->play();
        }
        break;
#ifdef DEBUG_TX
        case 23 :
        {
            enemies.push_back(new Shooter(data->hp,data->att,data->sh,
                                              loadTextureFromSurface(surface),
                                              nullptr,game_Xlimit + 1,
                                              data->y,data->w,data->h,-1,0));
        }
        break;
#endif
        case 50 :
        {
            enemies.push_back(new SemiBoss01(data->hp,data->att,data->sh,
                                         loadTextureFromSurface(surface),
                                         loadSample("audio/explosion.wav"),
                                         game_Xlimit + 1,data->y,data->w,data->h,-1,0));
        }
        break;

        case 100 :
        {
            enemies.push_back(new Tower1(data->hp,data->att,data->sh,
                                          loadTextureFromSurface(surface),
                                          nullptr,game_Xlimit + 1,
                                          data->y + 36,data->w,data->h,-1,0));
        }
        break;

        case 101 :
        {
            enemies.push_back(new BasicEnemy(data->hp,data->att,data->sh,
                                              loadTextureFromSurface(surface),
                                              nullptr,game_Xlimit + 1,
                                              data->y,data->w,data->h,-5,0));
        }
        break;

        case 102 :
        {
            enemies.push_back(new Shooter(data->hp,data->att,data->sh,
                                              loadTextureFromSurface(surface),
                                              nullptr,game_Xlimit + 1,
                                              data->y,data->w,data->h,-4,0));
        }
        break;

        case 103 :
        {
            enemies.push_back(new Bachi(data->hp,data->att,data->sh,
                                              loadTextureFromSurface(surface),
                                              nullptr,game_Xlimit + 1,
                                              data->y,data->w,data->h,-7,7));
        }
        break;

        default: break;
    }

    SDL_FreeSurface(surface);
}


void Game::playerShot(void)
{
    const unsigned int l = level->getLevelNum();

    if(l == 0)
    {
        player1->fire(MISSILE_TYPE::DOUBLE_MISSILE_TYPE);
    }
    else if(l < ROCKET_LEVEL_MIN)
    {
        player1->fire(MISSILE_TYPE::BASIC_MISSILE_TYPE);
    }
    else if(l < BOMB_LEVEL_MIN)
    {
        player1->fire(MISSILE_TYPE::DOUBLE_MISSILE_TYPE);
    }
    else if(l < LASER_LEVEL_MIN)
    {
        player1->fire(MISSILE_TYPE::BASIC_MISSILE_TYPE);
        player1->fire(MISSILE_TYPE::WAVE_MISSILE_TYPE);
    }
    else
    {
        player1->fire(MISSILE_TYPE::DOUBLE_MISSILE_TYPE);
        player1->fire(MISSILE_TYPE::WAVE_MISSILE_TYPE);
    }
}


void Game::stopBossMusic(void)
{
    if(boss_music != nullptr)
        boss_music->stop();
}

Score *Game::getScore()
{
    return score;
}


