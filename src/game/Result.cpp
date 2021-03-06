
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
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
#include "../utils/misc.hpp"

#include <Lunatix/Texture.hpp>
#include <Lunatix/Window.hpp>
#include <Lunatix/WindowManager.hpp>
#include <Lunatix/TrueTypeFont.hpp>
#include <Lunatix/Music.hpp>
#include <Lunatix/Time.hpp>
#include <Lunatix/Event.hpp>

#include <sstream>

using namespace lx::Win;
using namespace lx::Graphics;
using namespace lx::TrueTypeFont;
using namespace lx::Mixer;
using namespace lx::Event;

static lx::Mixer::Music * victory = nullptr;

namespace
{
const int TEXT_XPOS = 32;
const int TEXT_YPOS = 70;
const int RANK_SIZE = 512;
const int RESULT_SIZE = 48;
const long TEN_PERCENT = 10;
const long QUARTER = 4;
const double ANGLE = -M_PI / 12;
const unsigned long NO_DEATH_BONUS = 100000000;

const lx::Graphics::Colour WHITE_COLOUR = {255, 255, 255, 240};
const lx::Graphics::Colour BLUE_COLOUR = {0, 64, 255, 240};
const lx::Graphics::Colour RED_COLOUR = {255, 0, 0, 240};
const lx::Graphics::Colour ORANGE_COLOUR = {255, 127, 0, 240};
const lx::Graphics::Colour GREEN_COLOUR = {64, 255, 64, 240};


// Get the A rank score on a level
inline unsigned long ScoreRankA_( unsigned long max )
{
    return ( max - ( max / TEN_PERCENT ) );
}

// Get the B rank score on a level
inline unsigned long ScoreRankB_( unsigned long max )
{
    return ( max - ( max / QUARTER ) );
}

inline unsigned long scoreAfterDeath_( unsigned long sc, unsigned int nb_death )
{
    if ( nb_death >= 1 )
        sc /= ( nb_death + 1 );
    return sc;
}

inline UTF8string convertValueToFormattedString_( unsigned long score )
{
    UTF8string u8score( misc::to_string( score ) );
    Scoring::transformStringValue( u8score );
    return u8score;
}

inline bool shouldStopLoop_( const lx::Event::EventHandler& ev ) noexcept
{
    const lx::Event::EventType ty = ev.getEventType();

    return ( ty == lx::Event::EventType::KEYUP && ev.getKeyCode() == SDLK_RETURN ) ||
           ty == lx::Event::EventType::QUIT || ty == lx::Event::EventType::CONTROLLERBUTTONUP
           || ty == lx::Event::EventType::CONTROLLERBUTTONDOWN;
}

}


