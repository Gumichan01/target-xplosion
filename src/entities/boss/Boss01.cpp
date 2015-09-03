
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

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Random.hpp>

#include "Boss01.hpp"
#include "../../game/Game.hpp"
#include "../../entities/Bullet.hpp"
#include "../../entities/BasicMissile.hpp"
#include "../../xml/XMLReader.hpp"

#define rand6() ((LX_Random::xorshiftRand() %3)+2)
#define HALF_LIFE(n) (n/2)                           // The half of health points of the boss


static SDL_Surface * shot_surface;

static const int WALL_MISSILES = 4;
static const int NB_ROW = 2;

static const int BOSS_XPOS_MIN = 764;
static const int BOSS_XPOS_MAX = 771;
static const int BOSS_YPOS_MIN = 152;
static const int BOSS_YPOS_MAX = 160;
static const Uint32 BOSS_WALL_DELAY = 250;
static const Uint32 BOSS_TOTAL_DELAY = 2000;

static const int XLIM = 100;
static const int YLIM_UP = 0;
static const int YLIM_DOWN = 350;

static const Uint32 MOVE_DELAY = 8000;
static const Uint32 BOSS_ROW_DELAY = 100;

const double BOSS01_DELAY_NOISE = 3256.00;
const double BOSS01_DELAY_XPLOSION = 4000.00;
const double BOSS01_DELAY_SPRITE = 125.00;



/* ------------------------
            Boss
   ------------------------ */

Boss01::Boss01(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Chunk *audio,
               Sint16 x, Sint16 y, Uint16 w, Uint16 h, int dX, int dY)
    : Boss(hp,att,sh,image,audio,x,y,w,h,dX,dY)
{
    bossInit();
}


Boss01::Boss01(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect, LX_Vector2D *sp)
    : Boss(hp,att,sh,image,audio,rect,sp)
{
    bossInit();
}


void Boss01::bossInit(void)
{
    const std::string * missilesFiles = TX_Asset::getInstance()->enemyMissilesFiles();

    Boss::bossInit();

    idStrat = 1;
    strat = new Boss01PositionStrat(this);
    shot_surface = LX_Graphics::loadSurface(missilesFiles[0]);

    sprite[0] = {0,0,position.w,position.h};
    sprite[1] = {212,0,position.w,position.h};
    sprite[2] = {424,0,position.w,position.h};
    sprite[3] = {636,0,position.w,position.h};
    sprite[4] = {0,449,position.w,position.h};
    sprite[5] = {212,449,position.w,position.h};
    sprite[6] = {424,449,position.w,position.h};
    sprite[7] = {636,449,position.w,position.h};
    sprite[8] = {848,0,position.w,position.h};
    sprite[9] = {1060,0,position.w,position.h};
    sprite[10] = {1272,0,position.w,position.h};
    sprite[11] = {848,449,position.w,position.h};
    sprite[12] = {1484,0,position.w,position.h};
    sprite[13] = {1060,449,position.w,position.h};
    sprite[14] = {1272,449,position.w,position.h};
    sprite[15] = {1484,449,position.w,position.h};
}


Boss01::~Boss01()
{
    SDL_FreeSurface(shot_surface);
    shot_surface = NULL;
}


void Boss01::move(void)
{
    Enemy::move();
}


void Boss01::reaction(Missile *target)
{
    if(!dying)
    {
        Enemy::reaction(target);
        Boss::bossReaction();
    }
}


void Boss01::die()
{
    static double begin_die = 0;

    speed.vx = -1;
    speed.vy = 1;
    move();

    if(dying)
    {
        // Explosion animation during BOSS01_DELAY_XPLOSION ms
        if((SDL_GetTicks() - begin_die) > BOSS01_DELAY_XPLOSION)
            Boss::bossMustDie();
    }
    else
    {
        // It is time to die
        dying = true;
        Game::getInstance()->stopBossMusic();   // Stop the music
        sound->play();
        begin_die = SDL_GetTicks();
        ref_time = SDL_GetTicks();
    }
}


