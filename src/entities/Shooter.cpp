
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
#include "../game/Rank.hpp"

#include "../xml/XMLReader.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../resources/ResourceManager.hpp"

using namespace LX_Physics;
using namespace LX_Graphics;

static const int SHOOTER_BULLET_VEL = -8;


Shooter::Shooter(unsigned int hp, unsigned int att, unsigned int sh,
                 SDL_Texture *image, LX_Mixer::LX_Chunk *audio,
                 Sint16 x, Sint16 y, Uint16 w, Uint16 h,float vx, float vy)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,vx,vy)
{
    strat = new BasicStrategy(this);
}


void Shooter::shoot()
{
    const int N = 4;
    const int MIN_VEL = 3;
    SDL_Rect rect = {position.x, position.y + ( (position.h - MISSILE_HEIGHT)/ 2),24,24};

    // Shoot the player only if he can be seen
    if(Player::last_position.x + PLAYER_WIDTH < position.x)
    {
        LX_Vector2D v[N];
        unsigned int rank = Rank::getRank();
        Game *g = Game::getInstance();
        ResourceManager *rc = ResourceManager::getInstance();
        //SDL_Surface *surface = Enemy::getResources()[0];

        for(unsigned int i = 0; i<= rank; i++)
        {
            BulletPattern::shotOnPlayer(position.x,position.y,
                                        SHOOTER_BULLET_VEL-(i*MIN_VEL),v[i]);
            g->addEnemyMissile(new BasicMissile(attack_val,
                                                rc->getResource(RC_MISSILE,PLAYER_MISSILES+0),
                                                nullptr,rect,v[i]));
        }
    }
}


Shooter::~Shooter()
{
    //Empty
}

