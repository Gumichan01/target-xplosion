

/*
*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*
*/

/**
*	@file Game_engine.cpp
*	@brief The game file
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date July 23th, 2014
*
*/

#include "Game_engine.h"


/// @todo enum hitbox

/**
* DOC
*
*
*/
void Game_engine::createPlayer(unsigned int hp, unsigned int att, unsigned int sh, unsigned int critic, unsigned int bombs,
                                SDL_Surface *image, Mix_Chunk *audio,
                                    int x, int y, int w, int h,int dX, int dY)
{
    delete player1;     //clean the ancient player
    // The cast is necessary because of the SDL_Rect fields type
    // check the SDL_Rect documentation
    SDL_Rect new_pos = {(Sint16) x, (Sint16)y,(Uint16) w, (Uint16) h};
    Speed new_speed = {dX,dY};

    player1 = new Player(hp, att, sh, critic, bombs,image, audio,&new_pos,&new_speed);
}


/**
* DOC
*
*
*/
bool Game_engine::play()
{

    SDL_Rect posT = {100,100};

    std::string text= "é jap アカツキアライヴァル";

    bool err = true;
    bool go = true;

    double begin_game_Time = SDL_GetTicks();  // the time at the beginning of the game
    double ref_time = begin_game_Time;       // The reference time for the framerate
    double prev_time = begin_game_Time;     // The previous time for the framerate regulation
    //double final_game_Time = 0;            // The time at the end of the game
    //double end_time = 0;                  // The difference between the begin_game_Time and the final_game_Time
    unsigned int compt = 0;

    double framerate = (SECOND/FRAMERATE);      // The time used to display an image

    std::cout << "Creation personnage\n";

    createPlayer(100,20,10,1,0,graphics_tx->load_image("image/Deltaplane_64x64_alpha.png"),NULL,100,280,64,64,0,0);


    //audio_tx->load_music("sound/Comptroller - Galactic Hero.mp3");
    //audio_tx->load_music("sound/Afterburner.ogg");
    //audio_tx->play_music();
    /// @todo  PUT THE MUSIC

    player_missiles.reserve(16);
    enemies.reserve(16);


    enemies.push_back(new Enemy(20,10,5,graphics_tx->load_image("image/ennemi.png"),NULL,graphics_tx->getWidth(),100,47,47,-2,0));
    enemies.push_back(new Enemy(20,10,5,graphics_tx->load_image("image/ennemi.png"),NULL,graphics_tx->getWidth(),200,47,47,-2,0));
    enemies.push_back(new Enemy(20,10,5,graphics_tx->load_image("image/ennemi.png"),NULL,graphics_tx->getWidth(),300,47,47,-2,0));
    enemies.push_back(new Enemy(20,10,5,graphics_tx->load_image("image/ennemi.png"),NULL,graphics_tx->getWidth(),400,47,47,-2,0));

    enemies.push_back(new Enemy(20,10,5,graphics_tx->load_image("image/ennemi.png"),NULL,graphics_tx->getWidth() *2,100,47,47,-2,0));
    enemies.push_back(new Enemy(20,10,5,graphics_tx->load_image("image/ennemi.png"),NULL,graphics_tx->getWidth() *2,300,47,47,-2,0));
    enemies.push_back(new Enemy(20,10,5,graphics_tx->load_image("image/ennemi.png"),NULL,graphics_tx->getWidth() *2,100,47,47,-2,0));
    enemies.push_back(new Enemy(20,10,5,graphics_tx->load_image("image/ennemi.png"),NULL,graphics_tx->getWidth() *2,300,47,47,-2,0));

    enemies.push_back(new Enemy(100,10,10,graphics_tx->load_image("image/ennemi.png"),NULL,graphics_tx->getWidth() *4,300,47,47,-4,0));

    while(go)
    {

        //Event first (user)
        go = input();


         //***********
         // Physics  *
         //***********

        // collision ennemies/player_misiles & player
        for(std::vector<Enemy *>::size_type j = 0; j != enemies.size() ;j++)
        {

            if(!player1->isDead())
            {
                //enemies/player
                if(physics_tx->collision( player1->get_hitbox(), enemies[j]->get_hitbox()))
                {
                    //std::cout << "COLLISION player/enemy" << std::endl;
                    enemies[j]->receive_damages(player1->getHP());
                    player1->receive_damages(enemies[j]->getHP());
                }
            }

            //std::cout << "3 enemy, x : " << enemies[j]->getX() << ", y : " << enemies[j]->getY() << std::endl;
            //std::cout << "4 enemy, w : " << enemies[j]->getWidth() << ", h : " << enemies[j]->getHeight() << std::endl;

            // enemies/missiles
            for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size() ;i++)
            {

                //std::cout << "1 missile, x : " << player_missiles[i]->getX() << ", y : " << player_missiles[i]->getY() << std::endl;
                //std::cout << "2 missile, w : " << player_missiles[i]->getWidth() << ", h : " << player_missiles[i]->getHeight() << std::endl;


                if(physics_tx->collision( enemies[j]->get_hitbox(), player_missiles[i]->get_hitbox() ))
                {
                    //std::cout << "COLLISION enemy/missile" << std::endl;
                    enemies[j]->receive_damages(player_missiles[i]->put_damages());
                    player_missiles[i]->die();
                }

            }

        }



         //***********
         // Movement *
         //***********
        check_player_action();

        //The missiles movement
        for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size() ;i++)
        {

            if(out_of_screen( player_missiles[i]) )
                player_missiles[i]->die();
            else
                player_missiles[i]->move();

        }

        // The enemies movement
        for(std::vector<Enemy *>::size_type j = 0; j != enemies.size() ;j++)
        {

            if(enemies[j]->getX() <= 0 )
                enemies[j]->die();
            else
                enemies[j]->move();


        }

         //***********************
         //Clean all dead characters *
         //***********************

        // Missiles
        for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size() ;i++)
        {

            if(player_missiles[i]->isDead())
            {
                delete player_missiles[i];
                player_missiles.erase(player_missiles.begin()+i);
                i--;
            }

        }

        // Enemies
        for(std::vector<Enemy *>::size_type j = 0; j != enemies.size() ;j++)
        {
            if(enemies[j]->isDead())
            {
               enemies.erase(enemies.begin()+j);
               j--;
            }
        }


        //*****************
        // Display result *
        //*****************
        graphics_tx->clear();

        //display missiles
        for(std::vector<Missile *>::size_type i = 0; i != player_missiles.size() ;i++)
        {

            err = graphics_tx->put_image(player_missiles[i]->getSurface(),NULL, player_missiles[i]->getPos());
            if(err == false)
            {
                std::cerr << "Fail missile no " << i << "\n";
            }
        }

        //display the player
        if(!player1->isDead())
        {
            err = graphics_tx->put_image(player1->getSurface(),NULL, player1->getPos());

            if(err == false)
            {
                std::cerr << "Fail player\n";
            }
        }

        //display enemies
        for(std::vector<Enemy *>::size_type j = 0; j != enemies.size() ;j++)
        {
            err = graphics_tx->put_image(enemies[j]->getSurface(),NULL, enemies[j]->getPos());
            if(err == false)
            {
                std::cerr << "Fail enemy no " << j << "\n";
            }
        }


        ttf_tx->draw_BlendedText(text.c_str(),&posT);

        graphics_tx->update();


        //******
        // FPS *
        //******
        compt++;
        double curr_time = SDL_GetTicks();

        /*
        fprintf(stdout,"framerate : %f ; diff frame : %f; diff framerate :%f \n",framerate, (curr_time - prev_time),(curr_time - ref_time));
        fprintf(stdout,"compt : %d; wait : %f\n",compt, framerate - (curr_time - prev_time));
        */

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
            fprintf(stdout,"FPS : %d\n",compt);
            compt = 0;
        }
        //SDL_Delay(framerate);
    }

    return true;
}


