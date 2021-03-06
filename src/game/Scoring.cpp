
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

#include "Scoring.hpp"
#include "Balance.hpp"
#include "../resources/WinID.hpp"
#include "../asset/TX_Asset.hpp"
#include "../utils/misc.hpp"

#include <Lunatix/Texture.hpp>
#include <Lunatix/Window.hpp>
#include <Lunatix/WindowManager.hpp>
#include <Lunatix/TrueTypeFont.hpp>

#include <sstream>

using namespace lx::Win;
using namespace lx::TrueTypeFont;
using namespace lx::Graphics;

namespace
{
const int SCORE_SIZE = 28;
const int SCORE_DEFAULT_POS = 1;
const int SCORE_X = 1;
const int SCORE_Y = 32;
const lx::Graphics::Colour FONT_COLOUR = {255, 255, 255, 240};
const std::string SCORE_STRING = "Score";
const std::string COMBO_STRING = "Combo";

const unsigned int BASE_LENGTH = 3;
const int COMBO_XPOS = 590;
const int COMBO_YPOS = 1;
const int HITS_PER_COMBO = 4;
}

namespace Scoring
{
/*
    Transform a string representing a value into another one that contains ','


    - example of input: 1256548
    - output: 1,256,548

    @pre the string is a number
*/
void transformStringValue( UTF8string& u8str )
{
    if ( u8str.utf8_length() > BASE_LENGTH )
    {
        int i = 1;
        UTF8string u8tmp( u8str.utf8_reverse() );
        u8str.utf8_clear();

        for ( auto u8it = u8tmp.utf8_begin(); u8it != u8tmp.utf8_end(); ++u8it )
        {
            u8str += *u8it;
            if ( i % 3 == 0 && u8it != u8tmp.utf8_end() - 1 )
                u8str += ",";

            i++;
        }
        u8str.utf8_reverse();
    }
}

}

Score::Score()
    : score_font( nullptr ), score_str_img( nullptr ), score_val_img( nullptr ),
      combo_str_img( nullptr ), combo_val_img( nullptr ), previous_score( 0 ),
      current_score( 0 ), total_score( 0 ), killed_enemies( 0 ), combo( 0 ), max_combo( 0 )
{
    const TX_Asset * const a = TX_Asset::getInstance();
    Window& win = WindowManager::getInstance().getWindow( WinID::getWinID() );
    score_font = new lx::TrueTypeFont::Font( a->getFontFile(), FONT_COLOUR, SCORE_SIZE );

    score_str_img = new lx::Graphics::BlendedTextTexture( *score_font, win );
    combo_str_img = new lx::Graphics::BlendedTextTexture( *score_font, win );
    score_val_img = new lx::Graphics::BlendedTextTexture( *score_font, win );
    combo_val_img = new lx::Graphics::BlendedTextTexture( *score_font, win );

    score_str_img->setText( SCORE_STRING );
    score_str_img->setPosition( SCORE_DEFAULT_POS, SCORE_DEFAULT_POS );
    score_val_img->setPosition( SCORE_X, SCORE_Y );
    combo_str_img->setText( COMBO_STRING );
    combo_str_img->setPosition( COMBO_XPOS, COMBO_YPOS );
    combo_val_img->setPosition( COMBO_XPOS, SCORE_Y );
}

void Score::bonusScore( unsigned long nscore )
{
    current_score += nscore;
    total_score += nscore;
}

void Score::notify( unsigned long nscore, bool dead )
{
    static int hit_count = 0;
    current_score += nscore;
    total_score += nscore;

    if ( hit_count == HITS_PER_COMBO )
    {
        combo += 1;
        hit_count = 0;

        if ( dead )
        {
            killed_enemies += 1;
            current_score += nscore * combo;
            total_score += nscore * combo;
        }

        DynamicGameBalance::notifyCombo();
    }
    else
        hit_count += 1;

    // For enemies
    if ( dead )
    {
        killed_enemies += 1;
        current_score += nscore * ( combo + 1 );
        total_score += nscore * ( combo + 1 );
    }

    update();
}

void Score::update() {}

void Score::displayHUD()
{
    {
        UTF8string u8score( misc::to_string( current_score ) );
        Scoring::transformStringValue( u8score );
        score_val_img->setText( u8score );
    }

    combo_val_img->setText( "x" + misc::to_string( combo ) );
    score_str_img->draw();
    score_val_img->draw();
    combo_str_img->draw();
    combo_val_img->draw();
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

unsigned long Score::getCombo() const
{
    return combo;
}

unsigned long Score::getMaxCombo() const
{
    return ( combo > max_combo ) ? combo : max_combo;
}

void Score::resetCombo()
{
    max_combo = getMaxCombo();
    combo = 0;
}

void Score::resetScore()
{
    killed_enemies = 0;
    previous_score = current_score;
    current_score = 0;
    max_combo = 0;
    combo = 0;
}

Score::~Score()
{
    delete combo_val_img;
    delete combo_str_img;
    delete score_val_img;
    delete score_str_img;
    delete score_font;
}
