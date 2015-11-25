
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

#include "Boss01.hpp"
#include "../../game/Game.hpp"
#include "../../entities/Bullet.hpp"
#include "../../entities/BasicMissile.hpp"
#include "../../xml/XMLReader.hpp"
#include "../../pattern/BulletPattern.hpp"


static SDL_Surface * shot_surface;
static const int WALL_MISSILES = 4;
static const int NB_ROW = 2;

static const int BOSS_MIN_XPOS = 764;
static const int BOSS_MAX_XPOS = 771;
static const int BOSS_MIN_YPOS = 152;
static const int BOSS_MAX_YPOS = 160;
static const Uint32 TIME_BTWEEN_WALL_SHOTS = 250;
static const Uint32 WALL_SHOTS_TOTAL_DELAY = 2000;

static const int XLIM = 128;
static const int YLIM_UP = 0;
static const int YLIM_DOWN = 350;

static const int X_OFFSET = 74;
static const int Y1_OFFSET = 1;
static const int Y2_OFFSET = 432;

static const Uint32 MOVE_DELAY = 8000;
static const Uint32 BOSS_ROW_DELAY = 100;

const int BOSS01_SPRITE_DISPLAY_DELAY = 125.00;
const int BULLETS_VEL = 10;

// A specific RNG for the first boss
inline int randBoss01()
{
    return static_cast<int>((LX_Random::xorshiftRand() %3)+2);
}

// The half of health points of the boss
inline unsigned int halfLife(unsigned int n)
{
    return n/2;
}


/* ------------------------
            Boss
   ------------------------ */

Boss01::Boss01(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Chunk *audio,
               Sint16 x, Sint16 y, Uint16 w, Uint16 h, float vx, float vy)
    : Boss(hp,att,sh,image,audio,x,y,w,h,vx,vy)
{
    bossInit();
}


void Boss01::bossInit(void)
{
    const std::string * missilesFiles = TX_Asset::getInstance()->getEnemyMissilesFiles();

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
    shot_surface = nullptr;
}


void Boss01::strategy(void)
{
    Uint32 delay;

    if(!dying)
    {
        if(idStrat == 1 &&
                (position.x >= BOSS_MIN_XPOS && position.x <= BOSS_MAX_XPOS) &&
                (position.y >= BOSS_MIN_YPOS && position.y <= BOSS_MAX_YPOS))
        {
            // Use the second strategy
            idStrat = 2;
            addStrategy(new Boss01WallStrat(this));
            wallTime = SDL_GetTicks();
        }
        else if(idStrat == 2)
        {
            delay = WALL_SHOTS_TOTAL_DELAY;

            if(health_point < halfLife(max_health_point))
                delay = WALL_SHOTS_TOTAL_DELAY/2;

            if(health_point < halfLife(halfLife(max_health_point)))
                delay = WALL_SHOTS_TOTAL_DELAY/4;

            if((SDL_GetTicks() - wallTime) > delay)
            {
                // Use the third strategy
                idStrat = 3;
                addStrategy(new Boss01RowStrat(this));
                rowTime = SDL_GetTicks();
            }
        }
        else if(idStrat == 3)
        {
            delay = (MOVE_DELAY*1.5);

            if(health_point < halfLife(max_health_point))
                delay = (MOVE_DELAY*(1.5))/2;

            if((SDL_GetTicks() - wallTime) > delay)
            {
                // Reuse the first strategy
                idStrat = 1;
                addStrategy(new Boss01PositionStrat(this));
            }
        }

        shoot(BASIC_MISSILE_TYPE);
    }

    Enemy::strategy();
}


SDL_Rect * Boss01::getAreaToDisplay()
{
    if(!dying)
        return &sprite[0];

    int time = SDL_GetTicks();

    if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*15))
    {
        sprite_ref_time = time;
        return &sprite[15];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*14))
    {
        return &sprite[14];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*13))
    {
        return &sprite[13];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*12))
    {
        return &sprite[12];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*11))
    {
        return &sprite[11];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*10))
    {
        return &sprite[10];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*9))
    {
        return &sprite[9];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*8))
    {
        return &sprite[8];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*7))
    {
        return &sprite[7];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*6))
    {
        return &sprite[6];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*5))
    {
        return &sprite[5];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*4))
    {
        return &sprite[4];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*3))
    {
        return &sprite[3];
    }
    else if((time-sprite_ref_time) > (BOSS01_SPRITE_DISPLAY_DELAY*2))
    {
        return &sprite[2];
    }
    else
        return &sprite[1];
}

