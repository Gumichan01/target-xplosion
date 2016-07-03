

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
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
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file Game.cpp
*	@brief The game file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <sstream>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

// Including some header files of the engine
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Mixer.hpp>
#include <LunatiX/LX_Music.hpp>
#include <LunatiX/LX_Device.hpp>
#include <LunatiX/LX_Log.hpp>

// Game
#include "Game.hpp"
#include "hud.hpp"
#include "Rank.hpp"
#include "Result.hpp"
#include "Background.hpp"
#include "scoring.hpp"
#include "PlayerInput.hpp"
#include "Framerate.hpp"

// Enemies
#include "../entities/Player.hpp"
#include "../entities/BasicEnemy.hpp"
#include "../entities/Bachi.hpp"
#include "../entities/Shooter.hpp"
#include "../entities/Tower.hpp"
#include "../entities/boss/SemiBoss01.hpp"
#include "../entities/boss/Boss01.hpp"

// Bullets and item
#include "../entities/Rocket.hpp"
#include "../entities/Bullet.hpp"
#include "../entities/Bomb.hpp"

// Data
#include "../level/Level.hpp"

#include "../resources/EnemyInfo.hpp"
#include "../resources/ResourceManager.hpp"

#ifdef DEBUG_TX
#include <iostream>
#endif // DEBUG_TX


using namespace LX_Graphics;
using namespace LX_Physics;
using namespace LX_Mixer;
using namespace LX_Device;
using namespace Result;

int Game::game_Xlimit = 0;
int Game::game_Ylimit = 0;
Uint8 Game::fade_out_counter = 0;

static Game *game_instance = nullptr;

const unsigned int SCREEN_FPS = 60;
const Uint32 FRAME_DELAY = (1000 / SCREEN_FPS) + 1;


Game::Game()
    : begin(0), end_of_level(false),window_id(0),
      hud(nullptr),player1(nullptr), game_item(nullptr),
      level(nullptr), score(nullptr), bg(nullptr), gamepad(nullptr),
      main_music(nullptr), boss_music(nullptr), alarm(nullptr),
      resources(nullptr)
{
    ResourceManager::init();
    resources = ResourceManager::getInstance();

    LX_Window *g = LX_WindowManager::getInstance()->getWindow(window_id);
    game_Xlimit = g->getWidth();
    game_Ylimit = g->getHeight();
    channelVolume(-1,channelVolume(-1,-1)/2);       // Set the volume

    if(numberOfDevices() > 0)
        gamepad = new LX_Gamepad();
}


Game * Game::init()
{
    if(game_instance == nullptr)
        game_instance = new Game();

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
    ResourceManager::destroy();
    delete alarm;
    delete boss_music;
    delete main_music;
    delete gamepad;
    delete bg;
    delete level;
    delete score;
    delete game_item;
    delete hud;
    delete player1;
}


int Game::getXlim()
{
    return game_Xlimit;
}
int Game::getYlim()
{
    return game_Ylimit;
}

void Game::createPlayer(unsigned int hp, unsigned int att, unsigned int sh,
                        unsigned int critic,
                        SDL_Texture *image, LX_Chunk *audio,
                        int x, int y, int w, int h,float vx, float vy)
{
    SDL_Rect new_pos = {x,y,w,h};
    LX_Vector2D new_speed(vx,vy);

    delete hud;
    delete player1;
    player1 = new Player(hp,att,sh,critic,image,audio,
                         new_pos,new_speed,game_Xlimit,game_Ylimit);
    hud = new HUD(*player1);
    player1->setHUD(hud);
}

// Load the important ressources
void Game::loadRessources(void)
{
    Bomb::createExplosionBuffer();
    Rocket::createParticlesRessources();
    Item::createItemRessources();
}

// Free all ressources
void Game::freeRessources(void)
{
    Item::destroyItemRessources();
    Rocket::destroyParticlesRessources();
    Bomb::destroyExplosionBuffer();
}


