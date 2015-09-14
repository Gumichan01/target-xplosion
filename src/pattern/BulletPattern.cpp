
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
*	@file BulletPattern.hpp
*	@brief The bullet patterns file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <cmath>
#include <LunatiX/LX_Physics.hpp>

#include "BulletPattern.hpp"
#include "../entities/Player.hpp"

static const float HALF_SQRT_THREE = (sqrt(3))/2.0;
static const float ONE_HALF = 1.0/2.0;


namespace BulletPattern
{

// A private structure used in the circle pattern
typedef struct TX_Point{

    float x;
    float y;

}TX_Point;


void shotOnTarget(const float pos_x,const float pos_y,const int vel,LX_Vector2D *v)
{
    const float player_x = Player::last_position.x;
    const float player_y = Player::last_position.y;
    const float dx = pos_x - player_x;
    const float dy = pos_y - player_y;

    float tmp[2];
    float distance;

    // The distance between the shooter and the player
    distance = LX_Physics::euclide_distance(pos_x,pos_y,player_x,player_y);
    tmp[0] = (dx/distance)* vel;
    tmp[1] = (dy/distance)* vel;

    v->vx = tmp[0];
    v->vy = tmp[1];
}


void circlePattern(const float pos_x,const float pos_y,const int vel,LX_Vector2D *v, int size)
{
    TX_Point coordinates[2];

    coordinates[0].x = pos_x + 100;
    coordinates[0].y = pos_y + 100;
    coordinates[1].x = pos_x + (HALF_SQRT_THREE*100);
    coordinates[1].y = pos_y + (ONE_HALF*100);

    for(int i=0;i<2;i++)
    {
        shotOnTarget(coordinates[i].x,coordinates[i].y,vel,&v[i]);
    }
}


};












