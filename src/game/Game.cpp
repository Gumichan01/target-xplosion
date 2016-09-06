
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

// Game
#include "Game.hpp"
#include "Hud.hpp"
#include "Rank.hpp"
#include "Result.hpp"
#include "Background.hpp"
#include "Scoring.hpp"
#include "PlayerInput.hpp"
#include "Framerate.hpp"

// Entities
#include "../entities/Item.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../entities/Missile.hpp"
#include "../entities/Bomb.hpp"

// Data
#include "../level/Level.hpp"
#include "../resources/EnemyInfo.hpp"
#include "../resources/ResourceManager.hpp"

// Including some header files of the engine
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Audio.hpp>
#include <LunatiX/LX_Device.hpp>
#include <LunatiX/LX_Log.hpp>
#include <LunatiX/LX_Timer.hpp>

#include <SDL2/SDL_events.h>
#include <sstream>


using namespace LX_Win;
using namespace LX_Physics;
using namespace LX_Device;
using namespace Result;

int Game::game_Xlimit = 0;
int Game::game_Ylimit = 0;
uint8_t Game::fade_out_counter = 0;

static Game *game_instance = nullptr;


Game::Game()
    : game_state(GAME_STATUS::GAME_RUNNING),pause_allowed(true), start_point(0),
      end_of_level(false),window_id(0), hud(nullptr), player(nullptr),
      game_item(nullptr), level(nullptr), score(nullptr), bg(nullptr),
      gamepad(nullptr), main_music(nullptr), boss_music(nullptr), alarm(nullptr),
      resources(nullptr)
{
    ResourceManager::init();
    resources = ResourceManager::getInstance();

    LX_Window *g = LX_WindowManager::getInstance()->getWindow(window_id);
    game_Xlimit = g->getWidth();
    game_Ylimit = g->getHeight();
    current_window = g;

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
    delete player;
}


int Game::getXlim()
{
    return game_Xlimit;
}

int Game::getYlim()
{
    return game_Ylimit;
}


void Game::pause(uint32_t& tstart_point, uint32_t& tduration)
{
    tduration = LX_Timer::getTicks() - tstart_point;
}


void Game::resume(uint32_t& tstart_point, uint32_t& tduration)
{
    tstart_point = LX_Timer::getTicks() - tduration;
}


void Game::createPlayer(unsigned int hp, unsigned int att, unsigned int sh,
                        unsigned int critic,
                        LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
                        int x, int y, int w, int h,float vx, float vy)
{
    LX_AABB new_pos = {x,y,w,h};
    LX_Vector2D new_speed(vx,vy);

    delete hud;
    delete player;
    player = new Player(hp,att,sh,critic,image,audio,
                        new_pos,new_speed,game_Xlimit,game_Ylimit);
    hud = new HUD(*player);
    player->setHUD(hud);
}

// Load the important ressources
void Game::loadRessources()
{
    Bomb::loadExplosionBuffer();
    Item::createItemRessources();
}

// Free all ressources
void Game::freeRessources()
{
    Item::destroyItemRessources();
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
    std::string tmp = TX_Asset::getInstance()->getLevelMusic(lvl);

    if(level->isLoaded())
    {
        setBackground(lvl);
        loadRessources();

        main_music = LX_Mixer::loadMusic(tmp);
        alarm = resources->getSound(ALARM_STR_ID);
        LX_Graphics::LX_Sprite *player_sprite = resources->getPlayerResource();

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


void Game::endLevel()
{
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

    LX_Mixer::allocateChannels(CHANNELS);
    LX_Mixer::setOverallVolume(OV_VOLUME);
    LX_Mixer::setFXVolume(FX_VOLUME);
    //main_music->play();

    const unsigned long nb_enemies = level->numberOfEnemies();

    LX_Device::mouseCursorDisplay(LX_MOUSE_HIDE);
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Number of enemies :  %ud");
    current_window->setDrawBlendMode(SDL_BLENDMODE_BLEND);

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

        cycle();
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
    LX_Mixer::allocateChannels(0);

    // Status of the game
    if(end_of_level)
        game_status = GAME_FINISH;
    else
        game_status = GAME_QUIT;

    return game_status;
}


GAME_STATUS Game::play(ResultInfo& info,unsigned int lvl)
{
    pause_allowed = true;
    fade_out_counter = 0;

    if(loadLevel(lvl))
    {
        score = new Score(0);
        start_point = LX_Timer::getTicks();
        game_state = loop(info);
        endLevel();
    }
    else
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "Cannot load the level #%u",lvl);
    return game_state;
}

void Game::pause()
{
    if(pause_allowed)
    {
        game_state = GAME_PAUSE;
        pause(start_point,game_duration);
        player->pause();
        LX_Mixer::pause(-1);
        //main_music->pause();
    }
}

void Game::resume()
{
    game_state = GAME_RUNNING;
    resume(start_point,game_duration);
    player->resume();
    LX_Mixer::resume(-1);
    //main_music->pause();
}


void Game::cycle()
{
    static long previous_time = 0;

    if(static_cast<long>(LX_Timer::getTicks() - previous_time) >= 1000)
    {
        previous_time = LX_Timer::getTicks();
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Enemies: %u\n",enemies.size());
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Enemy missiles: %u\n",
                         enemies_missiles.size());
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Player's missiles: %u\n",
                         player_missiles.size());
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"Death(s): %d\n\n",
                         player->nb_death());
    }
}