bool Game::loadLevel(const unsigned int lvl)
{
    unsigned int hp, att, def, critic;

    level = new Level(lvl);
    end_of_level = false;

    // The player's skills
    hp = 100;
    att = 20;
    def = 12;
    critic = 3;

    att = Rank::attackPlayerUp(att);
    const char * tmp = TX_Asset::getInstance()->getLevelMusic(lvl);

    if(tmp == nullptr)
    {
#ifdef DEBUG_TX
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,"Cannot load %s\n",tmp);
#endif
        delete level;
        return false;
    }

    if(level->isLoaded())
    {
        setBackground();
        loadRessources();

        main_music = loadMusic(std::string(tmp));
        alarm = loadSample("audio/alarm.wav");
        SDL_Texture *player_sprite = resources->getPlayerResource();

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
    delete alarm;
    delete boss_music;
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
    GAME_STATUS game_status;
    bool done = false;
    Uint32 prev_time;   // The time for the framerate regulation
    Uint32 ticks;

    main_music->volume(MIX_MAX_VOLUME - 32);
    //main_music->play();
    allocateChannels(64);
    const unsigned long nb_enemies = level->numberOfEnemies();

    LX_Device::mouseCursorDisplay(LX_MOUSE_HIDE);
#ifdef DEBUG_TX
    std::cout << "Number of enemies : " << nb_enemies << std::endl;
#endif
    /// @todo Integrate it in the LunatiX Engine
    {
        LX_Window *win = LX_WindowManager::getInstance()->getWindow(0);
        SDL_SetRenderDrawBlendMode(win->getRenderer(),SDL_BLENDMODE_BLEND);
    }

    while(!done && !end_of_level)
    {
        if((done = input()) == true)
            continue;

        createItem();

        // Logic
        physics();
        status();
        clean();
        display();
        while(generateEnemy());

#ifdef DEBUG_TX
        cycle();
#endif
        Framerate::regulate();
        Framerate::cycle();
        Framerate::frame();
    }

    // A this point, the game is over
    generateResult(info);
    info.max_nb_enemies = static_cast<unsigned int>(nb_enemies);

    LX_Device::mouseCursorDisplay(LX_MOUSE_SHOW);
    main_music->stop();
    clearVectors();
    allocateChannels(0);

    // Status of the game
    if(end_of_level)
        game_status = GAME_FINISH;
    else
        game_status = GAME_QUIT;

    return game_status;
}


GAME_STATUS Game::play(ResultInfo& info,unsigned int lvl)
{
    GAME_STATUS game_state = GAME_STATUS::GAME_QUIT;
    fade_out_counter = 0;

    if(loadLevel(lvl))
    {
        score = new Score(0);
        begin = SDL_GetTicks();
        game_state = loop(info);
        endLevel();
    }
#ifdef DEBUG_TX
    else
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,"Cannot load the level #%ud",lvl);
#endif // DEBUG_TX
    return game_state;
}

#ifdef DEBUG_TX
void Game::cycle(void)
{
    static long previous_time = 0;

    if(static_cast<long>(SDL_GetTicks() - previous_time) >= 1000)
    {
        previous_time = SDL_GetTicks();
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Enemies: %u\n",enemies.size());
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Enemy missiles: %u\n",
                         enemies_missiles.size());
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Player's missiles: %u\n",
                         player_missiles.size());
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Death(s): %d\n\n",
                         player1->nb_death());
    }
}
#endif

void Game::generateResult(ResultInfo& info)
{
    // Create the result and copy it
    ResultInfo res = {level->getLevelNum(),player1->nb_death(),
                      score->getCurrentScore(),
                      score->getKilledEnemies(),0
                     };
    info = res;
}

bool Game::input(void)
{
    bool is_done = false;
    PlayerInput::input(*player1,is_done);
    return is_done;
}

void Game::acceptEnemyMissile(Missile *m)
{
    enemies_missiles.push_back(m);
}

void Game::acceptEnemy(Enemy *e)
{
    enemies.push_back(e);
}

void Game::acceptPlayerMissile(Missile *m)
{
    player_missiles.push_back(m);
}

void Game::acceptItem(Item * y)
{
    items.push_back(y);
}

void Game::acceptPlayerInput()
{
    takeScreenshot();
}

void Game::setBackground(int lvl)
{
    if(lvl < 3)
        bg = new Background("image/level00-bgd.png",0,0,1600,game_Ylimit,-3);
}

// Create a new item only if it does not exist
void Game::createItem()
{
    if(game_item == nullptr)
        game_item = new Item();
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

// Clean all objects
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

    for(auto m_it = enemies_missiles.begin();
            m_it != enemies_missiles.end(); m_it++)
    {
        items.push_back(new Item((*m_it)->getX(),
                                 (*m_it)->getY()));
    }
}

