

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
*	@file random.hpp
*	@brief The random file. This very small library contains a random number generator
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <cstdio>
#include <cstdlib>
#include <ctime>

#define N 100

// returns a random number between 0 and 100
inline unsigned int random100(void)
{
    static int tab[N];
    static bool first_call = false;
    int index;
    int val;

    if (!first_call)
    {
        for (int i = 0; i < N; i++)
        {
            tab[i] = rand() % N;
        }

        first_call = true;
    }

    index = rand() % N;
    val = tab[index];
    tab[index] = rand() % N;
    return (val);
}

// returns a random number
inline unsigned int randomValue(void)
{
    return rand();
}

// Init the random generator
inline void randomInit()
{
    srand(time(NULL));
}

#endif // RANDOM_H_INCLUDED







