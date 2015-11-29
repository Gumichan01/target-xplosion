
/*
*   Target_Xplosion - The classic shoot'em up video game
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
*	@file Result.cpp
*	@brief The result file implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

/// @todo Use a stream instead of sprintf from the C library
#include <cstdio>
#include <iostream>

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_TrueTypeFont.hpp>
#include <LunatiX/LX_Music.hpp>
#include <LunatiX/LX_Hitbox.hpp>

#include "Game.hpp"
#include "Result.hpp"
#include "Rank.hpp"

using namespace std;
using namespace LX_Graphics;
using namespace LX_TrueTypeFont;
using namespace LX_Mixer;

static const int TEXT_YPOS = 88;
static const int TEXTSIZE = 64;
static const int RANK_SIZE = 320;
static const int RESULT_SIZE = 48;
static const float ROUND_VALUE = 100.00;
static const int TEN_PERCENT = 10;
static const int ANGLE = -16;
static const int NO_DEATH_BONUS = 100000;

static const SDL_Color WHITE_COLOR = {255,255,255};
static const SDL_Color BLUE_COLOR = {0,64,255};
static const SDL_Color RED_COLOR = {255,0,0};
static const SDL_Color GREEN_COLOR = {64,255,64};


namespace Result
{
static void display(LX_Window *window, SDL_Texture *result_texture,
                    SDL_Texture *score_texture, SDL_Texture *kill_texture,
                    SDL_Texture *death_texture, SDL_Texture *percent_texture,
                    SDL_Texture *rank_texture, SDL_Rect *rect_result,
                    SDL_Rect *rect_score, SDL_Rect *rect_kill,
                    SDL_Rect *rect_death, SDL_Rect *rect_percent,
                    SDL_Rect *rect_rank, int deaths);
};


static inline float percentageOf(double value,double max)
{
    const double d = (static_cast<double>(value)/static_cast<double>(max));
    const float res = static_cast<float>(d * 100);

    return static_cast<float>(static_cast<int>(res * ROUND_VALUE))/ROUND_VALUE;
}

// Get the A rank score on a level
static inline unsigned long ScoreRankA(int max)
{
    return (max - (max/TEN_PERCENT));
}

// Get the B rank score on a level
static inline unsigned long ScoreRankB(int max)
{
    return (max - (max/4));
}


namespace Result
{

#ifdef DEBUG_TX
// Calculate the result and display it (Debug mode)
void displayResultConsole(ResultInfo& info)
{
    cout << " ==== Result ==== " << endl;
    cout << " Deaths : " << info.nb_death << endl;
    cout << " Score : " << info.score << endl;
    cout << " Killed : " << info.nb_killed_enemies << endl;
    cout << " Max possible number of killed enemies : " << info.max_nb_enemies
         << endl;
    cout << "Success percentage : "
         << percentageOf(info.nb_killed_enemies,info.max_nb_enemies)
         << " %"<< endl;

    if(info.nb_death > 2)
    {
        cout << "Your rank is : D" << endl;
    }
    else if(info.nb_death == 0
            && info.nb_killed_enemies >= ScoreRankA(info.max_nb_enemies))
    {
        cout << "Your rank is : A" << endl;
    }
    else if(info.nb_death < 2
            && info.nb_killed_enemies >= ScoreRankB(info.max_nb_enemies))
    {
        cout << "Your rank is : B" << endl;
    }
    else
    {
        cout << "Your rank is : C" << endl;
    }
}
#endif

// Calculate the result and display it
void displayResult(ResultInfo& info)
{
    SDL_Rect rect_result, rect_score;
    SDL_Rect rect_death, rect_percent;
    SDL_Rect rect_rank, rect_kill;
    SDL_Rect rect_total;
    SDL_Event event;
    SDL_Color color;

    LX_Font font({255,255,255,0});
    LX_Music *victory = nullptr;

    int w,h;
    float percentage;
    bool loop = true;
    bool loaded;
    char res_ch[TEXTSIZE] = "======== Result ========";
    char death_ch[TEXTSIZE];
    char score_ch[TEXTSIZE];
    char kill_ch[TEXTSIZE];
    char rank_ch[TEXTSIZE];
    char percent_ch[TEXTSIZE];
    char total_ch[TEXTSIZE];

    LX_Window *window = nullptr;
    SDL_Texture * result_texture = nullptr;
    SDL_Texture * score_texture = nullptr;
    SDL_Texture * kill_texture = nullptr;
    SDL_Texture * death_texture = nullptr;
    SDL_Texture * percent_texture = nullptr;
    SDL_Texture * rank_texture = nullptr;
    SDL_Texture * total_texture = nullptr;

    window = LX_WindowManager::getInstance()->getWindow(0);

    // The texture to diaplay the result title
    result_texture = font.drawTextToTexture(LX_TTF_BLENDED,res_ch,RESULT_SIZE,window);
    font.sizeOfText(res_ch,RESULT_SIZE,w,h);
    rect_result = {(Game::game_Xlimit-w)/2,TEXT_YPOS,w,h};

    // Create the texture for the score
    sprintf(score_ch,"Score : %ld ",info.score);
    score_texture = font.drawTextToTexture(LX_TTF_BLENDED,score_ch,RESULT_SIZE,window);
    font.sizeOfText(score_ch,RESULT_SIZE,w,h);
    rect_score = {(Game::game_Xlimit-w)/2,TEXT_YPOS*2,w,h};

    // Create the texture for the killed enemies
    sprintf(kill_ch,"Kill : %d ",info.nb_killed_enemies);
    kill_texture = font.drawTextToTexture(LX_TTF_BLENDED,kill_ch,RESULT_SIZE,window);
    font.sizeOfText(score_ch,RESULT_SIZE,w,h);
    rect_kill = {(Game::game_Xlimit-w)/2,TEXT_YPOS*3,w,h};

    // Create this texture if the player has no death
    if(info.nb_death == 0)
    {
        int bonus_survive = NO_DEATH_BONUS * (info.level +1);
        // Blue color
        color = BLUE_COLOR;
        font.setColor(&color);

        sprintf(death_ch,"NO DEATH +%d",bonus_survive);
        death_texture = font.drawTextToTexture(LX_TTF_BLENDED,death_ch,RESULT_SIZE,window);
        font.sizeOfText(death_ch,RESULT_SIZE,w,h);
        rect_death = {(Game::game_Xlimit-w)/2,TEXT_YPOS*4,w,h};

        // Restore the old color
        color = WHITE_COLOR;
        font.setColor(&color);

        info.score += bonus_survive;
    }

    // Percentage of success
    percentage = percentageOf(info.nb_killed_enemies,info.max_nb_enemies);
    sprintf(percent_ch,"Success percentage : %.2f %%",percentage);
    percent_texture = font.drawTextToTexture(LX_TTF_BLENDED,percent_ch,RESULT_SIZE,window);
    font.sizeOfText(percent_ch,RESULT_SIZE,w,h);
    rect_percent = {(Game::game_Xlimit-w)/2,TEXT_YPOS*5,w,h};

    // Define the rank
    if(info.nb_death > 2)
    {
        sprintf(rank_ch,"D");
        loaded = false;
        Rank::setRank(C_RANK);
    }
    else if(info.nb_death == 0 && info.nb_killed_enemies >= ScoreRankA(info.max_nb_enemies))
    {
        sprintf(rank_ch,"A");
        victory = new LX_Music("audio/victory-A.ogg");
        loaded = true;
        Rank::setRank(A_RANK);
    }
    else if(info.nb_death < 2 && info.nb_killed_enemies >= ScoreRankB(info.max_nb_enemies))
    {
        sprintf(rank_ch,"B");
        victory = new LX_Music("audio/victory-B.ogg");
        loaded = true;
        Rank::setRank(B_RANK);
    }
    else
    {
        sprintf(rank_ch,"C");
        victory = new LX_Music("audio/victory-C.ogg");
        loaded = true;
        Rank::setRank(C_RANK);
    }

    // Create the texture from the rank
    color = RED_COLOR;
    font.setColor(&color);

    rank_texture = font.drawTextToTexture(LX_TTF_BLENDED,rank_ch,RANK_SIZE,window);
    font.sizeOfText(rank_ch,RANK_SIZE,w,h);
    rect_rank = {(Game::game_Xlimit-(w*2)),TEXT_YPOS*5,w,h};

    if(loaded)
        victory->play();

    // Incremental display
    display(window,result_texture,score_texture,kill_texture,death_texture,percent_texture,
            rank_texture,&rect_result,&rect_score,&rect_kill,&rect_death,&rect_percent,
            &rect_rank,info.nb_death);

    // Set Green
    color = GREEN_COLOR;
    font.setColor(&color);

    sprintf(total_ch,"Total score : %ld ",info.score);
    total_texture = font.drawTextToTexture(LX_TTF_BLENDED,total_ch,RESULT_SIZE,window);
    font.sizeOfText(total_ch,RESULT_SIZE,w,h);
    rect_total = {(Game::game_Xlimit-w)/2,TEXT_YPOS*6,w,h};

    while(loop)
    {
        while(SDL_PollEvent(&event))
        {
            // Go on
            if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN)
                loop = false;

            // Quit the game
            if(event.type == SDL_QUIT)
                loop = false;
        }

        window->clearWindow();
        window->putTexture(result_texture,nullptr,&rect_result);
        window->putTexture(score_texture,nullptr,&rect_score);
        window->putTexture(kill_texture,nullptr,&rect_kill);

        if(info.nb_death == 0)
            window->putTexture(death_texture,nullptr,&rect_death);

        window->putTexture(percent_texture,nullptr,&rect_percent);
        window->putTexture(total_texture,nullptr,&rect_total);
        window->putTextureAndRotate(rank_texture,nullptr,&rect_rank,ANGLE);

        window->update();
        SDL_Delay(33);
    }

    delete victory;
    SDL_DestroyTexture(rank_texture);
    SDL_DestroyTexture(total_texture);
    SDL_DestroyTexture(percent_texture);
    SDL_DestroyTexture(kill_texture);
    SDL_DestroyTexture(score_texture);
    SDL_DestroyTexture(death_texture);
    SDL_DestroyTexture(result_texture);
}

// Display the result information
void display(LX_Window *window, SDL_Texture *result_texture,
             SDL_Texture *score_texture, SDL_Texture *kill_texture,
             SDL_Texture *death_texture, SDL_Texture *percent_texture,
             SDL_Texture *rank_texture, SDL_Rect *rect_result,
             SDL_Rect *rect_score,SDL_Rect *rect_kill, SDL_Rect *rect_death,
             SDL_Rect *rect_percent, SDL_Rect *rect_rank, int deaths)
{
    // Display results
    SDL_Delay(2000);
    window->clearWindow();
    window->putTexture(result_texture,nullptr,rect_result);
    window->update();

    // Score
    SDL_Delay(450);
    window->clearWindow();
    window->putTexture(result_texture,nullptr,rect_result);
    window->putTexture(score_texture,nullptr,rect_score);
    window->update();
    SDL_Delay(450);

    // Kill
    window->clearWindow();
    window->putTexture(result_texture,nullptr,rect_result);
    window->putTexture(score_texture,nullptr,rect_score);
    window->putTexture(kill_texture,nullptr,rect_kill);
    window->update();
    SDL_Delay(450);


    // Display 'NO DEATH' if it is necessary
    if(deaths == 0)
    {
        window->clearWindow();
        window->putTexture(result_texture,nullptr,rect_result);
        window->putTexture(score_texture,nullptr,rect_score);
        window->putTexture(kill_texture,nullptr,rect_kill);
        window->putTexture(death_texture,nullptr,rect_death);
        window->update();
        SDL_Delay(450);
    }

    // Percentage
    window->clearWindow();
    window->putTexture(result_texture,nullptr,rect_result);
    window->putTexture(score_texture,nullptr,rect_score);
    window->putTexture(kill_texture,nullptr,rect_kill);

    if(deaths == 0)
        window->putTexture(death_texture,nullptr,rect_death);

    window->putTexture(percent_texture,nullptr,rect_percent);
    window->update();

    // Rank
    SDL_Delay(450);
    window->clearWindow();
    window->putTexture(result_texture,nullptr,rect_result);
    window->putTexture(score_texture,nullptr,rect_score);
    window->putTexture(kill_texture,nullptr,rect_kill);

    if(deaths == 0)
        window->putTexture(death_texture,nullptr,rect_death);

    window->putTexture(percent_texture,nullptr,rect_percent);
    window->putTextureAndRotate(rank_texture,nullptr,rect_rank,ANGLE);
    window->update();
}

};

