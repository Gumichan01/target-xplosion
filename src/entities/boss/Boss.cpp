
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

Boss::Boss(unsigned int hp, unsigned int att, unsigned int sh,
           SDL_Texture *image, LX_Chunk *audio,
           Sint16 x, Sint16 y, Uint16 w, Uint16 h,int dX, int dY)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,dX,dY)
{
    // Empty
}


Boss::Boss(unsigned int hp, unsigned int att, unsigned int sh,
           SDL_Texture *image, LX_Chunk *audio,SDL_Rect& rect, LX_Vector2D& sp)
    : Enemy(hp,att,sh,image,audio,rect,sp)
{
    // Empty
}


Boss::~Boss()
{
    // Empty
}


void Boss::bossInit(void)
{
    strat = NULL;
    dying = false;
}


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
    // Screen cancel
    Game::getInstance()->screenCancel();

    // The boss ids dying
    dying = true;
    sound->play();

    // Update these variables, it is necessary
    // because the boss need it when it dies
    begin_die = SDL_GetTicks();
    noise_time = SDL_GetTicks();
    ref_time = SDL_GetTicks();
}

// Noise of explosion end death
void Boss::die(Uint32 sprite_display_delay,Uint32 explosion_delay)
{
    // Explosion noise during sprite_display_delay seconds (the total delay)
    // explosion_delay is the delay of each explosion sound
    if((SDL_GetTicks()-noise_time) < DELAY_NOISE
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
void Boss::bossMustDie(void)
{
    was_killed = true;   // It was set to false, so set it to true
    Entity::die();
    Game::getInstance()->getScore()->notify(max_health_point*2);
}

