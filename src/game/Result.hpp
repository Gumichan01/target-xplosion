#ifndef RESULT_HPP_INCLUDED
#define RESULT_HPP_INCLUDED


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
*	@file Result.hpp
*	@brief The result file header
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/


/*
    This structure contains information about the result on a specific level
*/
struct ResultInfo
{
    unsigned int level;         /* The level ID */
    int nb_death;               /* Number of player's death */
    unsigned long score;        /* Final score */
    unsigned long max_score;    /* Max score */
};


namespace Result
{

void displayResultConsole(ResultInfo *info);
void displayResult(ResultInfo *info);

};


#endif // RESULT_HPP_INCLUDED


