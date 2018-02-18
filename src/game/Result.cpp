
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017 Luxon Jean-Pierre
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
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "Result.hpp"
#include "Scoring.hpp"
#include "engine/Engine.hpp"
#include "../asset/TX_Asset.hpp"
#include "../resources/WinID.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_TrueTypeFont.hpp>
#include <LunatiX/LX_Music.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Event.hpp>

#include <sstream>

using namespace std;
using namespace LX_Win;
using namespace LX_Graphics;
using namespace LX_TrueTypeFont;
using namespace LX_Mixer;
using namespace LX_Event;

static LX_Music *victory = nullptr;

namespace
{
const int TEXT_XPOS = 32;
const int TEXT_YPOS = 70;
const int RANK_SIZE = 512;
const int RESULT_SIZE = 48;
const long TEN_PERCENT = 10;
const long QUARTER = 4;
const double ANGLE = -M_PI/12;
const unsigned long NO_DEATH_BONUS = 100000000;

const LX_Colour WHITE_COLOUR = {255,255,255,0};
const LX_Colour BLUE_COLOUR = {0,64,255,0};
const LX_Colour RED_COLOUR = {255,0,0,0};
const LX_Colour ORANGE_COLOUR = {255,127,0,0};
const LX_Colour GREEN_COLOUR = {64,255,64,0};

const UTF8string RES_A_BUTTON("a");

// Get the A rank score on a level
inline unsigned long ScoreRankA(unsigned long max)
{
    return (max - (max/TEN_PERCENT));
}

// Get the B rank score on a level
inline unsigned long ScoreRankB(unsigned long max)
{
    return (max - (max/QUARTER));
}

inline unsigned long scoreAfterDeath(unsigned long sc, unsigned int nb_death)
{
    if(nb_death >= 1)
        sc /= (nb_death + 1);
    return sc;
}

inline UTF8string convertValueToFormattedString(unsigned long score)
{
    UTF8string u8score;
    ostringstream ostmp1;
    ostmp1 << score;
    u8score = ostmp1.str();
    Scoring::transformStringValue(u8score);
    return u8score;
}

}


