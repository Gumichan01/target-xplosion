#ifndef BOSS_HPP_INCLUDED
#define BOSS_HPP_INCLUDED


/*
*   Target_Xplosion - The classic shoot'em up video game
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

const float XVEL_DIE = -1.0f;
const float YVEL_DIE = 1.0f;
const Uint32 BOSS_FIGHT_DELAY = 45000;

// Boss is the mother class of all bosses defined in the game
// A boss is an enemy
class Boss : public Enemy
{
protected:

    static const Uint32 DEFAULT_XPLOSION_DELAY = 4000;
    static const Uint32 DEFAULT_NOISE_DELAY = 250;

    bool dying;                     // If the boss is dying
    Uint32 sprite_ref_time;         // Reference time of explosion

    virtual void bossInit(void) = 0;
    void die();

public:

    explicit Boss(unsigned int hp, unsigned int att, unsigned int sh,
                  SDL_Texture *image, LX_Mixer::LX_Chunk *audio,
                  int x, int y, int w, int h,float vx, float vy);

    virtual void shoot(MISSILE_TYPE m_type) = 0;
    void boom();
    void reaction(Missile *target);
    virtual ~Boss();
};

// Boss
class BossStrategy: virtual public Strategy
{
protected:

    Boss * boss;
    virtual void fire(MISSILE_TYPE m_type) = 0;

public :

    explicit BossStrategy(Boss *newBoss);
    void proceed(void) = 0;
    ~BossStrategy();
};

#endif // BOSS_HPP_INCLUDED
