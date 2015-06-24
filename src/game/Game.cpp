

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

// Including all specialized engines
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_TrueTypeFont.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Mixer.hpp>
#include <LunatiX/LX_Sound.hpp>
#include <LunatiX/LX_Music.hpp>
#include <LunatiX/LX_Chunk.hpp>
#include <LunatiX/LX_Device.hpp>

#include "Game.hpp"
#include "../entities/Basic_Enemy.hpp"
#include "../entities/boss/Boss00.hpp"
#include "../entities/BulletZ.hpp"
#include "../entities/Item.hpp"
#include "../entities/Bomb.hpp"
#include "../level/Level.hpp"
#include "../level/EnemyData.hpp"
#include "../xml/XMLReader.hpp"


int Game::game_Xlimit = 0;
int Game::game_Ylimit = 0;

Score *Game::score = NULL;
static Game *game_instance = NULL;


Game::Game()
{
    windowID = 0;
    game_Xlimit = LX_Graphics::LX_WindowManager::getInstance()->getWindow(windowID)->getWidth();
    game_Ylimit = LX_Graphics::LX_WindowManager::getInstance()->getWindow(windowID)->getHeight();

    LX_Mixer::channelVolume(-1,LX_Mixer::channelVolume(-1,-1)/2);

    player1 = NULL;
    game_item = NULL;
    bg = NULL;
    score = new Score(0);
    joystick = NULL;

    if(LX_Device::numberOfDevices() > 0)
    {
        joystick = SDL_JoystickOpen(0);

        if(joystick == NULL);
        {
            std::cerr << "Cannot load the joystick: " << SDL_GetError() << std::endl;
        }
    }
}



Game * Game::init()
{
    if(game_instance == NULL)
    {
        game_instance = new Game();
    }

    return game_instance;
}


Game * Game::getInstance()
{
    if(game_instance == NULL)
    {
        game_instance = Game::init();
    }

    return game_instance;
}


void Game::destroy()
{
    delete game_instance;
    game_instance = NULL;
}


Game::~Game()
{
    SDL_JoystickClose(joystick);
    delete score;
    delete game_item;
    delete player1;
}


void Game::createPlayer(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic,
                        SDL_Texture *image, LX_Chunk *audio,
                        int x, int y, int w, int h,int dX, int dY)
{
    SDL_Rect new_pos = {(Sint16) x, (Sint16)y,(Uint16) w, (Uint16) h};
    LX_Vector2D new_speed = {dX,dY};

    delete player1;
    player1 = new Player(hp, att, sh, critic,image, audio,&new_pos,&new_speed,game_Xlimit,game_Ylimit);
}

bool Game::loadLevel(const unsigned int lvl)
{
    TX_Asset *tx = TX_Asset::getInstance();
    std::string str = tx->playerFile();
    level = new Level(lvl);

    const std::string strErr = "ERROR";
    const std::string strMusic = tx->loadLevelMusic(lvl);

    if(!strMusic.compare(strErr))
    {
        return false;
    }

    if(level->isLoaded())
    {
        setBackground();
        Bomb::createExplosionBuffer();
        mainMusic = LX_Mixer::loadMusic(strMusic.c_str());
        alarm = LX_Mixer::loadSample("audio/alarm.wav");
        SDL_Texture *player_sprite = LX_Graphics::loadTextureFromFile(str.c_str(),windowID);
        createPlayer(100,20,12,1,player_sprite,NULL,
                     (game_Xlimit/2)-(PLAYER_WIDTH/2),
                     (game_Ylimit/2)-(PLAYER_HEIGHT/2),PLAYER_WIDTH,PLAYER_HEIGHT,0,0);
        return true;
    }
    return false;
}

void Game::endLevel(void)
{
    delete alarm;
    delete mainMusic;
    delete bg;
    delete level;
    bg = NULL;
    level = NULL;
    mainMusic = NULL;
    Bomb::destroyExplosionBuffer();
}

