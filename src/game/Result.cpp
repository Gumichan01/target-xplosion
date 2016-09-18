
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

#include "Result.hpp"
#include "Rank.hpp"
#include "Game.hpp"
#include "../pattern/Angle.hpp"
#include "../asset/TX_Asset.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Music.hpp>
#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Log.hpp>

#include <SDL2/SDL_events.h>

#include <cmath>
#include <sstream>

using namespace std;
using namespace LX_Win;
using namespace LX_TrueTypeFont;
using namespace LX_Mixer;

static LX_Music *victory = nullptr;

namespace
{
const int TEXT_XPOS = 32;
const int TEXT_YPOS = 88;
const int RANK_SIZE = 512;
const int RESULT_SIZE = 48;
const float ROUND_VALUE = 100.00f;
const int TEN_PERCENT = 10;
const double ANGLE = -M_PI/12;
const int NO_DEATH_BONUS = 1000000;

const SDL_Color WHITE_COLOR = {255,255,255,0};
const SDL_Color BLUE_COLOR = {0,64,255,0};
const SDL_Color RED_COLOR = {255,0,0,0};
const SDL_Color ORANGE_COLOR = {255,127,0,0};
const SDL_Color GREEN_COLOR = {64,255,64,0};

// Percentage of killed enemies
float percentageOf(unsigned int value,unsigned int max)
{
    const float d = (static_cast<float>(value)/static_cast<float>(max));
    const float res = d * 100.0f;

    return static_cast<float>(static_cast<int>(res * ROUND_VALUE))/ROUND_VALUE;
}

// Get the A rank score on a level
inline unsigned int ScoreRankA(unsigned int max)
{
    return (max - (max/TEN_PERCENT));
}

// Get the B rank score on a level
inline unsigned int ScoreRankB(unsigned int max)
{
    return (max - (max/4));
}

inline unsigned int scoreAfterDeath(unsigned int sc, unsigned int nb_death)
{
    if(nb_death >= 1)
        sc /= (nb_death + 1);
    return sc;
}

};