// Not used
Missile * Boss01::shoot(MISSILE_TYPE m_type)
{
    return nullptr;
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
    if(target->getX() > BOSS_MAX_XPOS)
    {
        target->setXvel(-2);
    }
    else if(target->getX() < BOSS_MIN_XPOS)
    {
        target->setXvel(2);
    }
    else
        target->setXvel(0);

    // Y position
    if(target->getY() > BOSS_MAX_YPOS)
    {
        target->setYvel(-1);
    }
    else if(target->getY() < BOSS_MIN_YPOS)
    {
        target->setYvel(1);
    }
    else
        target->setYvel(0);

    target->move();

    if(target->getHP() < halfLife(target->getMaxHP()))
        target->move();

    if(target->getHP() < halfLife(halfLife(target->getMaxHP())))
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
    Uint32 delay = TIME_BTWEEN_WALL_SHOTS;
    Uint32 total_delay = WALL_SHOTS_TOTAL_DELAY;

    if(first == 1)
    {
        beginWall = SDL_GetTicks();
        first = 0;
    }

    if(target->getHP() < halfLife(target->getMaxHP()))
    {
        delay = TIME_BTWEEN_WALL_SHOTS/2;
        total_delay = WALL_SHOTS_TOTAL_DELAY/2;
    }

    if(target->getHP() < halfLife(halfLife(target->getMaxHP())))
    {
        delay = TIME_BTWEEN_WALL_SHOTS/4;
        total_delay = WALL_SHOTS_TOTAL_DELAY/4;
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
    LX_Physics::LX_Vector2D v;
    SDL_Rect rect[WALL_MISSILES];
    Game *g = Game::getInstance();
    SDL_Surface *bullet_surface = nullptr;

    const int N = WALL_MISSILES;

    // Speed of each bullet
    v = LX_Physics::LX_Vector2D(-ROCKET_SPEED,0);

    // Information of the bullets
    for(int i = 0; i < N; i++)
    {
        rect[i].x = target->getX() +92;
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
                                      nullptr,rect[j],v));
    }
    SDL_FreeSurface(bullet_surface);
}


/* Row */

Boss01RowStrat::Boss01RowStrat(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    target->setYvel(randBoss01());
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

    // The speed of the movement is greater
    if(target->getHP() < halfLife(target->getMaxHP()))
    {
        mv_delay = MOVE_DELAY/2;
        v += 1;
    }

    if((SDL_GetTicks() - beginRow) < mv_delay)
    {
        // Move faster
        if(target->getY() < YLIM_UP)
        {
            target->setYvel(randBoss01()*v);
        }
        else if(target->getY() > YLIM_DOWN)
        {
            target->setYvel(-randBoss01()*v);
        }
    }
    else
    {
        // Go to the left
        target->setXvel((-5)*v);
        target->setYvel(0);
    }

    // On the left of the screen
    if(target->getX() < XLIM)
    {
        target->setXvel(0);
    }

    target->move();
}


void Boss01RowStrat::fire(MISSILE_TYPE m_type)
{
    LX_Physics::LX_Vector2D v,v2;
    SDL_Rect rect[NB_ROW];
    Game *g = Game::getInstance();
    SDL_Surface * bullet_surface = nullptr;

    v = LX_Physics::LX_Vector2D(-MISSILE_SPEED,0);
    v2 = LX_Physics::LX_Vector2D((MISSILE_SPEED-(MISSILE_SPEED/4)),0);

    rect[0] = {target->getX()+X_OFFSET,target->getY()+Y1_OFFSET,
               MISSILE_WIDTH,MISSILE_HEIGHT
              };
    rect[1] = {target->getX()+X_OFFSET,target->getY()+Y2_OFFSET,
               MISSILE_WIDTH,MISSILE_HEIGHT
              };

    bullet_surface = LX_Graphics::loadSurfaceFromFileBuffer(Bullet::getLightBulletBuffer());

    for(int i = 0; i < NB_ROW; i++)
    {
        g->addEnemyMissile(new BasicMissile(target->getATT(),
                                            LX_Graphics::loadTextureFromSurface(shot_surface),
                                            nullptr,rect[i],v));
    }

    // Change the X position of the others bullets
    for(int j = 0; j < NB_ROW; j++)
    {
        rect[j].x = target->getX() + X_OFFSET + MISSILE_WIDTH;
        g->addEnemyMissile(new MegaBullet(target->getATT(),
                                          LX_Graphics::loadTextureFromSurface(bullet_surface),
                                          nullptr,rect[j],v2,BULLETS_VEL));
    }

    SDL_FreeSurface(bullet_surface);
}


