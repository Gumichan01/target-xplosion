

/*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.kappatau.fr
*
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*
*/

/**
*	@file SemiBoss01.cpp
*	@brief The implementation of the first boss
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "SemiBoss01.hpp"
#include "../Bullet.hpp"
#include "../BasicMissile.hpp"

#include "../../game/Game.hpp"
#include "../../game/Rank.hpp"

#include "../../pattern/BulletPattern.hpp"
#include "../../resources/ResourceManager.hpp"

using namespace LX_Random;
using namespace LX_Physics;

const Uint32 SPRITE_DISPLAY_DELAY = 125;
const int NB_SHOTS = 2;

const int XMIN = 1000;
const int X_OFFSET =  XMIN + 16;
const int YMIN = 47;
const int YMAX = 500;
const int YMIN_OFFSET = YMIN + 24;
const int YMAX_OFFSET =  YMAX - 24;
const Uint32 DELAY_TO_SHOOT = 1000;

const int SHOT1_OFFSET = 72;
const int SHOT2_OFFSET = 140;
const int BULLETX_OFFSET = 108;
const int BULLET_VELOCITY = 9;
const int HOMING_BULLET_VELOCITY = -4;


SemiBoss01::SemiBoss01(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Mixer::LX_Chunk *audio,
               int x, int y, int w, int h,float vx, float vy)
    : Boss(hp,att,sh,image,audio,x,y,w,h,vx,vy)
{
    bossInit();
}


void SemiBoss01::bossInit(void)
{
    hitbox.radius = 100;
    hitbox.square_radius = hitbox.radius*hitbox.radius;
    strat = new SemiBoss01ShootStrat(this);

    sprite[0] = {0,0,position.w,position.h};
    sprite[1] = {229,0,position.w,position.h};
    sprite[2] = {458,0,position.w,position.h};
    sprite[3] = {687,0,position.w,position.h};
    sprite[4] = {916,0,position.w,position.h};
    sprite[5] = {1145,0,position.w,position.h};
    sprite[6] = {1374,0,position.w,position.h};
}

// Direct shot from the semi-boss
void SemiBoss01::fire(void)
{
    const int SZ = 16;
    LX_Vector2D v = LX_Vector2D(HOMING_BULLET_VELOCITY,0);
    SDL_Rect rect = {position.x,(position.y + (position.w/2)),SZ,SZ};

    Game *g = Game::getInstance();
    ResourceManager *rc = ResourceManager::getInstance();

    g->acceptEnemyMissile(new BasicMissile(attack_val,
                                  rc->getResource(RC_MISSILE,PLAYER_MISSILES+4),
                                  nullptr,rect,v));
}

// Circular shot
void SemiBoss01::shoot(MISSILE_TYPE m_type)
{
    LX_Vector2D vel;
    SDL_Rect rect[NB_SHOTS];
    Game *g = Game::getInstance();

    // If the boss cannot shoot according to its position
    // Do not shoot!
    if(!canShoot())
        return;

    if(m_type == BASIC_MISSILE_TYPE)
    {
        rect[0] = {position.x,position.y + SHOT1_OFFSET,32,32};
        rect[1] = {position.x,position.y + SHOT2_OFFSET,32,32};
    }
    else if(m_type == ROCKET_TYPE)
    {
        rect[0] = {position.x + BULLETX_OFFSET,position.y + SHOT1_OFFSET,32,32};
        rect[1] = {position.x + BULLETX_OFFSET,position.y + SHOT2_OFFSET,32,32};
    }
    else if(m_type == LASER_TYPE)
    {
        if(Rank::getRank() == S_RANK)
            fire();
        return;
    }

    vel = LX_Vector2D(speed.vx,speed.vy);
    ResourceManager * rc = ResourceManager::getInstance();

    g->acceptEnemyMissile(new MegaBullet(attack_val,
                                  rc->getResource(RC_MISSILE,4),
                                  nullptr,rect[0],vel,BULLET_VELOCITY));

    g->acceptEnemyMissile(new MegaBullet(attack_val,
                                  rc->getResource(RC_MISSILE,4),
                                  nullptr,rect[1],vel,BULLET_VELOCITY));
}


bool SemiBoss01::canShoot(void)
{
    /*
        If the boss is close to a specific X position and is going to the left,
        OR if the boss is close to a specific Y maximum/minimum position
        and is going to the bottom/top of the screen, then it cannot shoot
    */

    if((position.x > XMIN && position.x < X_OFFSET && speed.vx < 0)
       || (position.y < YMAX && position.y > YMAX_OFFSET && speed.vy > 0)
       || (position.y > YMIN && position.y < YMIN_OFFSET && speed.vy < 0))
    {
        return false;
    }

    return true;
}


