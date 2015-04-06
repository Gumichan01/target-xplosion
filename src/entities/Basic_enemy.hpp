#ifndef BASIC_ENEMY_H_INCLUDED
#define BASIC_ENEMY_H_INCLUDED



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
*	@file Basic_Enemy.h
*	@brief The basic enemy class
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.2
*	@date September 19th, 2014
*
*
*
*/

#include "Enemy.hpp"


class Basic_Enemy: public Enemy
{


    public:

    Basic_Enemy(unsigned int hp, unsigned int att, unsigned int sh,
            SDL_Texture *image, LX_Chunk *audio,
                Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
        : Enemy(hp,att,sh,image,audio,x,y,w,h,dX,dY)
    {
        strat = new Basic_strategy(this);
    }

    Basic_Enemy(unsigned int hp, unsigned int att, unsigned int sh,
            SDL_Texture *image, LX_Chunk *audio, SDL_Rect *rect,Speed *sp)
        : Enemy(hp,att,sh,image,audio,rect,sp)
    {
        strat = new Basic_strategy(this);
    }

    Missile * shoot(MISSILE_TYPE m_type);


    ~Basic_Enemy(){}

};








#endif // BASIC_ENEMY_H_INCLUDED
