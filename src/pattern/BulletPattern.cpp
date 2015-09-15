
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


namespace BulletPattern
{

// A private structure used in the circle pattern
typedef struct TX_Point{

    float x;
    float y;

}TX_Point;


void shotOnPlayer(const float shooter_x,const float shooter_y,const int vel,LX_Vector2D *v)
{
    shotOnTarget(shooter_x,shooter_y,Player::last_position.x,
                 Player::last_position.y,vel,v);
}



void shotOnTarget(const float shooter_x,const float shooter_y,
                  const float target_x,const float target_y,
                  const int vel,LX_Vector2D *v)
{
    const float dx = shooter_x - target_x;
    const float dy = shooter_y - target_y;

    float tmp[2];
    float distance;

    // The distance between the shooter and the target
    distance = LX_Physics::euclide_distance(shooter_x,shooter_y,target_x,target_y);
    tmp[0] = (dx/distance)* vel;
    tmp[1] = (dy/distance)* vel;

    v->vx = tmp[0];
    v->vy = tmp[1];
}

/**
    Take a quarter of the circle, calculate the velocity,
    and apply theses results to the other quarter with the opposite sinus.
    Take the quarter withthe opposite cosinus, calculate the velocity
*/
void circlePattern(const float pos_x,const float pos_y,const int vel,LX_Vector2D *v,const int size)
{
    const int SZ = CIRCLE_BULLETS;
    TX_Point coordinates[SZ];
    /// @todo Continue the Circle patern and add the PI/8 angle;
    /// @todo When everything are done, optimize

    // No angle, PI/6, PI/2, PI/3 and PI/2 (positive and negative angle)
    coordinates[0].x = pos_x + 100;
    coordinates[0].y = pos_y;
    coordinates[1].x = pos_x + (HALF_OF_SQRT_THREE*100);
    coordinates[1].y = pos_y - (ONE_HALF*100);
    coordinates[2].x = pos_x + (HALF_OF_SQRT_TWO*100);
    coordinates[2].y = pos_y - (HALF_OF_SQRT_TWO*100);
    coordinates[3].x = pos_x + (ONE_HALF*100);
    coordinates[3].y = pos_y - (HALF_OF_SQRT_THREE*100);
    coordinates[4].x = pos_x;
    coordinates[4].y = pos_y - 100;
    coordinates[5].x = coordinates[1].x;
    coordinates[5].y = pos_y + (ONE_HALF*100);
    coordinates[6].x = coordinates[2].x;
    coordinates[6].y = pos_y + (HALF_OF_SQRT_TWO*100);
    coordinates[7].x = coordinates[3].x;
    coordinates[7].y = pos_y + (HALF_OF_SQRT_THREE*100);
    coordinates[8].x = pos_x;
    coordinates[8].y = pos_y + 100;

    // (2*PI)/3, (3*PI)/4, (5*PI)/6 and PI (positive and negative value)
    coordinates[9].x = pos_x - (ONE_HALF*100);
    coordinates[9].y = pos_y -(HALF_OF_SQRT_THREE*100);
    coordinates[10].x = pos_x - (HALF_OF_SQRT_TWO*100);
    coordinates[10].y = pos_y - (HALF_OF_SQRT_TWO*100);
    coordinates[11].x = pos_x - (HALF_OF_SQRT_THREE*100);
    coordinates[11].y = pos_y - (ONE_HALF*100);
    coordinates[12].x = pos_x - 100;
    coordinates[12].y = pos_y;
    coordinates[13].x = pos_x - (HALF_OF_SQRT_THREE*100);
    coordinates[13].y = pos_y + (ONE_HALF*100);
    coordinates[14].x = pos_x - (HALF_OF_SQRT_TWO*100);
    coordinates[14].y = pos_y + (HALF_OF_SQRT_TWO*100);
    coordinates[15].x = pos_x - (ONE_HALF*100);
    coordinates[15].y = pos_y + (HALF_OF_SQRT_THREE*100);

    for(int i=0;i<size;i++)
    {
        shotOnTarget(pos_x,pos_y,coordinates[i].x,coordinates[i].y,vel,&v[i]);
    }

    // Fix some issues on some velocities
    v[7].vx = v[3].vx;
    v[11].vy = v[1].vy;
    v[13].vy = v[5].vy;
    v[15].vx = v[9].vx;

    /*std::cout << "13 : " << v[13].vx << " " << v[13].vy << std::endl;
    std::cout << "5 : " << v[5].vx << " " << v[5].vy << std::endl;
    std::cout << "14 : " << v[14].vx << " " << v[14].vy << std::endl;
    std::cout << "10 : " << v[10].vx << " " << v[10].vy << std::endl;
    std::cout << "15 : " << v[15].vx << " " << v[15].vy << std::endl;
    std::cout << "9 : " << v[9].vx << " " << v[9].vy << std::endl;*/
}


};