/**
* DOC
*
*
*/
bool Game_engine::input()
{
    SDL_Event event;
    bool go_on = true;



    while (SDL_PollEvent(&event))
    {
        switch (event.type)
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

                        case SDLK_SPACE: player_missiles.push_back(player1->shoot(MISSILE_TYPE::BASIC_MISSILE_TYPE ) );
                                         break;

                        default : break;
                    }

            }break;

            default:    break;

        }

        Uint8 *keys = SDL_GetKeyState(NULL);

        if(keys[SDLK_UP] )
            player1->set_Yvel(-8);

        if(keys[SDLK_DOWN])
            player1->set_Yvel(8);

        if(keys[SDLK_LEFT])
            player1->set_Xvel(-8);

        if(keys[SDLK_RIGHT])
            player1->set_Xvel(8);

    }

    return go_on;

}


/**
* DOC
*
*
*/
bool Game_engine::out_of_screen(Entity *charac)
{
    return (charac->getX() >= graphics_tx->getWidth() || (charac->getX() + charac->getWidth()) <= 0
                || (charac->getY() + charac->getWidth()) <= 0 || charac->getY() >= graphics_tx->getHeight());
}


/**
* DOC
*
*
*/
void Game_engine::check_player_action()
{
    bool out = false;

    if(!player1->isDead())
    {

        //up
        if( (player1->getX() + player1->get_Xvel())  < 0 )
        {
            player1->setX(0);
            out = true;
        }

        //down
        if( (player1->getY() + player1->get_Yvel()) < 0 )
        {
            player1->setY(0);
            out = true;
        }


        // left
        if( (player1->getY() + player1->getHeight() + player1->get_Yvel()) > graphics_tx->getHeight() )
        {
            player1->setY(graphics_tx->getHeight() - player1->getHeight());
            out = true;
        }

        // right
        if( (player1->getX() + player1->getWidth() + player1->get_Xvel()) > graphics_tx->getWidth() )
        {
            player1->setX(graphics_tx->getWidth() - player1->getWidth());
            out = true;
        }

        if(!out)
        {
            player1->move();
        }

    }

}
