void Boss01::strategy(void)
{
    Uint32 delay;

    if(!dying)
    {
        if(idStrat == 1 &&
                (position.x >= BOSS_XPOS_MIN && position.x <= BOSS_XPOS_MAX) &&
                (position.y >= BOSS_YPOS_MIN && position.y <= BOSS_YPOS_MAX))
        {
            // Change strategy
            idStrat = 2;
            addStrategy(new Boss01WallStrat(this));
            wallTime = SDL_GetTicks();
        }
        else if(idStrat == 2)
        {
            delay = BOSS_TOTAL_DELAY;

            if(health_point < HALF_LIFE(max_health_point))
                delay = BOSS_TOTAL_DELAY/2;

            if(health_point < HALF_LIFE(HALF_LIFE(max_health_point)))
                delay = BOSS_TOTAL_DELAY/4;

            if((SDL_GetTicks() - wallTime) > delay)
            {
                // Change strategy
                idStrat = 3;
                addStrategy(new Boss01RowStrat(this));
                rowTime = SDL_GetTicks();
            }
        }
        else if(idStrat == 3)
        {
            delay = (MOVE_DELAY*(1.5));

            if(health_point < HALF_LIFE(max_health_point))
                delay = (MOVE_DELAY*(1.5))/2;

            if((SDL_GetTicks() - wallTime) > delay)
            {
                idStrat = 1;
                addStrategy(new Boss01PositionStrat(this));
            }
        }

        Enemy::strategy();
    }
    else
        die();
}


SDL_Rect * Boss01::getAreaToDisplay()
{
    if(!dying)
        return &sprite[0];

    double time = SDL_GetTicks();
    static double xtime = SDL_GetTicks();
    const static double NOISE_TIME = SDL_GetTicks();

    // Explosion noise during DELAY_NOISE seconds
    if((SDL_GetTicks() - NOISE_TIME) < BOSS01_DELAY_NOISE &&
            (SDL_GetTicks()-xtime) > (BOSS01_DELAY_SPRITE*5))
    {
        sound->play();
        xtime = SDL_GetTicks();
    }

    if((time-ref_time) > (BOSS01_DELAY_SPRITE*15))
    {
        ref_time = time;
        return &sprite[15];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*14))
    {
        return &sprite[14];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*13))
    {
        return &sprite[13];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*12))
    {
        return &sprite[12];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*11))
    {
        return &sprite[11];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*10))
    {
        return &sprite[10];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*9))
    {
        return &sprite[9];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*8))
    {
        return &sprite[8];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*7))
    {
        return &sprite[7];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*6))
    {
        return &sprite[6];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*5))
    {
        return &sprite[5];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*4))
    {
        return &sprite[4];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*3))
    {
        return &sprite[3];
    }
    else if((time-ref_time) > (BOSS01_DELAY_SPRITE*2))
    {
        return &sprite[2];
    }
    else
        return &sprite[1];
}


Missile * Boss01::shoot(MISSILE_TYPE m_type)
{
    return NULL;
}



/* ------------------------
        Boss Strategy
   ------------------------ */


/* Position */

Boss01PositionStrat::Boss01PositionStrat(Enemy * newEnemy)
    : Strategy(newEnemy)
{
    // Empty
}


Boss01PositionStrat::~Boss01PositionStrat()
{
    // Empty
}


void Boss01PositionStrat::proceed(void)
{
    // X position
    if(target->getX() > BOSS_XPOS_MAX)
    {
        target->setXvel(-2);
    }
    else if(target->getX() < BOSS_XPOS_MIN)
    {
        target->setXvel(2);
    }
    else
        target->setXvel(0);

    // Y position
    if(target->getY() > BOSS_YPOS_MAX)
    {
        target->setYvel(-1);
    }
    else if(target->getY() < BOSS_YPOS_MIN)
    {
        target->setYvel(1);
    }
    else
        target->setYvel(0);

    target->move();

    if(target->getHP() < HALF_LIFE(target->getMaxHP()))
        target->move();

    if(target->getHP() < HALF_LIFE(HALF_LIFE(target->getMaxHP())))
        target->move();
}


void Boss01PositionStrat::fire(MISSILE_TYPE m_type)
{
    // Empty
}


/* Shoot */


