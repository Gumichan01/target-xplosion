

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
*	@file Result.cpp
*	@brief The result file implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/


#include <cstdio>

#include "Result.hpp"

namespace Result
{

void displayResultConsole(ResultInfo *info)
{
    printf(" ==== Result ==== \n");
    printf(" Deaths : %d \n",info->nb_death);
    printf(" Score : %ld \n",info->score);
    printf(" Max possible Score : %ld \n",info->max_score);
    printf("\n");

    if(info->nb_death > 0)
    {
        printf("Your rank is : C \n");
    }
    else if(info->score > (info->max_score - (info->max_score/10)))
    {
        printf("Your rank is : A \n");
    }
    else if(info->score > (info->max_score - (info->max_score/4)))
        printf("Your rank is : B \n");
    else
        printf("Your rank is : C \n");
}


void displayResult(ResultInfo *info)
{
    /// @todo Display the result on the window
}

};


