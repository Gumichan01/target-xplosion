
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

#include "Scoring.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <sstream>

using namespace std;
using namespace LX_Win;
using namespace LX_TrueTypeFont;

const int SCORE_SIZE = 28;
const int SCORE_DEFAULT_POS = 1;
const int SCORE_X = 1;
const int SCORE_Y = 32;


Score::Score()
    : score_font(nullptr),previous_score(0),current_score(0),
      total_score(0), killed_enemies(0)
{
    score_font = new LX_Font({255,255,255,0});
}


void Score::notify(int newScore, bool dead)
{
    unsigned long nscore = static_cast<unsigned long>(newScore);
    long neg_nscore = static_cast<long>(-newScore);

    if(newScore < 0)
    {
        long abs = neg_nscore;

        if(static_cast<unsigned long>(abs) > current_score)
            current_score = 0;
        else
            current_score += nscore;

        if(static_cast<unsigned long>(abs) > total_score)
            total_score = 0;
        else
            total_score += nscore;
    }
    else
    {
        current_score += nscore;
        total_score += nscore;
    }

    // For enemies
    if(dead)
        killed_enemies += 1;
}


void Score::display()
{
    LX_Window *win = LX_WindowManager::getInstance()->getWindow(0);
    std::ostringstream score_sentence;  // The output string
    std::string score_str;              // The score string
    std::string score_val;              // The score value

    score_sentence << "Score";
    score_str = score_sentence.str();
    score_sentence.str("");
    score_sentence << current_score;
    score_val = score_sentence.str();

    LX_Graphics::LX_BlendedTextImage score_str_img(score_str,SCORE_SIZE,*score_font,*win);
    LX_Graphics::LX_BlendedTextImage score_val_img(score_val,SCORE_SIZE,*score_font,*win);

    score_str_img.setPosition(SCORE_DEFAULT_POS,SCORE_DEFAULT_POS);
    score_val_img.setPosition(SCORE_X,SCORE_Y);

    score_str_img.draw();
    score_val_img.draw();
}


unsigned long Score::getPrevScore() const
{
    return previous_score;
}

unsigned long Score::getCurrentScore() const
{
    return current_score;
}

unsigned long Score::getTotalScore()  const
{
    return total_score;
}

unsigned int Score::getKilledEnemies() const
{
    return killed_enemies;
}

void Score::reseScore()
{
    killed_enemies = 0;
    previous_score = current_score;
    current_score = 0;
}

void Score::reset()
{
    previous_score = 0;
    current_score = 0;
    total_score = 0;
}


Score::~Score()
{
    delete score_font;
}
