

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
*	@version 0.1
*	@date July 23th, 2014
*
*/

#include "../entities/Basic_Enemy.h"
#include "../entities/Item.h"

#include "Game.h"

#include "../engine/LX_Sound.h"
#include "../engine/LX_Music.h"

int Game::game_Xlimit = 0;
int Game::game_Ylimit = 0;

static Game *game_instance = NULL;


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


void Game::createPlayer(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic,
                                SDL_Texture *image, LX_Chunk *audio,
                                    int x, int y, int w, int h,int dX, int dY)
{
    delete player1;     //clean the ancient player
    // The cast is necessary because of the SDL_Rect fields type
    // check the SDL_Rect documentation
    SDL_Rect new_pos = {(Sint16) x, (Sint16)y,(Uint16) w, (Uint16) h};
    Speed new_speed = {dX,dY};

    player1 = new Player(hp, att, sh, critic,image, audio,&new_pos,&new_speed,game_Xlimit,game_Ylimit);
}



bool Game::play()
{

    bool err = true;
    bool go = true;

    double begin_game_Time = SDL_GetTicks();  // the time at the beginning of the game
    double ref_time = begin_game_Time;       // The reference time for the framerate
    double prev_time = begin_game_Time;     // The previous time for the framerate regulation
    //double final_game_Time = 0;            // The time at the end of the game
    //double end_time = 0;

    unsigned int compt = 0;

    double framerate = SECOND/FRAMERATE;      // The time used to display an image


    SDL_Texture *player_sprite = graphics_engine->loadTextureFromFile("image/Deltaplane_64x64_alpha.png");
    LX_Chunk *player_sample = LX_Mixer::loadSample("sound/Longshot.wav");

    createPlayer(100,20,5,1,player_sprite,player_sample,(game_Xlimit/2)-(PLAYER_WIDTH/2),(game_Ylimit/2)-(PLAYER_HEIGHT/2),64,64,0,0);

    LX_Music *mainMusic = LX_Mixer::loadMusic("sound/Afterburner.ogg");
    mainMusic->play();


    player_missiles.reserve(RESERVE);
    enemies_missiles.reserve(RESERVE);
    enemies.reserve(RESERVE);


    enemies.push_back(new Basic_Enemy(20,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit,100,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(20,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit + 100,200,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(20,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit + 200,300,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(20,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit + 300,400,47,47,-3,0));

    enemies.push_back(new Basic_Enemy(20,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit *2,100,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(30,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit *2,300,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(30,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit *2,600,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(30,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit *2,650,47,47,-3,0));

    enemies.push_back(new Basic_Enemy(50,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit *3,100,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(50,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit *3,200,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(50,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit *3,300,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(50,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit *3,500,47,47,-3,0));

    enemies.push_back(new Basic_Enemy(20,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit *4,100,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(20,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit *4,200,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(20,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit *4,300,47,47,-3,0));
    enemies.push_back(new Basic_Enemy(20,10,5,graphics_engine->loadTextureFromFile("image/ennemi.png"),NULL,game_Xlimit *4,400,47,47,-3,0));

    //enemies.push_back(new Basic_Enemy(500,11,9,graphics_engine->loadTexture("image/ennemi.png"),NULL,game_Xlimit *3.5,200,550,370,-1,0));


    setBackground();


    SDL_ShowCursor(SDL_DISABLE);

    while(go)
    {

        //Event first (user)
        if((go = input()) == false )
            continue;

        createItem();   /// create item


         //***********
         // Physics  *
         //***********

        if(player1->isDead() == false)
        {
            if(LX_Physics::collision(player1->get_hitbox(), game_item->box()))
            {
                player1->takeBonus(game_item->getPowerUp());
                game_item->die();
            }
        }


        for(std::vector<Enemy *>::size_type j = 0; j != enemies.size();j++)
        {

            if(!player1->isDead())
            {
                // enemies/player collision
                enemies[j]->collision(player1);
            }

            if(enemies[j]->isDead())
                continue;

            // enemies/missiles collision
            for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size();i++)
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
            for(std::vector<Missile *>::size_type k =0; k!= enemies_missiles.size();k++)
            {
                // enemies missiles/player collision
                player1->collision(enemies_missiles[k]);
            }
        }

        //***********
        // Movement *
        //***********

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
        for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size();i++)
        {

            if(player_missiles[i] == NULL)
            {
                continue;
            }

            if(player_missiles[i]->getX() >= game_Xlimit)
                player_missiles[i]->die();
            else
                player_missiles[i]->move();

        }

        // The enemies' missiles movement
        for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size();k++)
        {

            if(enemies_missiles[k] == NULL)
            {
                continue;
            }

            if(enemies_missiles[k]->getX() <= 0 || enemies_missiles[k]->getX() >= game_Xlimit)
                enemies_missiles[k]->die();
            else
                enemies_missiles[k]->move();

        }

        // The enemies strategy
        for(std::vector<Enemy *>::size_type j = 0; j != enemies.size() ;j++)
        {
            if(enemies[j]->getX() <= 0)
                enemies[j]->die();
            else
                enemies[j]->strategy();
        }


         // ***************************
         //  Clean all dead entities
         // ***************************

         destroyItem();     // Try to destroy a dead item

        // Missiles of the player
        for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size() ;i++)
        {

            if( player_missiles[i] == NULL || player_missiles[i]->getX() > game_Xlimit || player_missiles[i]->isDead() )
            {
                delete player_missiles[i];
                player_missiles.erase(player_missiles.begin() + i);
                i--;
            }

        }

        // Missiles of enemies
        for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size();k++)
        {

            if( enemies_missiles[k] == NULL || enemies_missiles[k]->isDead() )
            {
                delete enemies_missiles[k];
                enemies_missiles.erase(enemies_missiles.begin() + k);
                k--;
            }

        }


        // Enemies
        for(std::vector<Enemy *>::size_type j = 0; j != enemies.size();j++)
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


        //*****************
        // Display result *
        //*****************
        graphics_engine->clearRenderer();

        bg->scroll();   //scroll the brackground
        SDL_Rect tmp = {bg->getX_scroll(),bg->getY_scroll(),bg->getW(),bg->getH()};
        SDL_Rect tmp2 = {(Sint16)(tmp.x + tmp.w),0,bg->getW(),bg->getH()};

        graphics_engine->putTexture(bg->getBackground(),NULL,&tmp);
        graphics_engine->putTexture(bg->getBackground(),NULL,&tmp2);

        if(game_item != NULL)
        {
            graphics_engine->putTexture(game_item->getTexture(),NULL,game_item->getPos());   /// display the Item
        }


        //display player's missiles
        for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size();i++)
        {
            err = graphics_engine->putTexture(player_missiles[i]->getTexture(),NULL, player_missiles[i]->getPos());

            if(err == false)
            {
                std::cerr << "Fail player missile no " << i << std::endl;
            }
        }

        // display enemies' missiles
        for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size();k++)
        {
            err = graphics_engine->putTexture(enemies_missiles[k]->getTexture(),NULL, enemies_missiles[k]->getPos());

            if(err == false)
            {
                std::cerr << "Fail enemy missile no " << k << std::endl;
            }
        }

        // display the player
        if(!player1->isDead())
        {
            err = graphics_engine->putTexture(player1->getTexture(),NULL, player1->getPos());

            if(err == false)
            {
                std::cerr << "Fail player" << std::endl;
            }
        }

        // display enemies
        for(std::vector<Enemy *>::size_type j = 0; j != enemies.size();j++)
        {
            err = graphics_engine->putTexture(enemies[j]->getTexture(),NULL, enemies[j]->getPos());
            if(err == false)
            {
                std::cerr << "Fail enemy no " << j << std::endl;
            }
        }

        // Display text
        score->display();
        player1->updateHUD();

        graphics_engine->updateRenderer();


        //******
        // FPS *
        //******
        compt++;
        double curr_time = SDL_GetTicks();

        //Framerate regulation
        if( (curr_time - prev_time) < framerate)
        {
            std::cout << "TIME : " << (curr_time - prev_time) << " FRAME : " << framerate << std::endl;
            SDL_Delay(framerate - (curr_time - prev_time));
        }

        prev_time = curr_time;

        //Calculating the framerate
        if( (curr_time - ref_time) >= SECOND )
        {
            ref_time = curr_time;
            std::cout << "FPS : " << compt << std::endl;
            compt = 0;
        }
        //SDL_Delay(framerate);
    }

    SDL_ShowCursor(SDL_ENABLE);
    delete bg;
    mainMusic->stop();
    delete mainMusic;

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

            case SDL_QUIT:  go_on = false;
                            break;

            case SDL_KEYDOWN:   switch(event.key.keysym.sym)
                                {
                                    case SDLK_ESCAPE : go_on = false;
                                                       break;

                                    default : break;
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

                        case SDLK_RIGHT : player1->set_Xvel(0);
                                          break;

                        case SDLK_LEFT : player1->set_Xvel(0);
                                         break;

                        case SDLK_UP : player1->set_Yvel(0);
                                       break;

                        case SDLK_DOWN : player1->set_Yvel(0);
                                         break;

                        case SDLK_SPACE : player1->fire(MISSILE_TYPE::BASIC_MISSILE_TYPE);
                                          break;

                        case SDLK_g : player1->fire(MISSILE_TYPE::ROCKET_TYPE);
                                      break;

                        case SDLK_b : player1->fire(MISSILE_TYPE::BOMB_TYPE);
                                      break;

                        default : break;
                    }

            }break;

            default:    break;

        }



        if(keys[SDL_SCANCODE_UP] )
            player1->set_Yvel(-PLAYER_SPEED);

        if(keys[SDL_SCANCODE_DOWN])
            player1->set_Yvel(PLAYER_SPEED);

        if(keys[SDL_SCANCODE_LEFT])
            player1->set_Xvel(-PLAYER_SPEED);

        if(keys[SDL_SCANCODE_RIGHT])
            player1->set_Xvel(PLAYER_SPEED);

        delete [] keys;

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


template <typename T>
void Game::clean_up(std::vector<T> *vect)
{
    size_t j;

    if(vect == NULL)
        return;

    std::cout << "SIZE : " << vect->size() << std::endl;

    for( j = 0; j != vect->size();j++)
    {
        delete vect->at(j);
    }

    vect->clear();
}












