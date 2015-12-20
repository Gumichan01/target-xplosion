
/*
*	Target_Xplosion - The classic shoot'em up video game
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
*	@file BasicEnemy.cpp
*	@brief The basic enemy implementaion
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Hitbox.hpp>

#include "BasicEnemy.hpp"
#include "Missile.hpp"
#include "../game/Game.hpp"
#include "../xml/XMLReader.hpp"

using namespace LX_Graphics;
using namespace LX_Physics;

BasicEnemy::BasicEnemy(unsigned int hp, unsigned int att, unsigned int sh,
                       SDL_Texture *image, LX_Mixer::LX_Chunk *audio,
                       Sint16 x, Sint16 y, Uint16 w, Uint16 h,float vx, float vy)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,vx,vy)
{
    strat = new BasicStrategy(this);
}

BasicEnemy::~BasicEnemy()
{
    // Empty
}


void BasicEnemy::shoot()
{
    Enemy::shoot();
}
