
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


#include <cstdio>

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_TrueTypeFont.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Music.hpp>

#include "Game.hpp"
#include "Result.hpp"


using namespace LX_Graphics;
using namespace LX_TrueTypeFont;
using namespace LX_Mixer;

static const int TEXT_YPOS = 100;
static const int TEXTSIZE = 64;
static const int RANK_SIZE = 320;
static const int RESULT_SIZE = 48;
static const float ROUND_VALUE = 100.00;
static const int TEN_PERCENT = 10;
static const int ANGLE = -16;


namespace Result
{
static void display(LX_Window *window, SDL_Texture *result_texture,
                    SDL_Texture *score_texture, SDL_Texture *death_texture,
                    SDL_Texture *percent_texture, SDL_Texture *rank_texture,
                    SDL_Rect *rect_result, SDL_Rect *rect_score,
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
static inline unsigned long A_rankScore(int max)
{
    return (max - (max/TEN_PERCENT));
}

// Get the B rank score on a level
static inline unsigned long B_rankScore(int max)
{
    return (max - (max/4));
}


namespace Result
{

void displayResultConsole(ResultInfo *info)
{
    printf(" ==== Result ==== \n");
    printf(" Deaths : %d \n",info->nb_death);
    printf(" Score : %ld \n",info->score);
    printf(" Max possible Score : %ld \n",info->max_score);
    printf("Success percentage : %.2f %%",percentageOf(info->score,info->max_score));
    printf("\n");

    if(info->nb_death > 2)
    {
        printf("Your rank is : D \n");
    }
    else if(info->score > (info->max_score - (info->max_score/TEN_PERCENT)))
    {
        printf("Your rank is : A \n");
    }
    else if(info->nb_death == 1 || info->score > B_rankScore(info->max_score))
        printf("Your rank is : B \n");
    else
        printf("Your rank is : C \n");
}


void displayResult(ResultInfo *info)
{
    SDL_Rect rect_result, rect_score;
    SDL_Rect rect_death, rect_percent;
    SDL_Rect rect_rank;
    SDL_Event event;
    SDL_Color color;

    LX_Font font(NULL);
    LX_Music *victory = NULL;

    int w,h;
    float percentage;
    bool loop = true;
    bool loaded;
    char res_ch[TEXTSIZE] = "======== Result ========";
    char death_ch[TEXTSIZE] = "NO DEATH";
    char score_ch[TEXTSIZE];
    char rank_ch[TEXTSIZE];
    char percent_ch[TEXTSIZE];

    LX_Window *window = NULL;
    SDL_Texture * result_texture = NULL;
    SDL_Texture * score_texture = NULL;
    SDL_Texture * death_texture = NULL;
    SDL_Texture * percent_texture = NULL;
    SDL_Texture * rank_texture = NULL;

    window = LX_WindowManager::getInstance()->getWindow(0);

    if(window == NULL)
        printf("Window is NULL\n");

    // The texture to diaplay the result title
    result_texture = font.drawTextToTexture(LX_TTF_BLENDED,res_ch,RESULT_SIZE,window);
    font.sizeOfText(res_ch,RESULT_SIZE,&w,&h);
    rect_result = {(Game::game_Xlimit-w)/2,TEXT_YPOS,w,h};

    // Create the texture for the score
    sprintf(score_ch,"Score : %ld ",info->score);
    score_texture = font.drawTextToTexture(LX_TTF_BLENDED,score_ch,RESULT_SIZE,window);
    font.sizeOfText(score_ch,RESULT_SIZE,&w,&h);
    rect_score = {(Game::game_Xlimit-w)/2,TEXT_YPOS*2,w,h};

    // Create this texture if the player has no death
    if(info->nb_death == 0)
    {
        color = {0,255,64};
        font.setColor(&color);

        death_texture = font.drawTextToTexture(LX_TTF_BLENDED,death_ch,RESULT_SIZE,window);
        font.sizeOfText(death_ch,RESULT_SIZE,&w,&h);
        rect_death = {(Game::game_Xlimit-w)/2,TEXT_YPOS*3,w,h};

        color = {255,255,255};
        font.setColor(&color);
    }

    // Percentage of success
    percentage = percentageOf(info->score,info->max_score);
    sprintf(percent_ch,"Success percentage : %.2f %%",percentage);
    percent_texture = font.drawTextToTexture(LX_TTF_BLENDED,percent_ch,RESULT_SIZE,window);
    font.sizeOfText(percent_ch,RESULT_SIZE,&w,&h);
    rect_percent = {(Game::game_Xlimit-w)/2,TEXT_YPOS*4,w,h};

    victory = new LX_Music();

    // Define the rank
    if(info->nb_death > 2)
    {
        sprintf(rank_ch,"D");
        loaded = false;
    }
    else if(info->score > A_rankScore(info->max_score))
    {
        sprintf(rank_ch,"A");
        loaded = victory->load("audio/victory-A.ogg");
    }
    else if(info->nb_death == 1 || info->score > B_rankScore(info->max_score))
    {
        sprintf(rank_ch,"B");
        loaded = victory->load("audio/victory-B.ogg");
    }
    else
    {
        sprintf(rank_ch,"C");
        loaded = victory->load("audio/victory-C.ogg");
    }

    // Create the texture from the rank
    color = {255,0,0};
    font.setColor(&color);

    rank_texture = font.drawTextToTexture(LX_TTF_BLENDED,rank_ch,RANK_SIZE,window);
    font.sizeOfText(rank_ch,RANK_SIZE,&w,&h);
    rect_rank = {(Game::game_Xlimit-(w*2)),TEXT_YPOS*4,w,h};

    color = {255,0,0};
    font.setColor(&color);

    // Launch victory music
    if(loaded)
        victory->play();

    display(window,result_texture,score_texture,death_texture,percent_texture,
            rank_texture,&rect_result,&rect_score,&rect_death,&rect_percent,
            &rect_rank,info->nb_death);

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

        window->clearRenderer();
        window->putTexture(result_texture,NULL,&rect_result);
        window->putTexture(score_texture,NULL,&rect_score);

        if(info->nb_death == 0)
            window->putTexture(death_texture,NULL,&rect_death);

        window->putTexture(percent_texture,NULL,&rect_percent);
        window->putTextureAndRotate(rank_texture,NULL,&rect_rank,ANGLE);

        window->updateRenderer();
        SDL_Delay(33);
    }

    delete victory;
    SDL_DestroyTexture(rank_texture);
    SDL_DestroyTexture(percent_texture);
    SDL_DestroyTexture(score_texture);
    SDL_DestroyTexture(death_texture);
    SDL_DestroyTexture(result_texture);
}


void display(LX_Window *window, SDL_Texture *result_texture,
             SDL_Texture *score_texture, SDL_Texture *death_texture,
             SDL_Texture *percent_texture, SDL_Texture *rank_texture,
             SDL_Rect *rect_result, SDL_Rect *rect_score, SDL_Rect *rect_death,
             SDL_Rect *rect_percent, SDL_Rect *rect_rank, int deaths)
{
    // Display results
    SDL_Delay(2000);
    window->clearRenderer();
    window->putTexture(result_texture,NULL,rect_result);
    window->updateRenderer();

    // Score
    SDL_Delay(450);
    window->clearRenderer();
    window->putTexture(result_texture,NULL,rect_result);
    window->putTexture(score_texture,NULL,rect_score);
    window->updateRenderer();
    SDL_Delay(450);

    // Display 'NO DEATH'
    if(deaths == 0)
    {
        window->clearRenderer();
        window->putTexture(result_texture,NULL,rect_result);
        window->putTexture(score_texture,NULL,rect_score);
        window->putTexture(death_texture,NULL,rect_death);
        window->updateRenderer();
        SDL_Delay(450);
    }

    // Percentage
    window->clearRenderer();
    window->putTexture(result_texture,NULL,rect_result);
    window->putTexture(score_texture,NULL,rect_score);

    if(deaths == 0)
        window->putTexture(death_texture,NULL,rect_death);

    window->putTexture(percent_texture,NULL,rect_percent);
    window->updateRenderer();

    // Rank
    SDL_Delay(450);
    window->clearRenderer();
    window->putTexture(result_texture,NULL,rect_result);
    window->putTexture(score_texture,NULL,rect_score);

    if(deaths == 0)
        window->putTexture(death_texture,NULL,rect_death);

    window->putTexture(percent_texture,NULL,rect_percent);
    window->putTextureAndRotate(rank_texture,NULL,rect_rank,ANGLE);
    window->updateRenderer();
    SDL_Delay(2000);
}


};