void Game::loop(void)
{
    bool go = true;
    double ref_time = SDL_GetTicks();       // The reference time for the framerate
    double prev_time = SDL_GetTicks();     // The previous time for the framerate regulation
    double curr_time;

    unsigned int compt = 0;
    double framerate = SECOND/FRAMERATE;      // The time used to display an image

    mainMusic->volume(MIX_MAX_VOLUME - 32);
    //mainMusic->play();
    LX_Mixer::allocateChannels(64);

    player_missiles.reserve(RESERVE);
    enemies_missiles.reserve(RESERVE);
    enemies.reserve(RESERVE);

    LX_Device::mouseCursorDisplay(LX_MOUSE_HIDE);

    while(go)
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

        // FPS
        compt++;
        curr_time = SDL_GetTicks();

        //Framerate regulation
        if( static_cast<int>(curr_time - prev_time) <  static_cast<int>(framerate))
        {
            SDL_Delay(framerate - (curr_time - prev_time));
        }

        prev_time = curr_time;

        //Calculate the framerate
        if( (SDL_GetTicks() - ref_time) >= SECOND )
        {
            ref_time = SDL_GetTicks();
#ifdef DEBUG_TX
            std::cout << "FPS : " << compt << std::endl;
            std::cout << "Enemies : " << enemies.size()
                      << "; enemy missiles : " << enemies_missiles.size()
                      << "; player's missiles : " << player_missiles.size() << std::endl;
#endif
            compt = 0;
        }
    }

    SDL_ShowCursor(SDL_ENABLE);
    mainMusic->stop();
    clean_up();
    LX_Mixer::allocateChannels(0);
}


void Game::play(unsigned int lvl)
{
    if(loadLevel(lvl) == true)
    {
        loop();
        endLevel();
    }
    else
        std::cerr << "Cannot load the level" << std::endl;
}