namespace Result
{

void calculateRank( ResultInfo&, lx::Graphics::BlendedTextTexture& );

void calculateResult( ResultInfo&, lx::Graphics::BlendedTextTexture&, lx::Graphics::BlendedTextTexture&,
                      lx::Graphics::BlendedTextTexture&, lx::Graphics::BlendedTextTexture&,
                      lx::Graphics::BlendedTextTexture&, lx::Graphics::BlendedTextTexture&,
                      lx::Graphics::BlendedTextTexture&, lx::Graphics::BlendedTextTexture& );

void calculateRank( ResultInfo& info, lx::Graphics::BlendedTextTexture& rank_btext )
{
    const int VICTORY_A_ID = 11;
    const int VICTORY_B_ID = 10;
    const int VICTORY_C_ID = 9;

    std::ostringstream rank_str;
    const TX_Asset * a = TX_Asset::getInstance();

    if ( info.nb_death > 2 )
    {
        rank_str << "D";
        victory = nullptr;
    }
    else if ( info.nb_death == 0 &&
              info.nb_killed_enemies >= ScoreRankA_( info.max_nb_enemies ) )
    {
        rank_str << "A";
        victory = new lx::Mixer::Music( a->getLevelMusic( VICTORY_A_ID ) );
    }
    else if ( info.nb_death < 2 &&
              info.nb_killed_enemies >= ScoreRankB_( info.max_nb_enemies ) )
    {
        rank_str << "B";
        victory = new lx::Mixer::Music( a->getLevelMusic( VICTORY_B_ID ) );
    }
    else
    {
        rank_str << "C";
        victory = new lx::Mixer::Music( a->getLevelMusic( VICTORY_C_ID ) );
    }

    rank_btext.setText( rank_str.str(), RANK_SIZE );
    rank_btext.setPosition( Engine::getMaxXlim() - RANK_SIZE, TEXT_YPOS );
}

void calculateResult( ResultInfo& info, lx::Graphics::BlendedTextTexture& result_btext,
                      lx::Graphics::BlendedTextTexture& score_btext,
                      lx::Graphics::BlendedTextTexture& kill_btext,
                      lx::Graphics::BlendedTextTexture& death_btext,
                      lx::Graphics::BlendedTextTexture& rank_btext,
                      lx::Graphics::BlendedTextTexture& current_btext,
                      lx::Graphics::BlendedTextTexture& total_btext,
                      lx::Graphics::BlendedTextTexture& combo_text )
{
    std::string res_str = "======== Result ========";
    std::ostringstream death_str;
    std::ostringstream score_str;
    std::ostringstream kill_str;
    std::ostringstream final_str;
    std::ostringstream total_str;
    std::ostringstream combo_str;

    result_btext.setText( res_str, RESULT_SIZE );
    result_btext.setPosition( TEXT_XPOS, TEXT_YPOS );

    // Create the texture for the score
    score_str << "Score: " << convertValueToFormattedString_( info.score );
    score_btext.setText( score_str.str(), RESULT_SIZE );
    score_btext.setPosition( TEXT_XPOS, TEXT_YPOS * 2 );

    // Create the texture for the killed enemies
    kill_str << "Killed enemies: " << info.nb_killed_enemies;
    kill_btext.setText( kill_str.str(), RESULT_SIZE );
    kill_btext.setPosition( TEXT_XPOS, TEXT_YPOS * 3 );

    // Create this texture if the player has no death
    if ( info.nb_death == 0 )
    {
        unsigned long bonus_survive = NO_DEATH_BONUS * static_cast<unsigned long>( info.level + 1 );
        death_str << "NO DEATH" << " +" << convertValueToFormattedString_( bonus_survive );
        info.score += bonus_survive;
    }
    else
    {
        death_str << info.nb_death << " death(s) -> "
                  << convertValueToFormattedString_( info.score )
                  << " / " << info.nb_death + 1;
        info.score = scoreAfterDeath_( info.score, info.nb_death );
    }

    // Total score
    info.total_score += info.score;

    death_btext.setText( death_str.str(), RESULT_SIZE );
    death_btext.setPosition( TEXT_XPOS, TEXT_YPOS * 4 );

    // Combo
    combo_str << "Max Combo" << ": " << info.max_combo;
    combo_text.setText( combo_str.str(), RESULT_SIZE );
    combo_text.setPosition( TEXT_XPOS, TEXT_YPOS * 5 );

    // Define the rank
    calculateRank( info, rank_btext );

    if ( victory != nullptr )
        victory->play();

    final_str << "Final score" << ": " << convertValueToFormattedString_( info.score );
    current_btext.setText( final_str.str(), RESULT_SIZE );
    current_btext.setPosition( TEXT_XPOS, TEXT_YPOS * 6 );

    total_str << "Total score" << ": "
              << convertValueToFormattedString_( info.total_score );
    total_btext.setText( total_str.str(), RESULT_SIZE );
    total_btext.setPosition( TEXT_XPOS, TEXT_YPOS * 8 );
}


// Calculate the result and display it
void displayResult( ResultInfo& info )
{
    Window& window = WindowManager::getInstance().getWindow( WinID::getWinID() );
    const std::string& font_file = TX_Asset::getInstance()->getFontFile();

    lx::TrueTypeFont::Font font( font_file, WHITE_COLOUR, RESULT_SIZE );
    lx::TrueTypeFont::Font rfont( font_file, RED_COLOUR, RANK_SIZE );
    lx::TrueTypeFont::Font gfont( font_file, GREEN_COLOUR, RESULT_SIZE );
    lx::TrueTypeFont::Font bfont( font_file, BLUE_COLOUR, RESULT_SIZE );
    lx::TrueTypeFont::Font ofont( font_file, ORANGE_COLOUR, RESULT_SIZE );

    lx::Graphics::BlendedTextTexture result_btext( font, window );
    lx::Graphics::BlendedTextTexture score_btext( font, window );
    lx::Graphics::BlendedTextTexture kill_btext( font, window );
    lx::Graphics::BlendedTextTexture death_btext( bfont, window );
    lx::Graphics::BlendedTextTexture rank_btext( rfont, window );
    lx::Graphics::BlendedTextTexture current_btext( ofont, window );
    lx::Graphics::BlendedTextTexture total_btext( gfont, window );
    lx::Graphics::BlendedTextTexture combo_text( font, window );

    calculateResult( info, result_btext, score_btext, kill_btext, death_btext,
                     rank_btext, current_btext, total_btext, combo_text );

    EventHandler event;
    bool stop = false;

    while ( !stop )
    {
        while ( event.pollEvent() )
        {
            stop = shouldStopLoop_( event );
        }

        window.clearWindow();
        result_btext.draw();
        score_btext.draw();
        kill_btext.draw();
        death_btext.draw();
        current_btext.draw();
        combo_text.draw();
        total_btext.draw();
        rank_btext.draw( ANGLE );

        window.update();
        lx::Time::delay( 33 );
    }

    delete victory;
    victory = nullptr;
}

}
