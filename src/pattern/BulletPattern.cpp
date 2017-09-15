
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
                  const float vel, LX_Vector2D& v)
{
    PlayerVisitor pv;
    Player::accept(&pv);
    shotOnTarget(shooter_x, shooter_y, pv.getLastX(), pv.getLastY(), vel, v);
}

void shotOnTarget(const float shooter_x, const float shooter_y,
                  const float target_x, const float target_y,
                  const float vel, LX_Vector2D& v)
{
    const float dx = shooter_x - target_x;
    const float dy = shooter_y - target_y;
    const float distance = sqrtf(dx*dx + dy*dy);

    v.vx = (dx/distance) * vel;
    v.vy = (dy/distance) * vel;
}


void waveOnPlayer(const float shooter_x, const float shooter_y, const float vel,
                  std::array<LX_Physics::LX_Vector2D, WAVE_SZ>& varr)
{
    BulletPattern::shotOnPlayer(shooter_x, shooter_y, vel, varr[0]);

    // Change the y speed to get a spread shot
    varr[1] = varr[0];
    varr[2] = varr[0];
    varr[1].vx -= 0.5f;
    varr[2].vx -= 0.5f;
    varr[1].vy += 0.5f;
    varr[2].vy -= 0.5f;

    // Normalize the two vectors
    normalize(varr[1]);
    normalize(varr[2]);
    multiply(varr[1], -vel);
    multiply(varr[2], -vel);
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
            alpha = v.vx > 0.0f ? 0.0f : PI_F;
        else
            alpha = atanf(tan_alpha);

        angle = static_cast<double>(-alpha);
    }
}

/**********************************
    Spin Bullet (Implementation)
***********************************/

// Abstract class

const float AbstractSpin::R_UNIT = 100.0f;

AbstractSpin::AbstractSpin(): alpha(0.0f), alpha_step(0.0f) {}
AbstractSpin::~AbstractSpin() {}


// SpinShot

const float SpinShot::PI_2 = PI_F * 2.0f;

SpinShot::SpinShot(int speed, float a_step, float start)
    : alpha_start(start), vel(speed)
{
    alpha = alpha_start;
    alpha_step = a_step;
}

void SpinShot::operator ()(int x_src, int y_src, LX_Physics::LX_Vector2D& v)
{
    shotOnTarget(x_src, y_src, FLA(x_src) + cosf(alpha) * R_UNIT,
                 FLA(y_src) - sinf(alpha) * R_UNIT, vel, v);

    if(alpha == PI_2)
        alpha = alpha_start;
    else
        alpha += alpha_step;
}


// RevSpinShot

RevSpinShot::RevSpinShot(int speed, float a_step, float start)
    : SpinShot(speed, a_step, start)
{
    alpha_step = a_step;
}

void RevSpinShot::operator ()(int x_src, int y_src, LX_Physics::LX_Vector2D& v)
{
    shotOnTarget(x_src, y_src, FLA(x_src) + cosf(alpha) * R_UNIT,
                 FLA(y_src) - sinf(alpha) * R_UNIT, vel, v);

    if(alpha == -PI_2)
        alpha = alpha_start;
    else
        alpha -= alpha_step;
}


// DoubleSpinShot
DoubleSpinShot::DoubleSpinShot(int speed, float a_step,
                               float start1, float start2)
    : spshot(speed, a_step, start1), rev_spshot(speed, a_step, start2) {}

void DoubleSpinShot::operator ()(int x_src, int y_src,
                                 std::array<LX_Physics::LX_Vector2D, DOUBLE_SPIN>& v)
{
    spshot(x_src, y_src, v[0]);
    rev_spshot(x_src, y_src, v[1]);
}

}
