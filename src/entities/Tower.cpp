
/*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*
*/

/**
*	@file Tower.hpp
*	@brief The Towwer(s) implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Tower.hpp"


Tower1::Tower1(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Chunk *audio,
               Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,dX,dY)
{
    strat = new Basic_strategy(this);
}


Tower1::Tower1(unsigned int hp, unsigned int att, unsigned int sh,
                         SDL_Texture *image, LX_Chunk *audio, SDL_Rect *rect,LX_Vector2D *sp)
    : Enemy(hp,att,sh,image,audio,rect,sp)
{
    strat = new Basic_strategy(this);
}


Missile * Tower1::shoot(MISSILE_TYPE m_type)
{
    return NULL;
}


void Tower1::init_box(void)
{
    box = {117,171,109,109*109};
}


Tower1::~Tower1() {}