SDL_Rect * SemiBoss01::getAreaToDisplay()
{
    Uint32 time;

    if(!dying)
        return &sprite[6];
    else
    {
        time = SDL_GetTicks();

        if((time-sprite_ref_time) > (SPRITE_DISPLAY_DELAY*5))
        {
            sprite_ref_time = time - (SPRITE_DISPLAY_DELAY*2);
            return &sprite[5];
        }
        else if((time-sprite_ref_time) > (SPRITE_DISPLAY_DELAY*4))
        {
            return &sprite[4];
        }
        else if((time-sprite_ref_time) > (SPRITE_DISPLAY_DELAY*3))
        {
            return &sprite[3];
        }
        else if((time-sprite_ref_time) > (SPRITE_DISPLAY_DELAY*2))
        {
            return &sprite[2];
        }
        else if((time-sprite_ref_time) > (SPRITE_DISPLAY_DELAY))
            return &sprite[1];
        else
            return &sprite[0];
    }
}


SemiBoss01::~SemiBoss01()
{
    // Empty
}


void SemiBoss01::die()
{
    if(!dying)
        addStrategy(new DeathStrategy(this,DEFAULT_XPLOSION_DELAY,
                              DEFAULT_NOISE_DELAY));
    Boss::die();
}


/* Strategy */

SemiBoss01ShootStrat::SemiBoss01ShootStrat(SemiBoss01 * newEnemy)
    : Strategy(newEnemy),BossStrategy(newEnemy),fight_ref_time(SDL_GetTicks())
{
    shot_delay = DELAY_TO_SHOOT;
    begin_time = SDL_GetTicks();
}


void SemiBoss01ShootStrat::proceed()
{
    static Uint32 r_time = 0;
    unsigned int one_third_hp = target->getMaxHP()/3;
    unsigned int one_sixth_hp = one_third_hp/2;

    if(!started)
    {
        fight_ref_time = SDL_GetTicks();
        begin_time = SDL_GetTicks();
        started = true;
    }

    if((SDL_GetTicks() - fight_ref_time) < BOSS_FIGHT_DELAY)
    {
        // Only in rank B, A, S
        if((SDL_GetTicks() - r_time) > DELAY_TO_SHOOT)
        {
            fire(LASER_TYPE);
            r_time = SDL_GetTicks();
        }

        if((SDL_GetTicks() - begin_time) > shot_delay)
        {
            if(target->getHP() > (target->getMaxHP() - one_third_hp))
            {
                fire(BASIC_MISSILE_TYPE);
                begin_time = SDL_GetTicks();
            }
            else if(target->getHP() > one_third_hp)
            {
                shot_delay = DELAY_TO_SHOOT/2;
                fire(BASIC_MISSILE_TYPE);
                begin_time = SDL_GetTicks();
            }
            else if(target->getHP() > one_sixth_hp)
            {
                shot_delay = DELAY_TO_SHOOT/4;
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
    }
    else
    {
        target->setXvel(-4);
        target->setYvel(0);
    }

    target->move();
}


void SemiBoss01ShootStrat::fire(MISSILE_TYPE m_type)
{
    boss->shoot(m_type);
}


SemiBoss01ShootStrat::~SemiBoss01ShootStrat()
{
    // Empty
}

