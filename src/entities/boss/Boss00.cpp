

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

#include "Boss00.hpp"
#include "../../game/Game.hpp"
#include "../../entities/Basic_missile.hpp"
#include "../../entities/Rocket.hpp"
#include "../../entities/Bomb.hpp"

const double DELAY_NOISE = 3256.00;
const double DELAY_XPLOSION = 4000.00;
const double DELAY_SPRITE = 125.00;


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
    dying = false;
    box.radius = 97;
    box.square_radius = 97*97;
    speed.vx = -2;
    speed.vy = 2;

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

        if(health_point == 0)
        {
            // Cancel the kill, the boss will die
            wasKilled = false;
        }
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
        {
            wasKilled = true;   // It was set to false, so set it to true
            Entity::die();
        }
    }
    else
    {
        // It is time to die
        dying = true;
        begin_die = SDL_GetTicks();
        ref_timeB = SDL_GetTicks();
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

        if((time-ref_timeB) > (DELAY_SPRITE*5))
        {
            ref_timeB = time - (DELAY_SPRITE*2);
            return &sprite[5];
        }
        else if((time-ref_timeB) > (DELAY_SPRITE*4))
        {
            return &sprite[4];
        }
        else if((time-ref_timeB) > (DELAY_SPRITE*3))
        {
            return &sprite[3];
        }
        else if((time-ref_timeB) > (DELAY_SPRITE*2))
        {
            return &sprite[2];
        }
        else if((time-ref_timeB) > (DELAY_SPRITE))
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
            fire(ROCKET_TYPE);
            beginS_time = SDL_GetTicks();
        }
    }

    if(target->getY() < 64)
    {
        target->set_Yvel(2);
    }
    else if(target->getY() > 470)
    {
        target->set_Yvel(-2);
    }

    if(target->getX() < 64)
    {
        target->set_Xvel(2);
    }
    else if(target->getX() > 1024)
    {
        target->set_Xvel(-2);
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

    if(m_type == ROCKET_TYPE)
    {
        rect1.y = target->getY()+77;
        rect2.y = target->getY()+143;
        rect1.w = MISSIlE_WIDTH;
        rect1.h = MISSILE_HEIGHT;
        rect2.w = MISSIlE_WIDTH;
        rect2.h = MISSILE_HEIGHT;

        v = {-MISSILE_SPEED,0};

        g->addEnemyMissile(new Rocket(target->getATT(), LX_Graphics::loadTextureFromFile("image/fire.png",0),NULL,&rect1,&v));
        g->addEnemyMissile(new Rocket(target->getATT(), LX_Graphics::loadTextureFromFile("image/fire.png",0),NULL,&rect2,&v));
    }
}


Boss00ShootStrat::~Boss00ShootStrat()
{
    // Empty
}


