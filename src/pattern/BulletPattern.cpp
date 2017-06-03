
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017  Luxon Jean-Pierre
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "BulletPattern.hpp"
#include "Angle.hpp"
#include "../entities/Player.hpp"
#include "../entities/PlayerVisitor.hpp"

using namespace LX_Physics;

namespace BulletPattern
{

void shotOnPlayer(const float shooter_x, const float shooter_y,
                  const int vel, LX_Vector2D& v)
{
    PlayerVisitor pv;
    Player::accept(&pv);
    shotOnTarget(shooter_x, shooter_y, pv.getLastX(), pv.getLastY(), vel, v);
}


void shotOnTarget(const float shooter_x, const float shooter_y,
                  const float target_x, const float target_y,
                  const int vel, LX_Vector2D& v)
{
    float tmp[2];
    const float dx = shooter_x - target_x;
    const float dy = shooter_y - target_y;
    const float distance = sqrtf(dx*dx + dy*dy);

    tmp[0] = (dx/distance) * vel;
    tmp[1] = (dy/distance) * vel;

    v.vx = tmp[0];
    v.vy = tmp[1];
}



// Calculate the angle of rotation of a bullet
void calculateAngle(const LX_Physics::LX_Vector2D& v, double& angle)
{
    if(v.vx == 0.0f)
    {
        const double pi_2 = PI / 2.0;
        angle = v.vy > 0.0f ? -pi_2 : pi_2;
    }
    else
    {
        const float tan_alpha = v.vy / v.vx;
        float alpha;

        if(tan_alpha == 0.0f)
            alpha = v.vx > 0.0f ? 0.0f : PI;
        else
            alpha = atanf(tan_alpha);

        angle = static_cast<double>(-alpha);
    }
}

}