bool Game::input(void)
{
    SDL_Event event;
    bool go_on = true;
    static const Uint8 *keys = SDL_GetKeyboardState(NULL);
    static char freq = 1;
    static char continuous_shoot = 0;

    if(keys[SDL_SCANCODE_UP])
        player1->set_Yvel(-PLAYER_SPEED);

    if(keys[SDL_SCANCODE_DOWN])
        player1->set_Yvel(PLAYER_SPEED);

    if(keys[SDL_SCANCODE_LEFT])
        player1->set_Xvel(-PLAYER_SPEED);

    if(keys[SDL_SCANCODE_RIGHT])
        player1->set_Xvel(PLAYER_SPEED);

    if(keys[SDL_SCANCODE_SPACE] || continuous_shoot == 1)
    {
        if(freq%6 == 0)
        {
            if(!player1->isDead())
            {
                player1->fire(MISSILE_TYPE::BASIC_MISSILE_TYPE);
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
                        player1->set_Xvel(0);
                        break;

                    case SDLK_LEFT :
                        player1->set_Xvel(0);
                        break;

                    case SDLK_UP :
                        player1->set_Yvel(0);
                        break;

                    case SDLK_DOWN :
                        player1->set_Yvel(0);
                        break;

                    case SDLK_SPACE :
                        player1->fire(MISSILE_TYPE::BASIC_MISSILE_TYPE);
                        break;

                    case SDLK_g :
                        player1->fire(MISSILE_TYPE::ROCKET_TYPE);
                        break;

                    case SDLK_b :
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
                player1->set_Xvel(-PLAYER_SPEED);
            }
            else if(event->jaxis.value > JOYSTICK_HIGH_ZONE)
            {
                player1->set_Xvel(PLAYER_SPEED);
            }
            else if(event->jaxis.value < -JOYSTICK_DEAD_ZONE)
            {
                player1->set_Xvel(-(PLAYER_SPEED/2));
            }
            else if(event->jaxis.value > JOYSTICK_DEAD_ZONE)
            {
                player1->set_Xvel(PLAYER_SPEED/2);
            }
            else
            {
                player1->set_Xvel(0);
            }
        }
        else if(event->jaxis.axis == 1) // Y axis
        {
            if(event->jaxis.value < -JOYSTICK_HIGH_ZONE)
            {
                player1->set_Yvel(-PLAYER_SPEED);
            }
            else if(event->jaxis.value > JOYSTICK_HIGH_ZONE)
            {
                player1->set_Yvel(PLAYER_SPEED);
            }
            else if(event->jaxis.value < -JOYSTICK_DEAD_ZONE)
            {
                player1->set_Yvel(-(PLAYER_SPEED/2));
            }
            else if(event->jaxis.value > JOYSTICK_DEAD_ZONE)
            {
                player1->set_Yvel(PLAYER_SPEED/2);
            }
            else
            {
                player1->set_Yvel(0);
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


void Game::setBackground()
{
    bg = new Background("image/background.png",0,0,game_Xlimit,game_Ylimit,-3);
}

// Create a new item only if it does not exist
void Game::createItem()
{
    if(game_item == NULL)
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
        game_item = NULL;
    }
}


void Game::clean_up(void)
{
    // Player's missiles
    for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size(); i++)
    {
        if(player_missiles[i] != NULL)
        {
            delete player_missiles[i];
        }

        player_missiles.erase(player_missiles.begin() + i);
        i--;
    }

    // Enemies missiles
    for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size(); k++)
    {
        if(enemies_missiles[k] != NULL)
        {
            delete enemies_missiles[k];
        }

        enemies_missiles.erase(enemies_missiles.begin() + k);
        k--;
    }

    // Enemies
    for(std::vector<Enemy *>::size_type j = 0; j != enemies.size(); j++)
    {
        if(enemies[j] != NULL)
        {
            delete enemies[j];
        }

        enemies.erase(enemies.begin() + j);
        j--;
    }
}


void Game::physics(void)
{
    if(player1->isDead() == false)
    {
        if(LX_Physics::collisionCircleRect(player1->get_hitbox(), game_item->box()))
        {
            player1->takeBonus(game_item->getPowerUp());
            game_item->die();
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
            if(player_missiles[i] == NULL)
            {
                continue;
            }

            enemies[j]->collision(player_missiles[i]);
        }
    }

    if(!player1->isDead())
    {
        for(std::vector<Missile *>::size_type k =0; k!= enemies_missiles.size(); k++)
        {
            // enemies missiles/player collision
            player1->collision(enemies_missiles[k]);
        }
    }
}


void Game::status(void)
{
    if(game_item->getX() <= (-(game_item->getWidth()) - 1))
    {
        game_item->die();
    }
    else if(!game_item->isDead())
    {
        game_item->move();  // Item movement
    }

    if(!player1->isDead())
    {
        if(player1->isLaser_activated())
            player1->fire(MISSILE_TYPE::LASER_TYPE);

        player1->move();
    }

    // The player's missiles movement
    for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size(); i++)
    {
        if(player_missiles[i] == NULL)
            continue;

        if(player_missiles[i]->getX() >= game_Xlimit)
            player_missiles[i]->die();
        else
            player_missiles[i]->move();
    }

    // The enemies' missiles movement
    for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size(); k++)
    {
        if(enemies_missiles[k] == NULL)
            continue;

        if(enemies_missiles[k]->getX() <= (-(enemies_missiles[k]->getWidth()) -1)
                || enemies_missiles[k]->getX() >= game_Xlimit)
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

    // Missiles of the player
    for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size() ; i++)
    {
        if( player_missiles[i] == NULL || player_missiles[i]->isDead() )
        {
            delete player_missiles[i];
            player_missiles.erase(player_missiles.begin() + i);
            i--;
        }
    }

    // Missiles of enemies
    for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size(); k++)
    {
        if( enemies_missiles[k] == NULL || enemies_missiles[k]->isDead() )
        {
            delete enemies_missiles[k];
            enemies_missiles.erase(enemies_missiles.begin() + k);
            k--;
        }
    }

    // Enemies
    for(std::vector<Enemy *>::size_type j = 0; j != enemies.size(); j++)
    {
        if(enemies[j]->killed())
        {
            score->notify(enemies[j]->getMAX_HP() + enemies[j]->getATT() + enemies[j]->getDEF());
        }

        if(enemies[j]->isDead())
        {
            delete enemies[j];
            enemies.erase(enemies.begin() + j);
            j--;
        }
    }
}