namespace Result
{

void calculateRank(ResultInfo&, LX_BlendedTextTexture&);

void calculateResult(ResultInfo&, LX_BlendedTextTexture&, LX_BlendedTextTexture&,
                     LX_BlendedTextTexture&, LX_BlendedTextTexture&,
                     LX_BlendedTextTexture&, LX_BlendedTextTexture&,
                     LX_BlendedTextTexture&, LX_BlendedTextTexture&);


void calculateRank(ResultInfo& info, LX_BlendedTextTexture& rank_btext)
{
    const int VICTORY_A_ID = 11;
    const int VICTORY_B_ID = 10;
    const int VICTORY_C_ID = 9;

    ostringstream rank_str;
    const TX_Asset *a = TX_Asset::getInstance();

    if(info.nb_death > 2)
    {
        rank_str << "D";
        victory = nullptr;
    }
    else if(info.nb_death == 0 &&
            info.nb_killed_enemies >= ScoreRankA(info.max_nb_enemies))
    {
        rank_str << "A";
        victory = new LX_Music(a->getLevelMusic(VICTORY_A_ID));
    }
    else if(info.nb_death < 2 &&
            info.nb_killed_enemies >= ScoreRankB(info.max_nb_enemies))
    {
        rank_str << "B";
        victory = new LX_Music(a->getLevelMusic(VICTORY_B_ID));
    }
    else
    {
        rank_str << "C";
        victory = new LX_Music(a->getLevelMusic(VICTORY_C_ID));
    }

    rank_btext.setText(rank_str.str(), RANK_SIZE);
    rank_btext.setPosition(Engine::getMaxXlim()-RANK_SIZE, TEXT_YPOS);
}

void calculateResult(ResultInfo& info, LX_BlendedTextTexture& result_btext,
                     LX_BlendedTextTexture& score_btext,
                     LX_BlendedTextTexture& kill_btext,
                     LX_BlendedTextTexture& death_btext,
                     LX_BlendedTextTexture& rank_btext,
                     LX_BlendedTextTexture& current_btext,
                     LX_BlendedTextTexture& total_btext,
                     LX_BlendedTextTexture& combo_text)
{
    string res_str = "======== Result ========";
    ostringstream death_str;
    ostringstream score_str;
    ostringstream kill_str;
    ostringstream final_str;
    ostringstream total_str;
    ostringstream combo_str;

    result_btext.setText(res_str, RESULT_SIZE);
    result_btext.setPosition(TEXT_XPOS, TEXT_YPOS);

    // Create the texture for the score
    score_str << "Score: " << convertValueToFormattedString(info.score);
    score_btext.setText(score_str.str(), RESULT_SIZE);
    score_btext.setPosition(TEXT_XPOS, TEXT_YPOS*2);

    // Create the texture for the killed enemies
    kill_str << "Killed enemies: " << info.nb_killed_enemies;
    kill_btext.setText(kill_str.str(), RESULT_SIZE);
    kill_btext.setPosition(TEXT_XPOS, TEXT_YPOS*3);

    // Create this texture if the player has no death
    if(info.nb_death == 0)
    {
        unsigned long bonus_survive = NO_DEATH_BONUS * static_cast<unsigned long>(info.level +1);
        death_str << "NO DEATH" << " +" << convertValueToFormattedString(bonus_survive);
        info.score += bonus_survive;
    }
    else
    {
        death_str << info.nb_death << " death(s) -> "
                  << convertValueToFormattedString(info.score)
                  << " / " << info.nb_death + 1;
        info.score = scoreAfterDeath(info.score, info.nb_death);
    }

    // Total score
    info.total_score += info.score;

    death_btext.setText(death_str.str(), RESULT_SIZE);
    death_btext.setPosition(TEXT_XPOS, TEXT_YPOS*4);

    // Combo
    combo_str << "Max Combo" << ": " << info.max_combo;
    combo_text.setText(combo_str.str(), RESULT_SIZE);
    combo_text.setPosition(TEXT_XPOS, TEXT_YPOS*5);

    // Define the rank
    calculateRank(info, rank_btext);

    if(victory != nullptr)
        victory->play();

    final_str << "Final score" << ": " << convertValueToFormattedString(info.score);
    current_btext.setText(final_str.str(), RESULT_SIZE);
    current_btext.setPosition(TEXT_XPOS, TEXT_YPOS*6);

    total_str << "Total score" << ": "
              << convertValueToFormattedString(info.total_score);
    total_btext.setText(total_str.str(), RESULT_SIZE);
    total_btext.setPosition(TEXT_XPOS, TEXT_YPOS*8);
}


// Calculate the result and display it
void displayResult(ResultInfo& info)
{
    LX_Window *window = LX_WindowManager::getInstance()->getWindow(WinID::getWinID());
    const std::string& font_file = TX_Asset::getInstance()->getFontFile();
    LX_Font font(font_file, WHITE_COLOUR, RESULT_SIZE);
    LX_Font rfont(font_file, RED_COLOUR, RANK_SIZE);
    LX_Font gfont(font_file, GREEN_COLOUR, RESULT_SIZE);
    LX_Font bfont(font_file, BLUE_COLOUR, RESULT_SIZE);
    LX_Font ofont(font_file, ORANGE_COLOUR, RESULT_SIZE);
    LX_BlendedTextTexture result_btext(font,*window);
    LX_BlendedTextTexture score_btext(font,*window);
    LX_BlendedTextTexture kill_btext(font,*window);
    LX_BlendedTextTexture death_btext(bfont,*window);
    LX_BlendedTextTexture rank_btext(rfont,*window);
    LX_BlendedTextTexture current_btext(ofont,*window);
    LX_BlendedTextTexture total_btext(gfont,*window);
    LX_BlendedTextTexture combo_text(font,*window);

    calculateResult(info, result_btext, score_btext, kill_btext, death_btext,
                    rank_btext, current_btext, total_btext, combo_text);

    LX_EventHandler event;
    bool loop = true;

    while(loop)
    {
        while(event.pollEvent())
        {
            // Go on
            if(event.getEventType() == LX_EventType::LX_KEYUP
                    && event.getKeyCode() == SDLK_RETURN)
                loop = false;

            if(event.getEventType() == LX_EventType::LX_CONTROLLERBUTTONUP
                    && stringOfButton(event.getButton().value) == RES_A_BUTTON)
                loop = false;

            // Quit the game
            if(event.getEventType() == LX_EventType::LX_QUIT)
                loop = false;
        }

        window->clearWindow();
        result_btext.draw();
        score_btext.draw();
        kill_btext.draw();
        death_btext.draw();
        current_btext.draw();
        combo_text.draw();
        total_btext.draw();
        rank_btext.draw(ANGLE);

        window->update();
        LX_Timer::delay(33);
    }

    delete victory;
    victory = nullptr;
}

}
