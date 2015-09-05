

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

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Random.hpp>

#include "Boss00.hpp"
#include "../../game/Game.hpp"
#include "../../entities/BasicMissile.hpp"
#include "../../entities/Rocket.hpp"
#include "../../entities/Bomb.hpp"
#include "../../entities/Bullet.hpp"
#include "../../xml/XMLReader.hpp"

#define rand3() ((LX_Random::xorshiftRand() %3)+2)

using namespace LX_Random;

const double DELAY_NOISE = 3256.00;
const double DELAY_XPLOSION = 4000.00;
const double DELAY_SPRITE = 125.00;
const int NB_SHOTS = 2;

const int XMIN = 1000;
const int DELAY_TO_SHOOT = 1000;

const int OFFSET_SHOT1 = 77;
const int OFFSET_SHOT2 = 143;
const int OFFSET_BULLETX = 114;
const int OFFSET_BULLETY1 = 82;
const int OFFSET_BULLETY2 = 153;
const int BULLET_SPEED = 12;



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
    TX_Asset *tx = TX_Asset::getInstance();
    const std::string * missiles_files = tx->enemyMissilesFiles();

    box.radius = 97;
    box.square_radius = 97*97;

    Boss::bossInit();
    strat = new Boss00ShootStrat(this);
    shot_surface = LX_Graphics::loadSurface(missiles_files[5]);

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
    LX_Vector2D vel[NB_SHOTS];
    SDL_Rect rect[NB_SHOTS];

    SDL_Surface *bullet_surface = NULL;
    SDL_Texture *shot_texture = NULL;

    Game *g = Game::getInstance();

    vel[0] = {-BULLET_SPEED,-1};
    vel[1] = {-BULLET_SPEED,1};

    bullet_surface = LX_Graphics::loadSurfaceFromFileBuffer(Bullet::getLightBulletBuffer());

    if(m_type == BASIC_MISSILE_TYPE)
    {
        rect[0] = {position.x,position.y + OFFSET_SHOT1,32,32};
        rect[1] = {position.x,position.y + OFFSET_SHOT2,32,32};
    }
    else if(m_type == ROCKET_TYPE)
    {
        rect[0] = {position.x + OFFSET_BULLETX,position.y + OFFSET_SHOT1,32,32};
        rect[1] = {position.x + OFFSET_BULLETX,position.y + OFFSET_SHOT2,32,32};
    }

    for(int j = 0; j < NB_SHOTS; j++)
    {
        g->addEnemyMissile(new Bullet(attack_val,
                                      LX_Graphics::loadTextureFromSurface(bullet_surface),
                                      NULL,&rect[j],&vel[0]));

        g->addEnemyMissile(new Bullet(attack_val,
                                      LX_Graphics::loadTextureFromSurface(bullet_surface),
                                      NULL,&rect[j],&vel[1]));
    }

    SDL_FreeSurface(bullet_surface);

    return NULL; // We do not need to use it
}


void Boss00::die()
{
    static double begin_die;
    static double xtime = SDL_GetTicks();
    const static double noise_time = SDL_GetTicks();

    if(dying)
    {
        // Explosion noise during DELAY_NOISE seconds
        if((SDL_GetTicks()-noise_time) < DELAY_NOISE
           && (SDL_GetTicks()-xtime) > (DELAY_SPRITE*2))
        {
            sound->play();
            xtime = SDL_GetTicks();
        }

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
        ref_time = SDL_GetTicks();
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
    double time;

    if(!dying)
        return &sprite[6];
    else
    {
        time = SDL_GetTicks();

        if((time-ref_time) > (DELAY_SPRITE*5))
        {
            ref_time = time - (DELAY_SPRITE*2);
            return &sprite[5];
        }
        else if((time-ref_time) > (DELAY_SPRITE*4))
        {
            return &sprite[4];
        }
        else if((time-ref_time) > (DELAY_SPRITE*3))
        {
            return &sprite[3];
        }
        else if((time-ref_time) > (DELAY_SPRITE*2))
        {
            return &sprite[2];
        }
        else if((time-ref_time) > (DELAY_SPRITE))
            return &sprite[1];
        else
            return &sprite[0];
    }
}


Boss00::~Boss00()
{
    SDL_FreeSurface(shot_surface);
    shot_surface = NULL;
}


/* Strategy */

Boss00ShootStrat::Boss00ShootStrat(Enemy * newEnemy)
    : Strategy(newEnemy)
{
    shot_delay = DELAY_TO_SHOOT;
    begin_time = SDL_GetTicks();
}


void Boss00ShootStrat::proceed()
{
    if((SDL_GetTicks() - begin_time) > shot_delay)
    {
        if(target->getHP() > (target->getMaxHP() - (target->getMaxHP()/3)))
        {
            fire(BASIC_MISSILE_TYPE);
            begin_time = SDL_GetTicks();
        }
        else if(target->getHP() > (target->getMaxHP()/3))
        {
            shot_delay = 500;
            fire(BASIC_MISSILE_TYPE);
            fire(ROCKET_TYPE);
            begin_time = SDL_GetTicks();
        }
        else if(target->getHP() > (target->getMaxHP()/6))
        {
            shot_delay = 250;
            fire(BASIC_MISSILE_TYPE);
            fire(ROCKET_TYPE);
            begin_time = SDL_GetTicks();
        }
        else
        {
            shot_delay = 125;
            fire(BASIC_MISSILE_TYPE);
            fire(ROCKET_TYPE);
            begin_time = SDL_GetTicks();
        }
    }

    if(target->getX() < XMIN)
    {
        target->setXvel(0);
        target->setYvel(0);
    }

    target->move();
}


void Boss00ShootStrat::fire(MISSILE_TYPE m_type)
{
    target->shoot(m_type);
}


Boss00ShootStrat::~Boss00ShootStrat()
{
    // Empty
}