namespace Result
{

void calculateRank(ResultInfo&, LX_Font&,
                   LX_Graphics::LX_BlendedTextImage&);

void calculateResult(ResultInfo&, LX_Font&,LX_Graphics::LX_BlendedTextImage&,
                     LX_Graphics::LX_BlendedTextImage&,LX_Graphics::LX_BlendedTextImage&,
                     LX_Graphics::LX_BlendedTextImage&,LX_Graphics::LX_BlendedTextImage&,
                     LX_Graphics::LX_BlendedTextImage&,LX_Graphics::LX_BlendedTextImage&,
                     LX_Graphics::LX_BlendedTextImage&);


void calculateRank(ResultInfo& info, LX_Font& font,
                   LX_Graphics::LX_BlendedTextImage& rank_btext)
{
    const int VICTORY_A_ID = 11;
    const int VICTORY_B_ID = 10;
    const int VICTORY_C_ID = 9;

    SDL_Color color;
    ostringstream rank_str;
    TX_Asset *a = TX_Asset::getInstance();

    if(info.nb_death > 2)
    {
        rank_str << "D";
        Rank::setRank(C_RANK);
    }
    else if(info.nb_death == 0 &&
            info.nb_killed_enemies >= ScoreRankA(info.max_nb_enemies))
    {
        rank_str << "A";
        victory = new LX_Music(a->getLevelMusic(VICTORY_A_ID));
        Rank::setRank(A_RANK);
    }
    else if(info.nb_death < 2 &&
            info.nb_killed_enemies >= ScoreRankB(info.max_nb_enemies))
    {
        rank_str << "B";
        victory = new LX_Music(a->getLevelMusic(VICTORY_B_ID));
        Rank::setRank(B_RANK);
    }
    else
    {
        rank_str << "C";
        victory = new LX_Music(a->getLevelMusic(VICTORY_C_ID));
        Rank::setRank(C_RANK);
    }

    // Create the texture from the rank
    color = RED_COLOR;
    font.setColor(color);

    rank_btext.setText(rank_str.str(),RANK_SIZE);
    rank_btext.setPosition(Game::getXlim()-RANK_SIZE,TEXT_YPOS);
}

void calculateResult(ResultInfo& info, LX_Font& font,
                     LX_Graphics::LX_BlendedTextImage& result_btext,
                     LX_Graphics::LX_BlendedTextImage& score_btext,
                     LX_Graphics::LX_BlendedTextImage& kill_btext,
                     LX_Graphics::LX_BlendedTextImage& death_btext,
                     LX_Graphics::LX_BlendedTextImage& percent_btext,
                     LX_Graphics::LX_BlendedTextImage& rank_btext,
                     LX_Graphics::LX_BlendedTextImage& current_btext,
                     LX_Graphics::LX_BlendedTextImage& total_btext)
{
    SDL_Color color;
    float percentage;
    string res_str = "======== Result ========";
    ostringstream death_str;
    ostringstream score_str;
    ostringstream kill_str;
    ostringstream percent_str;
    ostringstream final_str;
    ostringstream total_str;

    result_btext.setText(res_str,RESULT_SIZE);
    result_btext.setPosition(TEXT_XPOS,TEXT_YPOS);

    // Create the texture for the score
    score_str << "Score " << info.score;
    score_btext.setText(score_str.str(),RESULT_SIZE);
    score_btext.setPosition(TEXT_XPOS,TEXT_YPOS*2);

    // Create the texture for the killed enemies
    kill_str << "Killed enemies " << info.nb_killed_enemies;
    kill_btext.setText(kill_str.str(),RESULT_SIZE);
    kill_btext.setPosition(TEXT_XPOS,TEXT_YPOS*3);

    // Create this texture if the player has no death
    if(info.nb_death == 0)
    {
        unsigned int bonus_survive = NO_DEATH_BONUS * (info.level +1);
        death_str << "NO DEATH +" << bonus_survive;
        info.score += bonus_survive;
        info.total_score += bonus_survive;
    }
    else
    {
        death_str << info.nb_death << " death(s) -> " << info.score
                  << " / " << info.nb_death + 1;
        info.total_score -= info.score;
        info.score = scoreAfterDeath(info.score,info.nb_death);
        info.total_score += info.score;
    }

    // Blue color
    color = BLUE_COLOR;
    font.setColor(color);

    death_btext.setText(death_str.str(),RESULT_SIZE);
    death_btext.setPosition(TEXT_XPOS,TEXT_YPOS*4);

    // Restore the old color
    color = WHITE_COLOR;
    font.setColor(color);

    // Percentage of success
    percentage = percentageOf(info.nb_killed_enemies,info.max_nb_enemies);
    percent_str << "K.O percentage : " << percentage << "%";
    percent_btext.setText(percent_str.str(),RESULT_SIZE);
    percent_btext.setPosition(TEXT_XPOS,TEXT_YPOS*5);

    // Define the rank
    calculateRank(info,font,rank_btext);

    if(victory != nullptr)
        victory->play();

    // Set Green
    color = ORANGE_COLOR;
    font.setColor(color);

    final_str << "Final score " << info.score;
    current_btext.setText(final_str.str(),RESULT_SIZE);
    current_btext.setPosition(TEXT_XPOS,TEXT_YPOS*6);

    color = GREEN_COLOR;
    font.setColor(color);

    total_str << "Total score " << info.total_score;
    total_btext.setText(total_str.str(),RESULT_SIZE);
    total_btext.setPosition(TEXT_XPOS,TEXT_YPOS*7);
}


// Calculate the result and display it
void displayResult(ResultInfo& info)
{
    LX_Window *window = LX_WindowManager::getInstance()->getWindow(0);
    LX_Font font({255,255,255,0});
    LX_Graphics::LX_BlendedTextImage result_btext(font,*window);
    LX_Graphics::LX_BlendedTextImage score_btext(font,*window);
    LX_Graphics::LX_BlendedTextImage kill_btext(font,*window);
    LX_Graphics::LX_BlendedTextImage death_btext(font,*window);
    LX_Graphics::LX_BlendedTextImage percent_btext(font,*window);
    LX_Graphics::LX_BlendedTextImage rank_btext(font,*window);
    LX_Graphics::LX_BlendedTextImage current_btext(font,*window);
    LX_Graphics::LX_BlendedTextImage total_btext(font,*window);

    calculateResult(info,font,result_btext,score_btext,kill_btext,death_btext,
                    percent_btext,rank_btext,current_btext,total_btext);

    SDL_Event event;
    bool loop = true;

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
        result_btext.draw();
        score_btext.draw();
        kill_btext.draw();
        death_btext.draw();
        percent_btext.draw();
        current_btext.draw();
        total_btext.draw();
        rank_btext.draw(ANGLE);

        window->update();
        LX_Timer::delay(33);
    }

    delete victory;
}

};
