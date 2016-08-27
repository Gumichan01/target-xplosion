
/*
*   Target_Xplosion - A classic shoot'em up video game
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
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file Tower.hpp
*	@brief The Towwer(s) implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <SDL2/SDL_timer.h>
#include <LunatiX/LX_Hitbox.hpp>

#include "Tower.hpp"
#include "Bullet.hpp"

#include "../game/Game.hpp"
#include "../game/Rank.hpp"

#include "../asset/TX_Asset.hpp"
#include "../resources/ResourceManager.hpp"


static const Uint32 DELAY_TOWER = 500;


Tower1::Tower1(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Image *image, LX_Mixer::LX_Sound *audio,
               Sint16 x, Sint16 y, Uint16 w, Uint16 h,float vx, float vy)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,vx,vy)
{
    strat = new Tower1Strat(this);
}


void Tower1::fire(void)
{
    static const int BULLET_VEL = -7;
    static const int N = 9;

    SDL_Rect rect[2] = {{position.x,position.y+125,24,24},
        {position.x,position.y+160,24,24}
    };

    if(isDead())
        return;

    LX_Physics::LX_Vector2D velocity[] = {{BULLET_VEL,0},{BULLET_VEL,-1},{BULLET_VEL,1},
        {BULLET_VEL,-2},{BULLET_VEL,2},{BULLET_VEL,-3},{BULLET_VEL,3},
        {BULLET_VEL,-4},{BULLET_VEL,4}
    };

    Game *g = Game::getInstance();
    ResourceManager *rc = ResourceManager::getInstance();

    for(int i = 0; i < N; i++)
    {
        g->acceptEnemyMissile(new Bullet(attack_val,rc->getResource(RC_MISSILE,4),
                                         nullptr,rect[0],velocity[i]));

        if(Rank::getRank() == S_RANK)
        {
            g->acceptEnemyMissile(new Bullet(attack_val,rc->getResource(RC_MISSILE,4),
                                             nullptr,rect[1],velocity[i]));
        }
    }
}


void Tower1::die()
{
    if((position.x + position.w) > 0)
        Game::getInstance()->screenCancel();

    Entity::die();
}


Tower1::~Tower1() {}


Tower1Strat::Tower1Strat(Enemy *newEnemy)
    : Strategy(newEnemy)
{
    reference_time = 0;
}


Tower1Strat::~Tower1Strat() {}


void Tower1Strat::proceed(void)
{
    if((SDL_GetTicks() - reference_time) > DELAY_TOWER)
    {
        fire(ROCKET_TYPE);
        reference_time = SDL_GetTicks();
    }
    target->move();
}


void Tower1Strat::fire(MISSILE_TYPE m_type)
{
    if(m_type == ROCKET_TYPE)
    {
        target->fire();
    }
}