void Game::takeScreenshot()
{
    static int id_screen = 1;
    std::ostringstream ss;

    ss << "screen-" << id_screen++ << ".png";
    LX_Window *w = LX_Graphics::getWindowManager()->getWindow(window_id);

    if(w != nullptr)
        w->screenshot(ss.str());
}

void Game::physics(void)
{
    if(player1->isDead() == false)
    {
        if(collisionCircleRect(*player1->getHitbox(), game_item->box()))
        {
            player1->takeBonus(game_item->getPowerUp());
            game_item->die();
        }

        for(auto it = items.begin(); it != items.end(); it++)
        {
            if(collisionCircleRect(*player1->getHitbox(), (*it)->box()))
            {
                player1->takeBonus((*it)->getPowerUp());
                (*it)->die();
            }
        }
    }

    for(auto en_it = enemies.begin(); en_it != enemies.end(); en_it++)
    {
        // enemy/player collision
        if(!player1->isDead())
            (*en_it)->collision(player1);

        if((*en_it)->isDead())
            continue;

        // enemy/missile collision
        for(auto pm_it = player_missiles.begin();
                pm_it != player_missiles.end(); pm_it++)
        {
            if((*pm_it) == nullptr)
                continue;

            (*en_it)->collision(*pm_it);
        }
    }

    if(!player1->isDead())
    {
        for(auto m_it = enemies_missiles.begin();
                m_it != enemies_missiles.end(); m_it++)
        {
            // enemy missiles/player collision
            player1->collision(*m_it);
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
        game_item->move();

    // Move the items
    for(auto it = items.begin(); it != items.end(); it++)
    {
        if((*it)->getX() > (-((*it)->getWidth()) - 1))
            (*it)->move();
        else
            (*it)->die();
    }

    // Move the player
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

    // Move the missiles of the player
    for(auto pm_it = player_missiles.begin();
            pm_it != player_missiles.end(); pm_it++)
    {
        if((*pm_it) == nullptr)
            continue;

        if((*pm_it)->getX() >= game_Xlimit)
            (*pm_it)->die();
        else
            (*pm_it)->move();
    }

    // Move the missiles of enemies
    for(unsigned int i = 0; i < enemies_missiles.size(); i++)
    {
        if(enemies_missiles[i] == nullptr)
            continue;

        /*  If an enemy missile is not in the visible part of the screen
            -> it dies.*/
        int x = enemies_missiles[i]->getX();
        int y = enemies_missiles[i]->getY();
        int w = enemies_missiles[i]->getWidth();
        int h = enemies_missiles[i]->getHeight();

        if(x <= (-w -1) || x >= game_Xlimit
                || y <= (-h -1) || y >= game_Ylimit)
            enemies_missiles[i]->die();
        else
            enemies_missiles[i]->move();
    }

    // The enemy strategy
    for(auto en_it = enemies.begin(); en_it != enemies.end(); en_it++)
    {
        if((*en_it)->getX() <= (-((*en_it)->getWidth()) -1))
            (*en_it)->die();
        else
            (*en_it)->strategy();
    }
}

void Game::clean(void)
{
    destroyItem();

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
        if(player_missiles[i] == nullptr || player_missiles[i]->isDead())
        {
            delete player_missiles[i];
            player_missiles.erase(player_missiles.begin() + i);
            i--;
        }
    }

    // Missiles of enemies
    for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size(); k++)
    {
        if(enemies_missiles[k] == nullptr || enemies_missiles[k]->isDead())
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
                int sc = static_cast<int>(enemies[j]->getMaxHP() +
                                          enemies[j]->getATT() +
                                          enemies[j]->getDEF());
                score->notify(sc,true);
            }

            delete enemies[j];
            enemies.erase(enemies.begin() + j);
            j--;
        }
    }
}

// In loop
void Game::display(void)
{
    LX_Window *current_window = LX_WindowManager::getInstance()->getWindow(0);

#ifdef DEBUG_TX
    if(current_window == nullptr)
    {
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,
                         "Cannot display anything, the window is invalid");
        return;
    }
