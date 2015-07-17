
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

#include <SDL2/SDL_timer.h>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Random.hpp>

#include "Boss01.hpp"
#include "../../game/Game.hpp"
#include "../../entities/Bullet.hpp"
#include "../../entities/Basic_missile.hpp"

#define WALL_MISSILES 4
#define rand6() ((LX_Random::xorshiftRand() %3)+2)

static const int BOSS_XPOS = 765;
static const int BOSS_YPOS = 155;
static const Uint32 BOSS_WALL_DELAY = 250;
static const Uint32 BOSS_TOTAL_DELAY = 2000;

static const int XLIM = 100;
static const int YLIM_UP = 0;
static const int YLIM_DOWN = 350;

static const Uint32 MOVE_DELAY = 8000;
static const Uint32 BOSS_ROW_DELAY = 100;


/* ------------------------
            Boss
   ------------------------ */

Boss01::Boss01(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Chunk *audio,
               Sint16 x, Sint16 y, Uint16 w, Uint16 h, int dX, int dY)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,dX,dY)
{
    init();
}


Boss01::Boss01(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect, LX_Vector2D *sp)
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
        // Change strategy
        idStrat = 2;
        addStrategy(new Boss01_WallStrat(this));
        wallTime = SDL_GetTicks();
    }
    else if(idStrat == 2)
    {
        if((SDL_GetTicks() - wallTime) > BOSS_TOTAL_DELAY)
        {
            // Change strategy
            idStrat = 3;
            addStrategy(new Boss01_RowStrat(this));
            rowTime = SDL_GetTicks();
        }
    }
    else if(idStrat == 3)
    {
        if((SDL_GetTicks() - wallTime) > (MOVE_DELAY*2))
        {
            idStrat = 1;
            addStrategy(new Boss01_PositionStrat(this));
        }
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


/* Position */

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
        target->set_Xvel(-2);
    }
    else if(target->getX() < BOSS_XPOS)
    {
        target->set_Xvel(2);
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


/* Shoot */


Boss01_WallStrat::Boss01_WallStrat(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    first = 1;
}


Boss01_WallStrat::~Boss01_WallStrat()
{
    // Empty
}

void Boss01_WallStrat::proceed(void)
{
    static Uint32 t = 0;

    if(first == 1)
    {
        beginWall = SDL_GetTicks();
        first = 0;
    }

    // Shoot during 2 seconds
    if((SDL_GetTicks() - beginWall) < BOSS_TOTAL_DELAY)
    {
        // Shoot every 250 ms
        if((SDL_GetTicks() - t) > BOSS_WALL_DELAY)
        {
            fire(ROCKET_TYPE);
            t = SDL_GetTicks();
        }
    }

}


void Boss01_WallStrat::fire(MISSILE_TYPE m_type)
{
    LX_Vector2D v;
    SDL_Rect rect[WALL_MISSILES];
    Game *g = Game::getInstance();

    const int n = WALL_MISSILES;

    // Speed of each bullet
    v = {-ROCKET_SPEED,0};

    // Information of the bullets
    for(int i = 0; i < n; i++)
    {
        rect[i].x = target->getX() + 25;
        rect[i].w = 24;
        rect[i].h = 24;
    }

    // Y position of the bullets
    rect[0].y = target->getY() + 115;
    rect[1].y = target->getY() + 150;
    rect[2].y = target->getY() + 275;
    rect[3].y = target->getY() + 310;


    for(int j = 0; j < n; j++)
    {
        g->addEnemyMissile(new Bullet(target->getATT(),
                                      LX_Graphics::loadTextureFromFile("image/light_bullet.png",0),
                                      NULL,&rect[j],&v));
    }
}


/* Row */

Boss01_RowStrat::Boss01_RowStrat(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    target->set_Yvel(rand6());
    target->set_Xvel(0);
    first = 1;
}


Boss01_RowStrat::~Boss01_RowStrat()
{
    // Empty
}

void Boss01_RowStrat::proceed(void)
{
    static Uint32 t = 0;

    if(first == 1)
    {
        beginRow = SDL_GetTicks();
        first = 0;
    }

    // Row Shoot
    if((SDL_GetTicks() - t) > BOSS_ROW_DELAY)
    {
        fire(ROCKET_TYPE);
        t = SDL_GetTicks();
    }

    if((SDL_GetTicks() - beginRow) < MOVE_DELAY)
    {
        if(target->getY() < YLIM_UP)
        {
            target->set_Yvel(rand6());
        }
        else if(target->getY() > YLIM_DOWN)
        {
            target->set_Yvel(-rand6());
        }
    }
    else
    {
        target->set_Xvel(-4);
        target->set_Yvel(0);
    }

    if(target->getX() < XLIM)
    {
        target->set_Xvel(0);
    }

    target->move();
}


void Boss01_RowStrat::fire(MISSILE_TYPE m_type)
{
    LX_Vector2D v;
    SDL_Rect rect1, rect2;
    Game *g = Game::getInstance();

    v = {-MISSILE_SPEED,0};
    rect1 = {target->getX()+80,target->getY()+1,MISSIlE_WIDTH,MISSILE_HEIGHT};
    rect2 = {target->getX()+80,target->getY()+432,MISSIlE_WIDTH,MISSILE_HEIGHT};;

    g->addEnemyMissile(new Basic_missile(target->getATT(),
                                         LX_Graphics::loadTextureFromFile("image/shoot2.png",0),
                                         NULL,&rect1,&v));

    g->addEnemyMissile(new Basic_missile(target->getATT(),
                                         LX_Graphics::loadTextureFromFile("image/shoot2.png",0),
                                         NULL,&rect2,&v));

}












