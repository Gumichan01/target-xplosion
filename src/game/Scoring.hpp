
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017  Luxon Jean-Pierre
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

namespace LX_TrueTypeFont
{
class LX_Font;
};

namespace LX_Graphics
{
class LX_TextTexture;
};

// The score the enmey gives when it gets hit
const int DAMAGE_SCORE = 10;

class Score
{
    LX_TrueTypeFont::LX_Font *score_font;
    LX_Graphics::LX_TextTexture *score_str_img;
    LX_Graphics::LX_TextTexture *score_val_img;
    unsigned long previous_score;
    unsigned long current_score;
    unsigned long total_score;
    unsigned int killed_enemies;

    Score(const Score& sc);

public:

    Score();

    void notify(int newScore, bool dead=false);
    void display();

    unsigned long getPrevScore() const;
    unsigned long getCurrentScore() const;
    unsigned long getTotalScore() const;
    unsigned int getKilledEnemies() const;
    void resetScore();
    void reset();

    ~Score();
};

#endif // SCORING_H_INCLUDED

