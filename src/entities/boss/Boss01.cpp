
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/


#include "Boss01.hpp"
#include "../Bullet.hpp"
#include "../BasicMissile.hpp"
#include "../../game/Game.hpp"
#include "../../game/Rank.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Physics.hpp>

using namespace LX_Physics;

namespace
{
const int WALL_MISSILES = 4;
const int NB_ROW = 2;

// Limits of the boss's action area
const int BOSS_MIN_XPOS = 876;
const int BOSS_MAX_XPOS = 896;
const int BOSS_MIN_YPOS = 152;
const int BOSS_MAX_YPOS = 160;

// Delays fot the Wall strategy
const Uint32 TIME_BETWEEN_WALL_SHOTS = 250;
const Uint32 WALL_SHOTS_TOTAL_DELAY = 2000;

// Extreme limits when the boss use the Row Strategy
const int XLIM = 128;
const int YLIM_UP = 0;
const int YLIM_DOWN = 350;

// These values are used in order to set the position of the missiles
const int X_OFFSET = 74;
const int Y1_OFFSET = 1;
const int Y2_OFFSET = 432;

const Uint32 MOVE_DELAY = 8000;
const Uint32 BOSS_ROW_DELAY = 100;

const Uint32 BOSS01_SPRITE_DISPLAY_DELAY = 125;
const Uint32 BOSS01_DELAY_NOISE = BOSS01_SPRITE_DISPLAY_DELAY*5;

// Constant values for the Position Strategy
const int BULLETS_VEL = 4;
const int BULLETS_DIM = 24;

const int HTOP_X = 105;
const int HTOP_Y = 122;
const int HDOWN_X = 105;
const int HDOWN_Y = 312;
const unsigned int H_RADIUS = 105;

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

};


/* ------------------------
            Boss 01
   ------------------------ */
Boss01::Boss01(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
               int x, int y, int w, int h, float vx, float vy)
    : Boss(hp,att,sh,image,audio,x,y,w,h,vx,vy), id_strat(3),
      htop(new LX_Circle(LX_Point(position.x + HTOP_X,position.y + HTOP_Y),H_RADIUS)),
      hdown(new LX_Circle(LX_Point(position.x + HDOWN_X,position.y + HDOWN_Y),H_RADIUS)) {}


Boss01::~Boss01()
{
    delete htop;
    delete hdown;
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
            g->acceptEnemyMissile(new MegaBullet(attack_val,
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
        g->acceptEnemyMissile(new BasicMissile(attack_val,
                                               rc->getResource(RC_MISSILE,PLAYER_MISSILES+4),
                                               nullptr,rect[i],v));

        /*
            Launch missiles to the other side
            Change the X position of the other bullets
        */
        rect[i].x += MISSILE_WIDTH;
        g->acceptEnemyMissile(new BasicMissile(attack_val,
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
        g->acceptEnemyMissile(new Bullet(attack_val,
                                         rc->getResource(RC_MISSILE,PLAYER_MISSILES+4),
                                         nullptr,rect[j],v));
    }
}


// Shot selecting the kind of the missile
void Boss01::shoot(const MISSILE_TYPE& m_type)
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
        if(id_strat == 1 &&
                (position.x >= BOSS_MIN_XPOS && position.x <= BOSS_MAX_XPOS) &&
                (position.y >= BOSS_MIN_YPOS && position.y <= BOSS_MAX_YPOS))
        {
            // Use the second strategy
            id_strat = 2;
            fire();
            addStrategy(new Boss01WallStrat(this));
            wallTime = SDL_GetTicks();
        }
        else if(id_strat == 2)
        {
            delay = WALL_SHOTS_TOTAL_DELAY;

            if(health_point < halfLife(max_health_point))
                delay = WALL_SHOTS_TOTAL_DELAY/2;

            if(health_point < halfLife(halfLife(max_health_point)))
                delay = WALL_SHOTS_TOTAL_DELAY/4;

            if((SDL_GetTicks() - wallTime) > delay)
            {
                // Use the third strategy
                id_strat = 3;
                addStrategy(new Boss01RowStrat(this));
                rowTime = SDL_GetTicks();
            }
        }
        else if(id_strat == 3)
        {
            delay = static_cast<Uint32>(MOVE_DELAY*1.5);

            if(health_point < halfLife(max_health_point))
                delay = static_cast<Uint32>((MOVE_DELAY*(1.5))/2);

            if((SDL_GetTicks() - wallTime) > delay)
            {
                // Fiirst strategy
                id_strat = 1;
                addStrategy(new Boss01PositionStrat(this));
            }
        }
    }

    Enemy::strategy();
}


void Boss01::move(void)
{
    moveCircle(*htop,speed);
    moveCircle(*hdown,speed);
    Enemy::move();
}

void Boss01::collision(Missile *mi)
{
    if(!mi->isDead() && mi->getX() <= (position.x + position.w))
    {
        if(collisionCircleRect(*htop,*mi->getHitbox()) ||
           collisionCircleRect(*hdown,*mi->getHitbox()))
        {
            reaction(mi);
            mi->die();
        }
    }
}

void Boss01::die()
{
    if(!dying)
    {
        ResourceManager *rc = ResourceManager::getInstance();
        graphic = rc->getResource(RC_XPLOSION,3);
        addStrategy(new DeathStrategy(this,DEFAULT_XPLOSION_DELAY,
                                      BOSS01_DELAY_NOISE));
    }

    Boss::die();
}


/* ------------------------
        Boss Strategy
   ------------------------ */

/* Position */
Boss01PositionStrat::Boss01PositionStrat(Boss01 * newEnemy)
    : Strategy(newEnemy),BossStrategy(newEnemy) {}


Boss01PositionStrat::~Boss01PositionStrat() {}


void Boss01PositionStrat::proceed(void)
{
    const int POS_XVEL = 2;
    const int POS_YVEL = 1;
    // X position
    if(boss->getX() > BOSS_MAX_XPOS)
    {
        boss->setXvel(-POS_XVEL);
    }
    else if(boss->getX() < BOSS_MIN_XPOS)
    {
        boss->setXvel(POS_XVEL);
    }
    else
        boss->setXvel(0);

    // Y position
    if(boss->getY() > BOSS_MAX_YPOS)
    {
        boss->setYvel(-POS_YVEL);
    }
    else if(boss->getY() < BOSS_MIN_YPOS)
    {
        boss->setYvel(POS_YVEL);
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
void Boss01PositionStrat::fire(const MISSILE_TYPE& m_type)
{
    boss->shoot(m_type);
}


/* Shoot */
Boss01WallStrat::Boss01WallStrat(Boss01 *newEnemy)
    : Strategy(newEnemy),BossStrategy(newEnemy),first(1) {}

Boss01WallStrat::~Boss01WallStrat() {}


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


void Boss01WallStrat::fire(const MISSILE_TYPE& m_type)
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


Boss01RowStrat::~Boss01RowStrat() {}

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
        boss->setXvel((-6)*v);
        boss->setYvel(0);
    }

    // On the left of the screen
    if(boss->getX() < XLIM)
    {
        boss->setXvel(0);
    }

    boss->move();
}


void Boss01RowStrat::fire(const MISSILE_TYPE& m_type)
{
    if(m_type == BASIC_MISSILE_TYPE)
        boss->shoot(m_type);
}