void Game::generateResult(ResultInfo& info)
{
    // Create the result and copy it
    info.level = level->getLevelNum();
    info.nb_death = player->nb_death();
    info.score = score->getCurrentScore();
    info.nb_killed_enemies = score->getKilledEnemies();
    info.max_nb_enemies = 0;
}

bool Game::input()
{
    bool is_done = false;
    PlayerInput::input(*player,is_done);
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

void Game::setBackground(unsigned int lvl)
{
    const int SPEED_BG = -3;
    TX_Asset * asset = TX_Asset::getInstance();
    LX_AABB box = {0,0,1600,game_Ylimit};

    switch(lvl)
    {
    case 1 :
        bg = new Background(asset->getLevelBg(lvl),box,SPEED_BG);
        break;
    default:
        bg = new Background(asset->getLevelBg(0),box,SPEED_BG);
        break;
    }
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
void Game::clearVectors()
{
    clearPlayerMissiles();
    clearEnemyMissiles();
    clearEnemies();
    clearItems();
}

void Game::clearPlayerMissiles()
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

void Game::clearEnemyMissiles()
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

void Game::clearEnemies()
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

void Game::clearItems()
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

void Game::screenCancel()
{
    missileToScore();
    clearEnemyMissiles();
}

void Game::missileToScore()
{
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
    current_window->screenshot(ss.str());
}

void Game::physics()
{
    if(player->isDead() == false)
    {
        if(game_item != nullptr)
            player->collision(game_item);

        for(auto it = items.begin(); it != items.end(); it++)
        {
            player->collision(*it);
        }
    }

    for(auto en_it = enemies.begin(); en_it != enemies.end(); en_it++)
    {
        // enemy/player collision
        if(!player->isDead())
            (*en_it)->collision(player);

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

    if(!player->isDead())
    {
        for(auto m_it = enemies_missiles.begin();
                m_it != enemies_missiles.end(); m_it++)
        {
            // enemy missiles/player collision
            player->collision(*m_it);
        }
    }
}

void Game::status()
{
    static uint32_t death_start = 0;
    const uint32_t DELAY_TO_REBORN = 2000;

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
    if(!player->isDead())
    {
        if(player->isLaserActivated())
        {
            player->fire(MISSILE_TYPE::LASER_TYPE);
            screenCancel();
        }

        player->move();
        death_start = LX_Timer::getTicks();   // Moment of possible death
    }
    else
    {
        if((LX_Timer::getTicks() - death_start) > DELAY_TO_REBORN)
            player->reborn();
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

void Game::clean()
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
void Game::display()
{
    current_window->clearWindow();
    scrollAndDisplayBackground();
    displayItems();
    displayPlayerMissiles();
    displayEnemies();
    displayEnemyMissiles();

    // display the player
    if(!player->isDead())
        player->draw();


    // Display the item
    if(game_item != nullptr)
        game_item->draw();

    screenFadeOut();

    // Display text
    score->display();
    player->updateHUD();
    current_window->update();
}

void Game::scrollAndDisplayBackground()
{
    bg->scroll();
    bg->draw();
}

void Game::displayPlayerMissiles()
{
    for(auto pm_it = player_missiles.cbegin();
            pm_it != player_missiles.cend(); pm_it++)
    {
        (*pm_it)->draw();
    }
}

void Game::displayItems()
{
    for(auto it = items.cbegin(); it != items.cend(); it++)
    {
        if((*it) != nullptr)
            (*it)->draw();
    }
}

void Game::displayEnemies()
{
    for(auto en_it = enemies.cbegin(); en_it != enemies.cend(); en_it++)
    {
        if((*en_it)->getX() < game_Xlimit)
            (*en_it)->draw();
    }
}

void Game::displayEnemyMissiles()
{
    for(auto m_it = enemies_missiles.cbegin();
            m_it != enemies_missiles.cend(); m_it++)
    {
        (*m_it)->draw();
    }
}

void Game::screenFadeOut()
{
    SDL_Color color = {0,0,0,fade_out_counter};
    LX_AABB box = {0,0,game_Xlimit,game_Ylimit};

    if(enemies.size() == 0 && level->numberOfEnemies() == 0)
    {
        if(fade_out_counter < 255)
        {
            current_window->setDrawColor(color);
            fade_out_counter++;
            current_window->fillRect(box);
        }
        else
        {
            fade_out_counter = 0;
            end_of_level = true;
            current_window->clearWindow();
        }
    }
}


bool Game::generateEnemy()
{
    const int BOSS01_MUSIC_ID = 7;
    const int BOSS02_MUSIC_ID = 8;
    EnemyInfo data;

    if(level->statEnemyInfo(data))
    {
        if((LX_Timer::getTicks() - start_point) > data.t)
        {
            level->popData();

            if(data._alarm)
            {
                pause_allowed = false;
                alarm->play();
            }
            else
                enemies.push_back(data.e);

            if(data.boss)
            {
                TX_Asset *a = TX_Asset::getInstance();

                if(level->getLevelNum()%2 == 1)
                    boss_music = LX_Mixer::loadMusic(a->getLevelMusic(BOSS01_MUSIC_ID));
                else
                    boss_music = LX_Mixer::loadMusic(a->getLevelMusic(BOSS02_MUSIC_ID));

                LX_Mixer::haltChannel(-1);
                //boss_music->play(-1);
            }

            return true;
        }
    }
    return false;
}


void Game::stopBossMusic()
{
    if(boss_music != nullptr)
        boss_music->stop();
}

Score *Game::getScore() const
{
    return score;
}
