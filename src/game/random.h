

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


#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED


/**
*	@file random.h
*	@brief The random file. It contains
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date September 3rd, 2014
*
*
*
*/

#include <cstdio>
#include <cstdlib>
#include <ctime>

#define N 100

unsigned int random(void)
{
    static int tab[N];
    static int first_call = 0;
    int index;
    int val;

    if (first_call == 0)
    {

        srand (time (NULL));
        for (int i = 0; i < N; i++)
        {
            tab[i] = rand() % 100;
            //printf("%d\n", tab[i]);
        }

        first_call = 1;
    }

    index = rand() % 100;
    //printf("index %d ",index);
    val = tab[index];
    tab[index] = rand() % 100;
    return (val);
}



#endif // RANDOM_H_INCLUDED







