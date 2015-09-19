

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

#include "Boss00.hpp"
#include "../../game/Game.hpp"
#include "../../entities/Bullet.hpp"
#include "../../xml/XMLReader.hpp"

#define rand3() ((LX_Random::xorshiftRand() %3)+2)

using namespace LX_Random;

const double DELAY_NOISE = 3256.00;
const double DELAY_XPLOSION = 4000.00;
const double DELAY_SPRITE = 125.00;
const int NB_SHOTS = 2;

const int XMIN = 1000;
const int XOFFSET =  XMIN + 16;
const int YMIN = 47;
const int YMAX = 500;
const int YOFFSET_MIN = YMIN + 24;
const int YOFFSET_MAX =  YMAX - 24;
const int DELAY_TO_SHOOT = 1000;

const int OFFSET_SHOT1 = 77;
const int OFFSET_SHOT2 = 143;
const int OFFSET_BULLETX = 114;
const int BULLET_SPEED = 9;



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
    const std::string * missiles_files = tx->getEnemyMissilesFiles();

    xtime = SDL_GetTicks();  //

    hitbox.radius = 100;
    hitbox.square_radius = hitbox.radius*hitbox.radius;

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
    LX_Vector2D vel;
    SDL_Rect rect[NB_SHOTS];

    SDL_Surface *bullet_surface = NULL;
    Game *g = Game::getInstance();

    // If the boss cannot shoot according its position
    // Do not shoot!
    if(!canShoot())
        return NULL;

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

    vel = {speed.vx,speed.vy};

    bullet_surface = LX_Graphics::loadSurfaceFromFileBuffer(Bullet::getLightBulletBuffer());

    g->addEnemyMissile(new MegaBullet(attack_val,
                                  LX_Graphics::loadTextureFromSurface(bullet_surface),
                                  NULL,&rect[0],&vel,BULLET_SPEED));

    g->addEnemyMissile(new MegaBullet(attack_val,
                                  LX_Graphics::loadTextureFromSurface(bullet_surface),
                                  NULL,&rect[1],&vel,BULLET_SPEED));

    SDL_FreeSurface(bullet_surface);

    return NULL; // We do not need to use it
}


bool Boss00::canShoot(void)
{
    /*
        If the boss is close to a specific X position and is going to the left,
        OR if the boss is close to a specific X maximum position and is going
        to the bottom of the screen,
        OR If the boss is close to a specific X minimum position and is going
        to the top of the screen, so it cannot shoot
    */

    if((position.x > XMIN && position.x < XOFFSET && speed.vx < 0)
       || (position.y < YMAX && position.y > YOFFSET_MAX && speed.vy > 0)
       || (position.y > YMIN && position.y < YOFFSET_MIN && speed.vy < 0))
    {
        return false;
    }

    return true;
}


void Boss00::die()
{
    speed.vx = XVEL_DIE*3;
    speed.vy = YVEL_DIE;
    move();

    if(dying)
    {
        // Explosion noise during DELAY_NOISE seconds (the total delay)
        // DELAY_SPRITE is the delay of each explosion sound
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
        sound->play();
        begin_die = SDL_GetTicks();
        ref_time = SDL_GetTicks();
    }
}


void Boss00::strategy(void)
{
    if(!dying)
    {
        // Update these variables, it is necessary
        // because the semi-boss need it when it dies
        begin_die = SDL_GetTicks();
        noise_time = SDL_GetTicks();

        // Use the strategy
        Enemy::strategy();
    }
    else
        die();
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
            begin_time = SDL_GetTicks();
        }
        else if(target->getHP() > (target->getMaxHP()/6))
        {
            shot_delay = 250;
            fire(ROCKET_TYPE);
            begin_time = SDL_GetTicks();
        }
        else
        {
            fire(BASIC_MISSILE_TYPE);
            fire(ROCKET_TYPE);
            begin_time = SDL_GetTicks();
        }
    }

    if(target->getX() < XMIN)
    {
        target->setX(XMIN +1);
        target->setXvel(0);
        target->setYvel(1);
    }

    if(target->getY() < YMIN)
        target->setYvel(1);
    else if(target->getY() > YMAX)
        target->setYvel(-1);

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

