
/*
*   Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
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
*	@brief First Boss
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

//#include <SDL2/SDL_surface.h>

#include "Boss01.hpp"
#include "../Bullet.hpp"
#include "../BasicMissile.hpp"

#include "../../game/Game.hpp"
#include "../../game/Rank.hpp"

#include "../../asset/TX_Asset.hpp"
#include "../../resources/ResourceManager.hpp"


static const int WALL_MISSILES = 4;
static const int NB_ROW = 2;

// Limits of the boss's action area
static const int BOSS_MIN_XPOS = 790;
static const int BOSS_MAX_XPOS = 810;
static const int BOSS_MIN_YPOS = 152;
static const int BOSS_MAX_YPOS = 160;

// Delays fot the Wall strategy
static const Uint32 TIME_BETWEEN_WALL_SHOTS = 250;
static const Uint32 WALL_SHOTS_TOTAL_DELAY = 2000;

// Extreme limits when the boss  use the Row Strategy
static const int XLIM = 128;
static const int YLIM_UP = 0;
static const int YLIM_DOWN = 350;

// These values are used in order to set the position of the missiles
static const int X_OFFSET = 74;
static const int Y1_OFFSET = 1;
static const int Y2_OFFSET = 432;

static const Uint32 MOVE_DELAY = 8000;
static const Uint32 BOSS_ROW_DELAY = 100;

const Uint32 BOSS01_SPRITE_DISPLAY_DELAY = 125;
const Uint32 BOSS01_DELAY_NOISE = BOSS01_SPRITE_DISPLAY_DELAY*5;

// Constant values for the Position Strategy
const int BULLETS_VEL = 4;
const int BULLETS_DIM = 24;

using namespace LX_Physics;

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
            Boss 01
   ------------------------ */
Boss01::Boss01(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Mixer::LX_Chunk *audio,
               int x, int y, int w, int h, float vx, float vy)
    : Boss(hp,att,sh,image,audio,x,y,w,h,vx,vy)
{
    bossInit();
}


void Boss01::bossInit(void)
{
    const unsigned int SHOT_INDEX = 0;
    TX_Asset *asset = TX_Asset::getInstance();
    std::string missilesFile = asset->getEnemyMissilesFile(SHOT_INDEX);

    idStrat = 1;
    strat = new Boss01PositionStrat(this);

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
    // Empty
}


// Default shot, circle bullets
void Boss01::fire(void)
{
    int NB;
    SDL_Rect rect[WALL_MISSILES];
    LX_Vector2D v = LX_Vector2D(0.0f,0.0f);
    unsigned int r = Rank::getRank();

    for(int i = 0; i < WALL_MISSILES; i++)
    {
        // X position and dimension
        rect[i].x = position.x +96;
        rect[i].w = BULLETS_DIM;
        rect[i].h = BULLETS_DIM;
    }

    rect[0].y = position.y + 115;
    rect[1].y = position.y + 150;
    rect[2].y = position.y + 275;
    rect[3].y = position.y + 310;

    // Set the number of bullets according to the rank
    if(r == B_RANK)
        NB = BULLETS_VEL*B_RANK;
    else if(r >= A_RANK)
        NB = BULLETS_VEL*r;
    else
        NB = 1;

    Game * g = Game::getInstance();
    ResourceManager *rc = ResourceManager::getInstance();

    for(int i = 0; i < WALL_MISSILES; i++)
    {
        for(int j = 0; j < NB; j++)
        {
            g->addEnemyMissile(new MegaBullet(attack_val,
                      rc->getResource(RC_MISSILE,4),
                      nullptr,rect[i],v,BULLETS_VEL+j));
        }
    }
}

// Shoot two lines of bullets around the boss
void Boss01::rowShot()
{
    SDL_Rect rect[NB_ROW];

    int sp_offset = static_cast<int>(speed.vy);
    LX_Vector2D v = LX_Vector2D(-MISSILE_SPEED,0);
    LX_Vector2D v2 = LX_Vector2D((MISSILE_SPEED-(MISSILE_SPEED/4)),0);

    rect[0] = {position.x + X_OFFSET,position.y + Y1_OFFSET + sp_offset,
               MISSILE_WIDTH,MISSILE_HEIGHT
              };
    rect[1] = {position.x + X_OFFSET,position.y + Y2_OFFSET + sp_offset,
               MISSILE_WIDTH,MISSILE_HEIGHT
              };

    Game *g = Game::getInstance();
    ResourceManager *rc = ResourceManager::getInstance();

    for(int i = 0; i < NB_ROW; i++)
    {
        g->addEnemyMissile(new BasicMissile(attack_val,
                                            rc->getResource(RC_MISSILE,PLAYER_MISSILES+4),
                                            nullptr,rect[i],v));

        /*
            Launch missiles to the other side
            Change the X position of the other bullets
        */
        rect[i].x += MISSILE_WIDTH;
        g->addEnemyMissile(new BasicMissile(attack_val,
                                            rc->getResource(RC_MISSILE,PLAYER_MISSILES+4),
                                            nullptr,rect[i],v2));
    }
}

// Shoot four bullets at the same time
void Boss01::wallShot()
{
    const int N = WALL_MISSILES;
    SDL_Rect rect[WALL_MISSILES];
    LX_Vector2D v = LX_Vector2D(-ROCKET_SPEED,0);

    for(int i = 0; i < N; i++)
    {
        // X position and dimension
        rect[i].x = position.x +92;
        rect[i].w = 28;
        rect[i].h = 28;
    }

    // Y position of the bullets
    rect[0].y = position.y + 115;
    rect[1].y = position.y + 150;
    rect[2].y = position.y + 275;
    rect[3].y = position.y + 310;

    Game *g = Game::getInstance();
    ResourceManager *rc = ResourceManager::getInstance();

    for(int j = 0; j < N; j++)
    {
        g->addEnemyMissile(new Bullet(attack_val,
                                      rc->getResource(RC_MISSILE,PLAYER_MISSILES+4),
                                      nullptr,rect[j],v));
    }
}


