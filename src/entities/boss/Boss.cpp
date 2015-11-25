
/*
*   Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2015  Luxon Jean-Pierre
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
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file Boss.cpp
*	@brief The implementation of the Boss Interface
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Boss.hpp"
#include "../../game/Game.hpp"

using namespace LX_Physics;

const float XPLOSION_DELAY = 4000.0f;
const float NOISE_DELAY = 3256.0f;


Boss::Boss(unsigned int hp, unsigned int att, unsigned int sh,
           SDL_Texture *image, LX_Chunk *audio,
           Sint16 x, Sint16 y, Uint16 w, Uint16 h,float vx, float vy)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,vx,vy)
{
    // Empty
}



Boss::~Boss()
{
    // Empty
}


void Boss::bossInit(void)
{
    strat = nullptr;
    dying = false;
}

void Boss::reaction(Missile *target)
{
    if(!dying)
        Enemy::reaction(target);
}

///@deprecated This function will be removed
void Boss::bossReaction(void)
{
    if(health_point == 0)
    {
        // Cancel the kill, the boss will die
        // With an explosion
        was_killed = false;
    }
}

// It is time to die
void Boss::die()
{
    // The boss is dying
    if(!dying)
    {
        // The boss will die
        Game::getInstance()->screenCancel();

        speed = LX_Vector2D(XVEL_DIE,YVEL_DIE);
        dying = true;
        addStrategy(new DeathStrategy(this,XPLOSION_DELAY,NOISE_DELAY));
        sprite_ref_time = SDL_GetTicks();
    }
    else
    {
        // It is dead
        // Give points to the player
        Entity::die();
        Game::getInstance()->getScore()->notify(max_health_point*2);
    }
}

// Noise of explosion end death
///@deprecated This function will not be useful
void Boss::die(Uint32 sprite_display_delay,Uint32 explosion_delay)
{
    // Explosion noise during sprite_display_delay seconds (the total delay)
    // explosion_delay is the delay of each explosion sound
    if((SDL_GetTicks()-noise_time) < NOISE_DELAY
            && (SDL_GetTicks()-xtime) > (sprite_display_delay))
    {
        sound->play();
        xtime = SDL_GetTicks();
    }

    // Explosion animation during explosion_delay ms
    if((SDL_GetTicks() - begin_die) > explosion_delay)
            bossMustDie();
}


// The boss is dead
///@deprecated This function will be useless
void Boss::bossMustDie(void)
{
    was_killed = true;   // It was set to false, so set it to true
    Entity::die();
    Game::getInstance()->getScore()->notify(max_health_point*2);
}

