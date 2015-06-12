

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
*	@file Boss00.cpp
*	@brief The implementation of the first boss
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL_timer.h>

#include "Boss00.hpp"
#include "../../game/Game.hpp"


Boss00::Boss00(unsigned int hp, unsigned int att, unsigned int sh,
                 SDL_Texture *image, LX_Chunk *audio,
                 Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,dX,dY)
{
    init();
}


Boss00::Boss00(unsigned int hp, unsigned int att, unsigned int sh,
             SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp)
    : Enemy(hp,att,sh,image,audio,rect,sp)
{
    init();
}


void Boss00::init(void)
{
    strat = NULL;
    box.radius = 97;
    box.square_radius = 97*97;

    speed.vx = -3;
    speed.vy = 3;

    strat = new Boss00ShootStrat(this);
}



Missile * Boss00::shoot(MISSILE_TYPE m_type)
{

}


void Boss00::reaction(Missile *target)
{
    Enemy::reaction(target);
}



Boss00::~Boss00()
{
    // Empty
}




Boss00ShootStrat::Boss00ShootStrat(Enemy * newEnemy)
    : Strategy(newEnemy)
{
    delay = 10000;
    shoot_delay = 750;
}

void Boss00ShootStrat::proceed()
{
    static unsigned int begin_time = SDL_GetTicks();
    static unsigned int beginS_time = SDL_GetTicks();

    if((SDL_GetTicks() - beginS_time) > shoot_delay)
    {
        fire(BASIC_MISSILE_TYPE);   /// No real shoot
        beginS_time = SDL_GetTicks();
    }

    if((SDL_GetTicks() - begin_time) > delay)
    {
        fire(BOMB_TYPE);
        begin_time = SDL_GetTicks();
    }
    else
    {
        /*if(target->getX() <= 1000)
        {
            target->set_Xvel(0);
        }*/

        if(target->getY() < 64)
        {
            target->set_Yvel(3);
        }
        else if(target->getY() > 470)
        {
            target->set_Yvel(-3);
        }

        if(target->getX() < 64)
        {
            target->set_Xvel(3);
        }
        else if(target->getX() > 1024)
        {
            target->set_Xvel(-3);
        }
    }

    target->move();
}

void Boss00ShootStrat::fire(MISSILE_TYPE m_type)
{

}

Boss00ShootStrat::~Boss00ShootStrat()
{

}