void Game::display(void)
{
    bool err;
    LX_Window *currentWindow = LX_Graphics::LX_WindowManager::getInstance()->getWindow(0);

    if(currentWindow == NULL)
    {
#ifdef DEBUG_TX
        std::cerr << "Cannot display anything " << std::endl;
#endif
        return;
    }

    currentWindow->clearRenderer();

    bg->scroll();   //scroll the brackground
    SDL_Rect tmp = {bg->getX_scroll(),bg->getY_scroll(),bg->getW(),bg->getH()};
    SDL_Rect tmp2 = {(Sint16)(tmp.x + tmp.w),0,bg->getW(),bg->getH()};

    currentWindow->putTexture(bg->getBackground(),NULL,&tmp);
    currentWindow->putTexture(bg->getBackground(),NULL,&tmp2);

    if(game_item != NULL)
    {
        currentWindow->putTexture(game_item->getTexture(),NULL,game_item->getPos());
    }

    // display player's missiles
    for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size(); i++)
    {
        player_missiles[i]->displayAdditionnalData();
        SDL_Rect *area = player_missiles[i]->getAreaToDisplay();
        err = currentWindow->putTexture(player_missiles[i]->getTexture(),area, player_missiles[i]->getPos());
    }

    // display enemies' missiles
    for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size(); k++)
    {
        enemies_missiles[k]->displayAdditionnalData();
        SDL_Rect *area = enemies_missiles[k]->getAreaToDisplay();
        currentWindow->putTexture(enemies_missiles[k]->getTexture(),area, enemies_missiles[k]->getPos());
    }

    // display the player
    if(!player1->isDead())
    {
        err = currentWindow->putTexture(player1->getTexture(),NULL, player1->getPos());

        if(err == false)
        {
#ifdef DEBUG_TX
            std::cerr << "Fail player" << std::endl;
#endif
        }
    }

    // display enemies
    for(std::vector<Enemy *>::size_type j = 0; j != enemies.size(); j++)
    {
        if(enemies[j]->getX() < game_Xlimit)
        {
            currentWindow->putTexture(enemies[j]->getTexture(),NULL, enemies[j]->getPos());
        }
    }

    // Display text
    score->display();
    player1->updateHUD();
    currentWindow->updateRenderer();
}


bool Game::generateEnemy(void)
{
    EnemyData data;
    static const double begin = SDL_GetTicks();


    if(level->statEnemyData(&data))
    {
        if((SDL_GetTicks() - begin) > data.time)
        {
            level->popData();

            switch(data.type)
            {
                case 22 :
                {
                    // Boss is comming ( T_T)
                    alarm->play();
                }
                break;

                case 21 :
                {
                    enemies.push_back(new Boss00(data.hp,data.att,data.sh,
                                  LX_Graphics::loadTextureFromFile("image/boss00.png",0),
                                  NULL,game_Xlimit + 1,
                                  data.y,data.w,data.h,-4,0));
                }
                break;

                case 100 :
                {
                    enemies.push_back(new BulletZ(data.hp,data.att,data.sh,
                                                      LX_Graphics::loadTextureFromFile("image/bulletZ.png",0),
                                                      NULL,game_Xlimit + 1,
                                                      data.y,data.w,data.h,-2,0));
                }
                break;

                case 101 :
                {
                    enemies.push_back(new Basic_Enemy(data.hp,data.att,data.sh,
                                                      LX_Graphics::loadTextureFromFile("image/enemy.png",0),
                                                      NULL,game_Xlimit + 1,
                                                      data.y,data.w,data.h,-5,0));
                }
                break;

                case 102 :
                {
                    enemies.push_back(new Basic_Enemy(data.hp,data.att,data.sh,
                                                      LX_Graphics::loadTextureFromFile("image/watcher.png",0),
                                                      NULL,game_Xlimit + 1,
                                                      data.y,data.w,data.h,-4,0));
                }
                break;

                default: break;
            }
            return true;
        }
    }
    return false;
}