#endif
    scrollAndDisplayBackground();
    displayPlayerMissiles();
    displayItems();
    displayEnemies();
    displayEnemyMissiles();

    // display the player
    if(!player1->isDead())
        current_window->putTexture(player1->getTexture(),nullptr,
                                   player1->getPos());

    // Display the item
    if(game_item != nullptr && game_item->getTexture() != nullptr)
        current_window->putTexture(game_item->getTexture(),nullptr,
                                   game_item->getPos());
    screenFadeOut();

    // Display text
    score->display();
    player1->updateHUD();
    current_window->update();
}

void Game::scrollAndDisplayBackground(void)
{
    LX_Window *current_window = LX_WindowManager::getInstance()->getWindow(0);
    bg->scroll();   // Scroll the brackground
    SDL_Rect tmp = {bg->getX_scroll(),bg->getY_scroll(),bg->getW(),bg->getH()};
    SDL_Rect tmp2 = {(tmp.x + tmp.w),0,tmp.w,tmp.h};

    current_window->clearWindow();
    current_window->putTexture(bg->getBackground(),nullptr,&tmp);
    current_window->putTexture(bg->getBackground(),nullptr,&tmp2);
}

void Game::displayPlayerMissiles(void)
{
    LX_Window *current_window = LX_WindowManager::getInstance()->getWindow(0);
    for(auto pm_it = player_missiles.cbegin();
            pm_it != player_missiles.cend(); pm_it++)
    {
        (*pm_it)->displayAdditionnalData();
        SDL_Rect *area = (*pm_it)->getAreaToDisplay();
        current_window->putTexture((*pm_it)->getTexture(),area,
                                   (*pm_it)->getPos());
    }
}

void Game::displayItems(void)
{
    LX_Window *current_window = LX_WindowManager::getInstance()->getWindow(0);
    for(auto it = items.cbegin(); it != items.cend(); it++)
    {
        if((*it) != nullptr && (*it)->getTexture() != nullptr)
            current_window->putTexture((*it)->getTexture(),nullptr,
                                       (*it)->getPos());
    }
}

void Game::displayEnemies(void)
{
    LX_Window *current_window = LX_WindowManager::getInstance()->getWindow(0);
    for(auto en_it = enemies.cbegin(); en_it != enemies.cend(); en_it++)
    {
        if((*en_it)->getX() < game_Xlimit)
        {
            SDL_Rect *area = (*en_it)->getAreaToDisplay();
            current_window->putTexture((*en_it)->getTexture(),area,
                                       (*en_it)->getPos());
        }
    }
}

void Game::displayEnemyMissiles(void)
{
    LX_Window *current_window = LX_WindowManager::getInstance()->getWindow(0);
    for(auto m_it = enemies_missiles.cbegin();
            m_it != enemies_missiles.cend(); m_it++)
    {
        (*m_it)->displayAdditionnalData();
        SDL_Rect *area = (*m_it)->getAreaToDisplay();
        current_window->putTexture((*m_it)->getTexture(),area,
                                   (*m_it)->getPos());
    }
}

void Game::screenFadeOut()
{
    LX_Window * current_window = LX_WindowManager::getInstance()->getWindow(0);

    if(enemies.size() == 0 && level->numberOfEnemies() == 0)
    {
        if(fade_out_counter < 255)
        {
            SDL_SetRenderDrawColor(current_window->getRenderer(),0,0,0,
                                   fade_out_counter);
            fade_out_counter++;
            SDL_RenderFillRect(current_window->getRenderer(),nullptr);
        }
        else
        {
            fade_out_counter = 0;
            end_of_level = true;
            current_window->clearWindow();
        }
    }
}


bool Game::generateEnemy(void)
{
    EnemyInfo data;

    if(level->statEnemyInfo(data))
    {
        if((SDL_GetTicks() - begin) > data.t)
        {
            level->popData();

            if(data._alarm)
                alarm->play();
            else
                enemies.push_back(data.e);

            if(data.boss)
            {
                boss_music = loadMusic("audio/boss02.ogg");
                haltChannel(-1);
                //boss_music->play(-1);
            }

            return true;
        }
    }
    return false;
}


void Game::stopBossMusic(void)
{
    if(boss_music != nullptr)
        boss_music->stop();
}

Score *Game::getScore() const
{
    return score;
}
