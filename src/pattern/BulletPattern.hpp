
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

#ifndef BULLETPATTERN_HPP_INCLUDED
#define BULLETPATTERN_HPP_INCLUDED

#include <LunatiX/LX_Vector2D.hpp>

#include <cmath>
#include <array>


namespace LX_Physics
{
struct LX_Point;
}

// The number of bullets in the circle
const std::size_t CIRCLE_BULLETS = 24;

// The velocity of the bullets in the circle
const int CIRCLE_BULLETS_DEFAULT_VEL = -8;

namespace BulletPattern
{

const double PI = 3.14159265358979323846;
const float PI_F = static_cast<float>(PI);
const std::size_t WAVE_SZ = 3;

void shotOnPlayer(const float shooter_x, const float shooter_y,
                  const int vel, LX_Physics::LX_Vector2D& v);

void shotOnTarget(const float shooter_x, const float shooter_y,
                  const float target_x, const float target_y,
                  const int vel, LX_Physics::LX_Vector2D& v);

void calculateAngle(const LX_Physics::LX_Vector2D& v, double& angle);

void waveOnPlayer(const float shooter_x, const float shooter_y,
                  const int vel, std::array<LX_Physics::LX_Vector2D, 3>& varr);

/*
    Create the circle pattern, the circle contains CIRCLE_BULLETS bullets
    (default value)
*/
template<std::size_t SZ>
void circlePattern(const float pos_x, const float pos_y, const int vel,
                   std::array<LX_Physics::LX_Vector2D, SZ>& varray)
{
    const float BSTEP = PI_F/static_cast<float>(varray.size() / 2);
    const float BSR = 128.0f;
    float alpha = 0.0f;

    for(LX_Physics::LX_Vector2D& v : varray)
    {
        shotOnTarget(pos_x, pos_y, pos_x + cosf(alpha) * BSR,
                     pos_y - sinf(alpha) * BSR, vel, v);
        alpha += BSTEP;
    }
}

}

#endif // BULLETPATTERN_HPP_INCLUDED
