
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

#include <iostream>
#include <cmath>
#include <LunatiX/LX_Physics.hpp>

#include "BulletPattern.hpp"
#include "../entities/Player.hpp"


static const float HALF_OF_SQRT_THREE = (sqrt(3))/2.0;
static const float ONE_HALF = 1.0/2.0;
static const float HALF_OF_SQRT_TWO = (sqrt(2))/2.0;
static const float PI12_COS = (sqrt(2) * (sqrt(3) + 1))/4.0;
static const float PI12_SIN = (sqrt(2) * (sqrt(3) - 1))/4.0;


namespace BulletPattern
{

// A private structure used in the circle pattern
typedef struct TX_Point{

    float x;
    float y;

}TX_Point;


void shotOnPlayer(const float shooter_x,const float shooter_y,const int vel,
                  LX_Physics::LX_Vector2D& v)
{
    shotOnTarget(shooter_x,shooter_y,Player::last_position.x,
                 Player::last_position.y,vel,v);
}



void shotOnTarget(const float shooter_x,const float shooter_y,
                  const float target_x,const float target_y,
                  const int vel, LX_Physics::LX_Vector2D& v)
{
    const float dx = shooter_x - target_x;
    const float dy = shooter_y - target_y;

    float tmp[2];
    float distance;

    // The distance between the shooter and the target
    distance = LX_Physics::euclide_distance(shooter_x,shooter_y,target_x,target_y);
    tmp[0] = (dx/distance)* vel;
    tmp[1] = (dy/distance)* vel;

    v.vx = tmp[0];
    v.vy = tmp[1];
}

/*
    Create the circle pattern, the circle contains CIRCLE_BULLETS bullets
    The LX_Vector2D must be an array that contains CIRCLE_BULLETS elements
*/
void circlePattern(const float pos_x,const float pos_y,const int vel,
                   LX_Physics::LX_Vector2D v[])
{
    const int OFFSET_CIRCLE = 1000;
    const int SZ = CIRCLE_BULLETS;
    const int QUARTER_CIRCLE_BULLETS = 7;
    TX_Point coordinates[SZ];

    // No angle, PI/12 PI/6, PI/2, PI/3 and PI/2 (positive and negative angle)
    coordinates[0].x = pos_x + 10;
    coordinates[0].y = pos_y;
    coordinates[1].x = pos_x + (HALF_OF_SQRT_THREE*OFFSET_CIRCLE);
    coordinates[1].y = pos_y - (ONE_HALF*OFFSET_CIRCLE);
    coordinates[2].x = pos_x + (HALF_OF_SQRT_TWO*OFFSET_CIRCLE);
    coordinates[2].y = pos_y - (HALF_OF_SQRT_TWO*OFFSET_CIRCLE);
    coordinates[3].x = pos_x + (ONE_HALF*OFFSET_CIRCLE);
    coordinates[3].y = pos_y - (HALF_OF_SQRT_THREE*OFFSET_CIRCLE);
    coordinates[4].x = pos_x;
    coordinates[4].y = pos_y - OFFSET_CIRCLE;
    coordinates[5].x = pos_x + PI12_COS;
    coordinates[5].y = pos_y - PI12_SIN;
    coordinates[6].x = pos_x + PI12_SIN;
    coordinates[6].y = pos_y - PI12_COS;

    for(int i=0;i<QUARTER_CIRCLE_BULLETS;i++)
    {
        shotOnTarget(pos_x,pos_y,coordinates[i].x,coordinates[i].y,vel,v[i]);
    }

    // Fix some position issues
    v[3].vx = -(v[1].vy);

    // Other sides of the circle
    v[7].vx = v[1].vx;
    v[7].vy = -(v[1].vy);
    v[8].vx = v[2].vx;
    v[8].vy = -(v[2].vy);
    v[9].vx = v[3].vx;
    v[9].vy = -(v[3].vy);
    v[10].vx = v[1].vx;
    v[10].vy = -(v[1].vy);
    v[11].vx = -(v[3].vx);
    v[11].vy = v[3].vy;
    v[12].vx = -(v[2].vx);
    v[12].vy = v[2].vy;
    v[13].vx = -(v[1].vx);
    v[13].vy = v[1].vy;
    v[14].vx = -(v[0].vx);
    v[14].vy = v[0].vy;
    v[15].vx = -(v[1].vx);
    v[15].vy = -(v[1].vy);
    v[16].vx = -(v[2].vx);
    v[16].vy = -(v[2].vy);
    v[17].vx = -(v[3].vx);
    v[17].vy = -(v[3].vy);
    v[18].vx = v[4].vx;
    v[18].vy = -(v[4].vy);
    v[19].vx = -(v[6].vx);
    v[19].vy = v[6].vy;
    v[20].vx = -(v[5].vx);
    v[20].vy = v[5].vy;
    v[21].vx = -(v[5].vx);
    v[21].vy = -(v[5].vy);
    v[22].vx = -(v[6].vx);
    v[22].vy = -(v[6].vy);
    v[23].vx = v[6].vx;
    v[23].vy = -(v[6].vy);
    v[24].vx = v[5].vx;
    v[24].vy = -(v[5].vy);

}


};


