
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
*	@file Shooter.cpp
*	@brief The Shooter implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL_rect.h>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_FileBuffer.hpp>
#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Hitbox.hpp>

#include "Shooter.hpp"
#include "Player.hpp"
#include "BasicMissile.hpp"
#include "Bullet.hpp"
#include "../pattern/Strategy.hpp"

#include "../game/Game.hpp"
#include "../pattern/BulletPattern.hpp"

Shooter::Shooter(unsigned int hp, unsigned int att, unsigned int sh,
                 SDL_Texture *image, LX_Chunk *audio,
                 Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,dX,dY)
{
    strat = new BasicStrategy(this);
}


Shooter::Shooter(unsigned int hp, unsigned int att, unsigned int sh,
                 SDL_Texture *image, LX_Chunk *audio, SDL_Rect *rect,LX_Vector2D *sp)
    : Enemy(hp,att,sh,image,audio,rect,sp)
{
    strat = new BasicStrategy(this);
}


Missile * Shooter::shoot(MISSILE_TYPE m_type)
{
    LX_Vector2D v;
    SDL_Rect rect;
    SDL_Surface * surface = NULL;
    Game *g = Game::getInstance();

    rect = {position.x, position.y + ( (position.h - MISSILE_HEIGHT)/ 2),24,24};

    // Shoot the player only if he can be seen
    if(Player::last_position.x < position.x)
    {
        BulletPattern::shotOnPlayer(position.x,position.y,-16,&v);

        surface = LX_Graphics::loadSurfaceFromFileBuffer(Bullet::getRedBulletBuffer());

        g->addEnemyMissile(new BasicMissile(attack_val,
                                            LX_Graphics::loadTextureFromSurface(surface),
                                            NULL,&rect,&v));

        SDL_FreeSurface(surface);
    }
    return NULL;
}


Shooter::~Shooter()
{
    //dtor
}


