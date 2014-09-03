

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

#include "Game.h"

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


void Game::createPlayer(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic, unsigned int bombs,
                                SDL_Surface *image, Mix_Chunk *audio,
                                    int x, int y, int w, int h,int dX, int dY)
{
    delete player1;     //clean the ancient player
    // The cast is necessary because of the SDL_Rect fields type
    // check the SDL_Rect documentation
    SDL_Rect new_pos = {(Sint16) x, (Sint16)y,(Uint16) w, (Uint16) h};
    Speed new_speed = {dX,dY};

    player1 = new Player(hp, att, sh, critic, bombs,image, audio,&new_pos,&new_speed,game_Xlimit,game_Ylimit);
}



bool Game::play()
{
    SDL_Surface *text = ttf_engine->draw_BlendedText("é jap アカツキアライヴァル");
    SDL_Rect posT = {100,100};

    bool err = true;
    bool go = true;

    double begin_game_Time = SDL_GetTicks();  // the time at the beginning of the game
    double ref_time = begin_game_Time;       // The reference time for the framerate
    double prev_time = begin_game_Time;     // The previous time for the framerate regulation
    //double final_game_Time = 0;            // The time at the end of the game
    //double end_time = 0;

    unsigned int compt = 0;

    double framerate = (SECOND/FRAMERATE);      // The time used to display an image

    SDL_Surface *player_sprite = graphics_engine->load_image("image/Deltaplane_64x64_alpha.png");

    createPlayer(100,20,10,1,0,player_sprite,NULL,(game_Xlimit/2)-(PLAYER_WIDTH/2),(game_Ylimit/2)-(PLAYER_HEIGHT/2),64,64,0,0);

    audio_engine->load_music("sound/Afterburner.ogg");
    audio_engine->play_music();

    player_missiles.reserve(16);
    enemies.reserve(16);


    enemies.push_back(new Enemy(20,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit,100,47,47,-1,0));
    enemies.push_back(new Enemy(20,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit + 100,200,47,47,-1,0));
    enemies.push_back(new Enemy(20,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit + 200,300,47,47,-1,0));
    enemies.push_back(new Enemy(20,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit + 300,400,47,47,-1,0));

    enemies.push_back(new Enemy(20,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *2,100,47,47,-2,0));
    enemies.push_back(new Enemy(30,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *2,300,47,47,-2,0));
    enemies.push_back(new Enemy(30,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *2,200,47,47,-2,0));
    enemies.push_back(new Enemy(30,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *2,500,47,47,-2,0));

    enemies.push_back(new Enemy(50,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *3,100,47,47,-3,0));
    enemies.push_back(new Enemy(50,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *3,200,47,47,-3,0));
    enemies.push_back(new Enemy(50,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *3,300,47,47,-3,0));
    enemies.push_back(new Enemy(50,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *3,500,47,47,-3,0));

    enemies.push_back(new Enemy(20,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *5,100,47,47,-4,0));
    enemies.push_back(new Enemy(20,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *5,300,47,47,-4,0));
    enemies.push_back(new Enemy(20,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *5,200,47,47,-4,0));
    enemies.push_back(new Enemy(20,10,5,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *5,300,47,47,-4,0));

    enemies.push_back(new Enemy(100,10,10,graphics_engine->load_image("image/ennemi.png"),NULL,game_Xlimit *5 + 512,300,47,47,-4,0));

    SDL_ShowCursor(SDL_DISABLE);

    while(go)
    {

        //Event first (user)
        go = input();


         //***********
         // Physics  *
         //***********

        /// @todo collision player/missile

        // collision ennemies/player_misiles & player
        for(std::vector<Enemy *>::size_type j = 0; j != enemies.size();j++)
        {

            if(!player1->isDead())
            {
                //enemies/player
                if(physics_engine->collision( player1->get_hitbox(), enemies[j]->get_hitbox()))
                {
                    enemies[j]->receive_damages(player1->getHP());
                    player1->receive_damages(enemies[j]->getHP());
                }
            }

            // enemies/missiles
            for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size();i++)
            {
                if(player_missiles[i] == NULL)
                {
                    continue;
                }

                if(physics_engine->collision( enemies[j]->get_hitbox(), player_missiles[i]->get_hitbox() ))
                {
                    enemies[j]->receive_damages(player_missiles[i]->put_damages());
                    player_missiles[i]->die();
                }

            }

        }

        // collision player/ennemies missiles
        for(std::vector<Missile *>::size_type k =0; k!= enemies_missiles.size();k++)
        {
            if(!player1->isDead())
            {
                //enemies missiles/player
                if(physics_engine->collision( player1->get_hitbox(), enemies_missiles[k]->get_hitbox()))
                {
                    player1->receive_damages(enemies_missiles[k]->put_damages());
                    enemies_missiles[k]->die();
                }
            }
        }

        //***********
        // Movement *
        //***********

        if(!player1->isDead())
            player1->move();

        //The player's missiles movement
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

        //The enemies' missiles movement
        for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size();k++)
        {

            if(enemies_missiles[k] == NULL)
            {
                continue;
            }

            if(enemies_missiles[k]->getX() <= 0 || enemies_missiles[k]->getX() >= game_Xlimit )
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

         //***********************
         //Clean all dead characters *
         //***********************

        // Missiles of the player
        for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size() ;i++)
        {

            if( player_missiles[i] == NULL || player_missiles[i]->isDead() )
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
        graphics_engine->clear();

        /// @todo display background

        //display player's missiles
        for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size();i++)
        {
            err = graphics_engine->put_image(player_missiles[i]->getSurface(),NULL, player_missiles[i]->getPos());

            if(err == false)
            {
                std::cerr << "Fail player missile no " << i << std::endl;
            }
        }

        // display enemies' missiles
        for(std::vector<Missile *>::size_type k = 0; k != enemies_missiles.size();k++)
        {
            err = graphics_engine->put_image(enemies_missiles[k]->getSurface(),NULL, enemies_missiles[k]->getPos());

            if(err == false)
            {
                std::cerr << "Fail enemy missile no " << k << std::endl;
            }
        }

        //display the player
        if(!player1->isDead())
        {
            err = graphics_engine->put_image(player1->getSurface(),NULL, player1->getPos());

            if(err == false)
            {
                std::cerr << "Fail player" << std::endl;
            }
        }

        //display enemies
        for(std::vector<Enemy *>::size_type j = 0; j != enemies.size();j++)
        {
            err = graphics_engine->put_image(enemies[j]->getSurface(),NULL, enemies[j]->getPos());
            if(err == false)
            {
                std::cerr << "Fail enemy no " << j << std::endl;
            }
        }


        graphics_engine->put_image(text,NULL,&posT);

        graphics_engine->update();


        //******
        // FPS *
        //******
        compt++;
        double curr_time = SDL_GetTicks();

        //Framerate regulation
        if( (curr_time - prev_time) < framerate)
        {
            SDL_Delay(framerate - (curr_time - prev_time));
        }

        prev_time = curr_time;

        //Calculating the framerate
        if( (curr_time - ref_time) >= 1000 )
        {
            ref_time = curr_time;
            std::cout << "FPS : " << compt << std::endl;
            compt = 0;
        }
        //SDL_Delay(framerate);
    }

    SDL_ShowCursor(SDL_ENABLE);
    audio_engine->stop_music();

    SDL_FreeSurface(text);

    return true;
}



bool Game::input()
{
    SDL_Event event;
    bool go_on = true;

    if(player1->isLaser_activated())
        player1->fire(MISSILE_TYPE::LASER_TYPE);

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


        Uint8 *keys = SDL_GetKeyState(NULL);


        if(keys[SDLK_UP] )
            player1->set_Yvel(-PLAYER_SPEED);

        if(keys[SDLK_DOWN])
            player1->set_Yvel(PLAYER_SPEED);

        if(keys[SDLK_LEFT])
            player1->set_Xvel(-PLAYER_SPEED);

        if(keys[SDLK_RIGHT])
            player1->set_Xvel(PLAYER_SPEED);

        delete [] keys;

    }

    return go_on;

}


void Game::addEnemyMissile(Missile *m)
{
    enemies_missiles.push_back(m);
}


void Game::addPlayerMissile(Missile *m)
{
    player_missiles.push_back(m);
}





















