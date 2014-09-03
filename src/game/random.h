#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED



/*
*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*
*/

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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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







