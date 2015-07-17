

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
*	@file Boss01.cpp
*	@brief The header of the second Boss
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/


#include "Boss01.hpp"


static const int BOSS_XPOS = 768;
static const int BOSS_YPOS = 155;




/* ------------------------
            Boss
   ------------------------ */

Boss01::Boss01(unsigned int hp, unsigned int att, unsigned int sh,
                 SDL_Texture *image, LX_Chunk *audio,
                 Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,dX,dY)
{
    init();
}


Boss01::Boss01(unsigned int hp, unsigned int att, unsigned int sh,
             SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp)
    : Enemy(hp,att,sh,image,audio,rect,sp)
{
    init();
}


void Boss01::init(void)
{
    // Empty
    strat = new Boss01_PositionStrat(this);
    idStrat = 1;
}


Boss01::~Boss01()
{

}


void Boss01::move(void)
{
    Enemy::move();
}


void Boss01::reaction(Missile *target)
{
    Enemy::reaction(target);
}


void Boss01::die()
{
    Enemy::die();
}


void Boss01::strategy(void)
{
    if(idStrat == 1 && position.x == BOSS_XPOS && position.y == BOSS_YPOS)
    {
        // Do something
    }

    Enemy::strategy();
}


SDL_Rect * Boss01::getAreaToDisplay()
{
    return NULL;
}


Missile * Boss01::shoot(MISSILE_TYPE m_type)
{
    return NULL;
}



/* ------------------------
        Boss Strategy
   ------------------------ */

Boss01_PositionStrat::Boss01_PositionStrat(Enemy * newEnemy)
    : Strategy(newEnemy)
{
    // Empty
}


Boss01_PositionStrat::~Boss01_PositionStrat()
{
    // Empty
}


void Boss01_PositionStrat::proceed(void)
{
    // X position
    if(target->getX() > BOSS_XPOS)
    {
        target->set_Xvel(-1);
    }
    else if(target->getX() < BOSS_XPOS)
    {
        target->set_Xvel(1);
    }
    else
        target->set_Xvel(0);

    // Y position
    if(target->getY() > BOSS_YPOS)
    {
        target->set_Yvel(-1);
    }
    else if(target->getY() < BOSS_YPOS)
    {
        target->set_Yvel(1);
    }
    else
        target->set_Yvel(0);

    target->move();
}


void Boss01_PositionStrat::fire(MISSILE_TYPE m_type)
{
    // Empty
}








