#ifndef BOSS_HPP_INCLUDED
#define BOSS_HPP_INCLUDED


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
*	@file Boss.hpp
*	@brief The header of the Boss Interface
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <LunatiX/LX_Chunk.hpp>

#include <SDL2/SDL_timer.h>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Hitbox.hpp>

#include "../Enemy.hpp"


using namespace LX_Mixer;

const float XVEL_DIE = -1.0f;
const float YVEL_DIE = 1.0f;


// Boss is the mother class of all bosses defined in the game
// A boss is an enemy
class Boss : public Enemy
{
protected:

    bool dying;                     // If the boss is dying
    Uint32 sprite_ref_time;         // Reference time of explosion

    virtual void bossInit(void) = 0;
    void die();

public:

    Boss(unsigned int hp, unsigned int att, unsigned int sh,
         SDL_Texture *image, LX_Chunk *audio,
         Sint16 x, Sint16 y, Uint16 w, Uint16 h,float vx, float vy);

    void reaction(Missile *target);
    virtual ~Boss();
};

#endif // BOSS_HPP_INCLUDED
