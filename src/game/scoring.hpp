#ifndef SCORING_H_INCLUDED
#define SCORING_H_INCLUDED


/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
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
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file scoring.hpp
*	@brief The scoring file header
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

namespace LX_TrueTypeFont
{
class LX_Font;
};

// The score the enmey gives when it gets hit
const int DAMAGE_SCORE = 10;

class Score
{
    unsigned long previous_score;
    unsigned long current_score;
    unsigned long total_score;
    LX_TrueTypeFont::LX_Font *score_font;
    static unsigned int killed_enemies;

    Score(const Score& sc);

public:

    explicit Score(unsigned int ps);

    void notify(int newScore,bool dead=false);
    void display(void);

    unsigned long getPrevScore() const;
    unsigned long getCurrentScore() const;
    unsigned long getTotalScore() const;
    static unsigned int getKilledEnemies();

    ~Score();
};

#endif // SCORING_H_INCLUDED

