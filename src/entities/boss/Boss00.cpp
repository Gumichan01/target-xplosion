

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
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Random.hpp>

#include "Boss00.hpp"
#include "../../game/Game.hpp"
#include "../../entities/Basic_missile.hpp"
#include "../../entities/Rocket.hpp"
#include "../../entities/Bomb.hpp"

#define rand3() ((LX_Random::xorshiftRand() %3)+2)

using namespace LX_Random;

const double DELAY_NOISE = 3256.00;
const double DELAY_XPLOSION = 4000.00;
const double DELAY_SPRITE = 125.00;
const int XMIN = 64;
const int XMAX = 1024;
const int YMIN = 64;
const int YMAX = 500;



Boss00::Boss00(unsigned int hp, unsigned int att, unsigned int sh,
                 SDL_Texture *image, LX_Chunk *audio,
                 Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Boss(hp,att,sh,image,audio,x,y,w,h,dX,dY)
{
    bossInit();
}


Boss00::Boss00(unsigned int hp, unsigned int att, unsigned int sh,
             SDL_Texture *image, LX_Chunk *audio,SDL_Rect *rect,LX_Vector2D *sp)
    : Boss(hp,att,sh,image,audio,rect,sp)
{
    bossInit();
}


void Boss00::bossInit(void)
{
    box.radius = 97;
    box.square_radius = 97*97;
    speed.vx = -2;
    speed.vy = 2;

    Boss::bossInit();
    strat = new Boss00ShootStrat(this);

    sprite[0] = {0,0,position.w,position.h};
    sprite[1] = {229,0,position.w,position.h};
    sprite[2] = {458,0,position.w,position.h};
    sprite[3] = {687,0,position.w,position.h};
    sprite[4] = {916,0,position.w,position.h};
    sprite[5] = {1145,0,position.w,position.h};
    sprite[6] = {1374,0,position.w,position.h};
}


void Boss00::reaction(Missile *target)
{
    if(!dying)
    {
        Enemy::reaction(target);
        Boss::bossReaction();
    }
}


Missile * Boss00::shoot(MISSILE_TYPE m_type)
{
    return NULL; // We do not need to use it
}


void Boss00::die()
{
    static double begin_die;

    if(dying)
    {
        // Explosion animation during DELAY_XPLOSION ms
        if((SDL_GetTicks() - begin_die) > DELAY_XPLOSION)
            Boss::bossMustDie();
    }
    else
    {
        // It is time to die
        dying = true;
        Game::getInstance()->stopBossMusic();   // Stop the music
        sound->play();
        begin_die = SDL_GetTicks();
        ref_timeX = SDL_GetTicks();
    }
}


void Boss00::strategy(void)
{
    if(!dying)
        Enemy::strategy();
    else
    {
        die();
    }
}


SDL_Rect * Boss00::getAreaToDisplay()
{
    if(!dying)
        return &sprite[6];
    else
    {
        double time = SDL_GetTicks();
        static double xtime = SDL_GetTicks();
        const static double noise_time = SDL_GetTicks();

        // Explosion noise during DELAY_NOISE seconds
        if((SDL_GetTicks()-noise_time) < DELAY_NOISE && (SDL_GetTicks()-xtime) > (DELAY_SPRITE*3))
        {
            sound->play();
            xtime = SDL_GetTicks();
        }

        if((time-ref_timeX) > (DELAY_SPRITE*5))
        {
            ref_timeX = time - (DELAY_SPRITE*2);
            return &sprite[5];
        }
        else if((time-ref_timeX) > (DELAY_SPRITE*4))
        {
            return &sprite[4];
        }
        else if((time-ref_timeX) > (DELAY_SPRITE*3))
        {
            return &sprite[3];
        }
        else if((time-ref_timeX) > (DELAY_SPRITE*2))
        {
            return &sprite[2];
        }
        else if((time-ref_timeX) > (DELAY_SPRITE))
            return &sprite[1];
        else
            return &sprite[0];
    }
}


Boss00::~Boss00()
{
    // Empty
}


/* Strategy */

Boss00ShootStrat::Boss00ShootStrat(Enemy * newEnemy)
    : Strategy(newEnemy)
{
    shoot_delay = 1000;
}


void Boss00ShootStrat::proceed()
{
    static unsigned int beginS_time = SDL_GetTicks();

    if((SDL_GetTicks() - beginS_time) > shoot_delay)
    {
        if(target->getHP() > (target->getMAX_HP()/2))
        {
            fire(ROCKET_TYPE);
            beginS_time = SDL_GetTicks();
        }
        else
        {
            shoot_delay = 750;
            fire(BASIC_MISSILE_TYPE);
            beginS_time = SDL_GetTicks();
        }
    }

    if(target->getY() < YMIN)
    {
        target->set_Yvel(rand3());
    }
    else if(target->getY() > YMAX)
    {
        target->set_Yvel(-(rand3()));
    }

    if(target->getX() < XMIN)
    {
        target->set_Xvel(rand3());
    }
    else if(target->getX() > XMAX)
    {
        target->set_Xvel(-(rand3()));
    }

    target->move();
}


void Boss00ShootStrat::fire(MISSILE_TYPE m_type)
{
    LX_Vector2D v;
    SDL_Rect rect1, rect2;
    Game *g = Game::getInstance();

    rect1.x = target->getX()+29;
    rect2.x = target->getX()+29;
    rect1.y = target->getY()+77;
    rect2.y = target->getY()+143;

    rect1.w = MISSILE_WIDTH;
    rect1.h = MISSILE_HEIGHT;
    rect2.w = MISSILE_WIDTH;
    rect2.h = MISSILE_HEIGHT;

    v = {-MISSILE_SPEED,0};

    g->addEnemyMissile(new Basic_missile(target->getATT(), LX_Graphics::loadTextureFromFile("image/fire.png",0),NULL,&rect1,&v));
    g->addEnemyMissile(new Basic_missile(target->getATT(), LX_Graphics::loadTextureFromFile("image/fire.png",0),NULL,&rect2,&v));
}


Boss00ShootStrat::~Boss00ShootStrat()
{
    // Empty
}





