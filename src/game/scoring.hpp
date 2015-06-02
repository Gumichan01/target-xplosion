#ifndef SCORING_H_INCLUDED
#define SCORING_H_INCLUDED



/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2014  Luxon Jean-Pierre
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
*	@file scoring.h
*	@brief The scoring file header
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#define VAL_SCORE_SIZE 0


namespace LX_TrueTypeFont{

class LX_Font;

};

using namespace LX_TrueTypeFont;


class Score
{

    unsigned int previous_score;
    unsigned int current_score;
    unsigned int total_score;
    LX_Font *score_font;


    public:

    Score(unsigned int ps);

    void notify(int newScore);
    void display(void);

    unsigned int get_prev_score(){return previous_score;}
    unsigned int get_cur_score(){return current_score;}
    unsigned int get_total_score(){return total_score;}


    ~Score();
};


#endif // SCORING_H_INCLUDED