Boss01WallStrat::Boss01WallStrat(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    first = 1;
}


Boss01WallStrat::~Boss01WallStrat()
{
    // Empty
}

void Boss01WallStrat::proceed(void)
{
    static Uint32 t = 0;
    Uint32 delay = BOSS_WALL_DELAY;
    Uint32 total_delay = BOSS_TOTAL_DELAY;

    if(first == 1)
    {
        beginWall = SDL_GetTicks();
        first = 0;
    }

    if(target->getHP() < HALF_LIFE(target->getMaxHP()))
    {
        delay = BOSS_WALL_DELAY/2;
        total_delay = BOSS_TOTAL_DELAY/2;
    }

    if(target->getHP() < HALF_LIFE(HALF_LIFE(target->getMaxHP())))
    {
        delay = BOSS_WALL_DELAY/4;
        total_delay = BOSS_TOTAL_DELAY/4;
    }

    // Shoot during 2 seconds
    if((SDL_GetTicks() - beginWall) < total_delay)
    {
        // Shoot every 250 ms
        if((SDL_GetTicks() - t) > delay)
        {
            fire(ROCKET_TYPE);
            t = SDL_GetTicks();
        }
    }

}


void Boss01WallStrat::fire(MISSILE_TYPE m_type)
{
    LX_Vector2D v;
    SDL_Rect rect[WALL_MISSILES];
    Game *g = Game::getInstance();
    SDL_Surface *bullet_surface = NULL;

    const int N = WALL_MISSILES;

    // Speed of each bullet
    v = {-ROCKET_SPEED,0};

    // Information of the bullets
    for(int i = 0; i < N; i++)
    {
        rect[i].x = target->getX();
        rect[i].w = 28;
        rect[i].h = 28;
    }

    // Y position of the bullets
    rect[0].y = target->getY() + 115;
    rect[1].y = target->getY() + 150;
    rect[2].y = target->getY() + 275;
    rect[3].y = target->getY() + 310;

    bullet_surface = LX_Graphics::loadSurfaceFromFileBuffer(Bullet::getLightBulletBuffer());

    for(int j = 0; j < N; j++)
    {
        g->addEnemyMissile(new Bullet(target->getATT(),
                                      LX_Graphics::loadTextureFromSurface(bullet_surface),
                                      NULL,&rect[j],&v));
    }
    SDL_FreeSurface(bullet_surface);
}


/* Row */

Boss01RowStrat::Boss01RowStrat(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    target->setYvel(rand6());
    target->setXvel(0);
    first = 1;
}


Boss01RowStrat::~Boss01RowStrat()
{
    // Empty
}

void Boss01RowStrat::proceed(void)
{
    static Uint32 t = 0;
    int v = 1;
    Uint32 mv_delay = MOVE_DELAY;

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

    if(target->getHP() < HALF_LIFE(target->getMaxHP()))
    {
        mv_delay = MOVE_DELAY/2;
        v += 1;
    }

    if((SDL_GetTicks() - beginRow) < mv_delay)
    {
        if(target->getY() < YLIM_UP)
        {
            target->setYvel(rand6()*v);
        }
        else if(target->getY() > YLIM_DOWN)
        {
            target->setYvel(-rand6()*v);
        }
    }
    else
    {
        target->setXvel((-5)*v);
        target->setYvel(0);
    }

    if(target->getX() < XLIM)
    {
        target->setXvel(0);
    }

    target->move();
}


void Boss01RowStrat::fire(MISSILE_TYPE m_type)
{
    LX_Vector2D v;
    SDL_Rect rect[NB_ROW];
    Game *g = Game::getInstance();

    v = {-MISSILE_SPEED,0};
    rect[0] = {target->getX()+64,target->getY()+1,MISSILE_WIDTH,MISSILE_HEIGHT};
    rect[1] = {target->getX()+64,target->getY()+432,MISSILE_WIDTH,MISSILE_HEIGHT};;


    for(int i = 0; i < NB_ROW; i++)
    {
        g->addEnemyMissile(new BasicMissile(target->getATT(),
                                            LX_Graphics::loadTextureFromSurface(shot_surface),
                                            NULL,&rect[i],&v));
    }
}


