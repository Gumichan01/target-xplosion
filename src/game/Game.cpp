

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2014  Luxon Jean-Pierre
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

#include "Game.hpp"
#include "../entities/Basic_Enemy.hpp"
#include "../entities/Item.hpp"

#include "../engine/LX_Sound.hpp"
#include "../engine/LX_Music.hpp"
#include "../engine/LX_Device.hpp"


int Game::game_Xlimit = 0;
int Game::game_Ylimit = 0;

static Game *game_instance = NULL;


Game::Game()
{
    int id;
    LX_Window *win = new LX_Window(LX_WINDOW_RENDERING);
    win->setTitle("Target Xplosion v0.2.5");

    game_Xlimit = win->getWidth();
    game_Ylimit = win->getHeight();

    id = LX_Graphics::LX_WindowManager::getInstance()->addWindow(win);

    if(id == -1)
    {
#ifdef DEBUG_TX
        std::cerr << "Cannot add the window into the manager" << std::endl;
#endif
        windowID = (1 << 20);   // Give an invalid value to windowID
    }
    else
        windowID = id;

    LX_Mixer::channelVolume(-1,LX_Mixer::channelVolume(-1,-1)/2);

    player1 = NULL;
    game_item = NULL;
    bg = NULL;
    score = new Score(0);
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



bool Game::play()
{
    bool go = true;

    double begin_game_Time = SDL_GetTicks();  // the time at the beginning of the game
    double ref_time = begin_game_Time;       // The reference time for the framerate
    double prev_time = begin_game_Time;     // The previous time for the framerate regulation
    //double final_game_Time = 0;            // The time at the end of the game
    //double end_time = 0;

    unsigned int compt = 0;
    double framerate = SECOND/FRAMERATE;      // The time used to display an image

    SDL_Texture *player_sprite = LX_Graphics::loadTextureFromFile("image/player.png",windowID);
    LX_Chunk *player_sample = LX_Mixer::loadSample("sound/Longshot.wav");

    createPlayer(100,20,5,1,player_sprite,player_sample,(game_Xlimit/2)-(PLAYER_WIDTH/2),(game_Ylimit/2)-(PLAYER_HEIGHT/2),64,64,0,0);

    LX_Music *mainMusic = LX_Mixer::loadMusic("sound/Afterburner.ogg");
    mainMusic->play();

    player_missiles.reserve(RESERVE);
    enemies_missiles.reserve(RESERVE);
    enemies.reserve(RESERVE);

    enemies.push_back(new Basic_Enemy(20,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit,100,47,47,-4,0));
    enemies.push_back(new Basic_Enemy(20,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit + 100,200,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(20,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit + 200,300,47,47,-4,0));
    enemies.push_back(new Basic_Enemy(20,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit + 300,400,47,47,-3,0));

    enemies.push_back(new Basic_Enemy(20,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit *2,100,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(30,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit *2,300,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(30,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit *2 +100,600,47,47,-4,0));
    enemies.push_back(new Basic_Enemy(30,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit *2 +100,650,47,47,-4,0));

    enemies.push_back(new Basic_Enemy(50,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit *3,100,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(50,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit *3 +100,200,47,47,-4,0));
    enemies.push_back(new Basic_Enemy(50,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit *3,300,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(50,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit *3 +100,500,47,47,-4,0));

    enemies.push_back(new Basic_Enemy(20,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit *4 +100,100,47,47,-4,0));
    enemies.push_back(new Basic_Enemy(20,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit *4,200,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(20,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit *4 +100,300,47,47,-4,0));
    enemies.push_back(new Basic_Enemy(20,10,5,LX_Graphics::loadTextureFromFile("image/ennemi.png",0),NULL,game_Xlimit *4,400,47,47,-3,0));

    //enemies.push_back(new Basic_Enemy(500,11,9,LX_Graphics::loadTexture("image/ennemi.png",0),NULL,game_Xlimit *3.5,200,550,370,-1,0));

    setBackground();
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

        // FPS
        compt++;
        double curr_time = SDL_GetTicks();

        //Framerate regulation
        if( (curr_time - prev_time) < framerate)
        {
            SDL_Delay(framerate - (curr_time - prev_time));
        }

        prev_time = curr_time;

        //Calculate the framerate
        if( (curr_time - ref_time) >= SECOND )
        {
            ref_time = curr_time;
#ifdef DEBUG_TX
            std::cout << "FPS : " << compt << std::endl;
#endif
            compt = 0;
        }
    }

    SDL_ShowCursor(SDL_ENABLE);
    delete bg;
    mainMusic->stop();
    delete mainMusic;

    clean_up();

    return true;
}



bool Game::input()
{
    SDL_Event event;
    bool go_on = true;
    static const Uint8 *keys = SDL_GetKeyboardState(NULL);

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

            default:
                break;

        }

        if(keys[SDL_SCANCODE_UP] )
            player1->set_Yvel(-PLAYER_SPEED);

        if(keys[SDL_SCANCODE_DOWN])
            player1->set_Yvel(PLAYER_SPEED);

        if(keys[SDL_SCANCODE_LEFT])
            player1->set_Xvel(-PLAYER_SPEED);

        if(keys[SDL_SCANCODE_RIGHT])
            player1->set_Xvel(PLAYER_SPEED);

    }

    return go_on;

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
    bg = new Background("image/background.png",0,0,1600,1200,-2);
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
    if(game_item->getX() <= 0)
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

        if(enemies_missiles[k]->getX() <= 0 || enemies_missiles[k]->getX() >= game_Xlimit)
            enemies_missiles[k]->die();
        else
            enemies_missiles[k]->move();

    }

    // The enemies strategy
    for(std::vector<Enemy *>::size_type j = 0; j != enemies.size() ; j++)
    {
        if(enemies[j]->getX() <= 0)
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
        if( player_missiles[i] == NULL || player_missiles[i]->getX() > game_Xlimit || player_missiles[i]->isDead() )
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
        err = currentWindow->putTexture(player_missiles[i]->getTexture(),NULL, player_missiles[i]->getPos());

        if(err == false)
        {
#ifdef DEBUG_TX
            std::cerr << "Fail player missile no " << i << std::endl;
#endif
        }
    }

    // display enemies' missiles
    for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size(); k++)
    {
        err = currentWindow->putTexture(enemies_missiles[k]->getTexture(),NULL, enemies_missiles[k]->getPos());

        if(err == false)
        {
#ifdef DEBUG_TX
            std::cerr << "Fail enemy missile no " << k << std::endl;
#endif
        }
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
        err = currentWindow->putTexture(enemies[j]->getTexture(),NULL, enemies[j]->getPos());

        if(err == false)
        {
#ifdef DEBUG_TX
            std::cerr << "Fail enemy no " << j << std::endl;
#endif
        }
    }

    // Display text
    score->display();
    player1->updateHUD();
    currentWindow->updateRenderer();
}