// Shot selecting the kind of the missile
void Boss01::shoot(MISSILE_TYPE m_type)
{
    // Unused
    if(m_type == BASIC_MISSILE_TYPE)
    {
        // Row strategy
        rowShot();
    }
    else if(m_type == ROCKET_TYPE)
    {
        // Wall strategy
        wallShot();
    }
    else
    {
        // Position strat -> circle pattern IF rank = S
        if(Rank::getRank() >= B_RANK)
            fire();
    }
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
            fire();
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
            delay = static_cast<Uint32>(MOVE_DELAY*1.5);

            if(health_point < halfLife(max_health_point))
                delay = static_cast<Uint32>((MOVE_DELAY*(1.5))/2);

            if((SDL_GetTicks() - wallTime) > delay)
            {
                // Fiirst strategy
                idStrat = 1;
                addStrategy(new Boss01PositionStrat(this));
            }
        }
    }

    Enemy::strategy();
}


SDL_Rect * Boss01::getAreaToDisplay()
{
    if(!dying)
        return &sprite[0];

    Uint32 time = SDL_GetTicks();

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


void Boss01::die()
{
    if(!dying)
        addStrategy(new DeathStrategy(this,DEFAULT_XPLOSION_DELAY,
                                      BOSS01_DELAY_NOISE));
    Boss::die();
}


/* ------------------------
        Boss Strategy
   ------------------------ */

/* Position */
Boss01PositionStrat::Boss01PositionStrat(Boss01 * newEnemy)
    : Strategy(newEnemy),BossStrategy(newEnemy)
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
    if(boss->getX() > BOSS_MAX_XPOS)
    {
        boss->setXvel(-2);
    }
    else if(boss->getX() < BOSS_MIN_XPOS)
    {
        boss->setXvel(2);
    }
    else
        boss->setXvel(0);

    // Y position
    if(boss->getY() > BOSS_MAX_YPOS)
    {
        boss->setYvel(-1);
    }
    else if(boss->getY() < BOSS_MIN_YPOS)
    {
        boss->setYvel(1);
    }
    else
        boss->setYvel(0);

    boss->move();       // Move normally

    // Move again
    if(boss->getHP() < halfLife(boss->getMaxHP()))
        boss->move();

    // Move again if life level is too low
    if(boss->getHP() < halfLife(halfLife(boss->getMaxHP())))
        boss->move();
}

// In S rank, the boss uses four circle bullet
void Boss01PositionStrat::fire(MISSILE_TYPE m_type)
{
    boss->shoot(m_type);
}


/* Shoot */
Boss01WallStrat::Boss01WallStrat(Boss01 *newEnemy)
    : Strategy(newEnemy),BossStrategy(newEnemy),first(1)
{
    // Empty
}


Boss01WallStrat::~Boss01WallStrat()
{
    // Empty
}

void Boss01WallStrat::proceed(void)
{
    static Uint32 wall_time = 0;
    Uint32 delay = TIME_BETWEEN_WALL_SHOTS;
    Uint32 total_delay = WALL_SHOTS_TOTAL_DELAY;

    if(first == 1)
    {
        beginWall = SDL_GetTicks();
        first = 0;
    }

    if(boss->getHP() < halfLife(boss->getMaxHP()))
    {
        delay = TIME_BETWEEN_WALL_SHOTS/2;
        total_delay = WALL_SHOTS_TOTAL_DELAY/2;
    }

    if(boss->getHP() < halfLife(halfLife(boss->getMaxHP())))
    {
        delay = TIME_BETWEEN_WALL_SHOTS/4;
        total_delay = WALL_SHOTS_TOTAL_DELAY/4;
    }

    // Shoot during 2 seconds
    if((SDL_GetTicks() - beginWall) < total_delay)
    {
        // Shoot every 250 ms
        if((SDL_GetTicks() - wall_time) > delay)
        {
            fire(ROCKET_TYPE);
            wall_time = SDL_GetTicks();
        }
    }
}


void Boss01WallStrat::fire(MISSILE_TYPE m_type)
{
    if(m_type == ROCKET_TYPE)
        boss->shoot(m_type);
}


/* Row */
Boss01RowStrat::Boss01RowStrat(Boss01 *newEnemy)
    : Strategy(newEnemy),BossStrategy(newEnemy)
{
    boss->setYvel(randBoss01());
    boss->setXvel(0);
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
        fire(BASIC_MISSILE_TYPE);
        t = SDL_GetTicks();
    }

    // The speed of the movement is greater
    if(boss->getHP() < halfLife(boss->getMaxHP()))
    {
        mv_delay = MOVE_DELAY/2;
        v += 1;
    }

    if((SDL_GetTicks() - beginRow) < mv_delay)
    {
        // Move faster
        if(boss->getY() < YLIM_UP)
        {
            boss->setYvel(randBoss01()*v);
        }
        else if(boss->getY() > YLIM_DOWN)
        {
            boss->setYvel(-randBoss01()*v);
        }
    }
    else
    {
        // Go to the left
        boss->setXvel((-5)*v);
        boss->setYvel(0);
    }

    // On the left of the screen
    if(boss->getX() < XLIM)
    {
        boss->setXvel(0);
    }

    boss->move();
}


void Boss01RowStrat::fire(MISSILE_TYPE m_type)
{
    if(m_type == BASIC_MISSILE_TYPE)
        boss->shoot(m_type);
}

