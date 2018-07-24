
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
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#ifndef SCORING_H_INCLUDED
#define SCORING_H_INCLUDED

#include "engine/Hud.hpp"

namespace lx
{

namespace TrueTypeFont
{
class Font;
}

namespace Graphics
{
class TextTexture;
}

}

class UTF8string;

namespace Scoring
{
const unsigned long DAMAGE_SCORE = 10;    // The score the enmey gives when it gets hit
void transformStringValue( UTF8string& u8str );
}

class Score final: public HUD
{
    lx::TrueTypeFont::Font * score_font;
    lx::Graphics::TextTexture * score_str_img;
    lx::Graphics::TextTexture * score_val_img;
    lx::Graphics::TextTexture * combo_str_img;
    lx::Graphics::TextTexture * combo_val_img;
    unsigned long previous_score;
    unsigned long current_score;
    unsigned long total_score;
    unsigned int killed_enemies;
    unsigned long combo;
    unsigned long max_combo;

    Score( const Score& sc ) = delete ;
    Score& operator =( const Score& sc ) = delete;

    virtual void update() override;

public:

    Score();

    void bonusScore( unsigned long nscore );
    void notify( unsigned long nscore, bool dead = false );
    void display();

    virtual void displayHUD() override;
    unsigned long getPrevScore() const;
    unsigned long getCurrentScore() const;
    unsigned long getTotalScore() const;
    unsigned int getKilledEnemies() const;
    unsigned long getCombo() const;
    unsigned long getMaxCombo() const;

    void resetCombo();
    void resetScore();

    ~Score();
};

#endif // SCORING_H_INCLUDED
