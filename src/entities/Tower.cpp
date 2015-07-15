
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
*	@file Tower.hpp
*	@brief The Towwer(s) implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <LunatiX/LX_Graphics.hpp>

#include "Tower.hpp"
#include "Bullet.hpp"
#include "../game/Game.hpp"


static const int DELAY_TOWER = 500;


Tower1::Tower1(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Chunk *audio,
               Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,dX,dY)
{
    strat = new Tower1_Strat(this);
}


Tower1::Tower1(unsigned int hp, unsigned int att, unsigned int sh,
               SDL_Texture *image, LX_Chunk *audio, SDL_Rect *rect,LX_Vector2D *sp)
    : Enemy(hp,att,sh,image,audio,rect,sp)
{
    strat = new Tower1_Strat(this);
}


Missile * Tower1::shoot(MISSILE_TYPE m_type)
{
    return NULL;
}


void Tower1::init_box(void)
{
    box = {117,171,100,100*100};
}


Tower1::~Tower1() {}


Tower1_Strat::Tower1_Strat(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    rtime = 0.0;
}


Tower1_Strat::~Tower1_Strat() {}


void Tower1_Strat::proceed(void)
{
    if((SDL_GetTicks() - rtime) > DELAY_TOWER)
    {
        fire(ROCKET_TYPE);
        rtime = SDL_GetTicks();
    }
    target->move();
}


void Tower1_Strat::fire(MISSILE_TYPE m_type)
{
    static const int v = -11;
    SDL_Rect rect1 = {target->getX()+26,target->getY()+142,24,24};
    SDL_Rect rect2 = {target->getX()+26,target->getY()+130,24,24};
    SDL_Rect rect3 = {target->getX()+26,target->getY()+171,24,24};

    LX_Vector2D v1 = {v,0};
    LX_Vector2D v2 = {v,-1};
    LX_Vector2D v3 = {v,1};
    LX_Vector2D v4 = {v,-2};
    LX_Vector2D v5 = {v,2};
    LX_Vector2D v6 = {v,-3};
    LX_Vector2D v7 = {v,3};
    LX_Vector2D v8 = {v,-4};
    LX_Vector2D v9 = {v,4};

    Game *g = Game::getInstance();

    if(m_type == ROCKET_TYPE)
    {
        g->addEnemyMissile(new Bullet(target->getATT(),
                                      LX_Graphics::loadTextureFromFile("image/light_bullet.png",0),
                                      NULL,&rect1,&v1));
        g->addEnemyMissile(new Bullet(target->getATT(),
                                      LX_Graphics::loadTextureFromFile("image/light_bullet.png",0),
                                      NULL,&rect2,&v2));
        g->addEnemyMissile(new Bullet(target->getATT(),
                                      LX_Graphics::loadTextureFromFile("image/light_bullet.png",0),
                                      NULL,&rect3,&v3));
        g->addEnemyMissile(new Bullet(target->getATT(),
                                      LX_Graphics::loadTextureFromFile("image/light_bullet.png",0),
                                      NULL,&rect2,&v4));
        g->addEnemyMissile(new Bullet(target->getATT(),
                                      LX_Graphics::loadTextureFromFile("image/light_bullet.png",0),
                                      NULL,&rect3,&v5));
        g->addEnemyMissile(new Bullet(target->getATT(),
                                      LX_Graphics::loadTextureFromFile("image/light_bullet.png",0),
                                      NULL,&rect2,&v6));
        g->addEnemyMissile(new Bullet(target->getATT(),
                                      LX_Graphics::loadTextureFromFile("image/light_bullet.png",0),
                                      NULL,&rect3,&v7));

        g->addEnemyMissile(new Bullet(target->getATT(),
                                      LX_Graphics::loadTextureFromFile("image/light_bullet.png",0),
                                      NULL,&rect2,&v8));

        g->addEnemyMissile(new Bullet(target->getATT(),
                                      LX_Graphics::loadTextureFromFile("image/light_bullet.png",0),
                                      NULL,&rect3,&v9));
    }
}


