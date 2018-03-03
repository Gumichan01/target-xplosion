
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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
#include "../entities/Player.hpp"
#include "../entities/PlayerVisitor.hpp"


using namespace LX_Physics;

/// @todo Float (lunatix 0.13.0)

namespace BulletPattern
{

void shotOnPlayer(const Float& shooter_x, const Float& shooter_y,
                  const Float& vel, LX_Vector2D& v) noexcept
{
    PlayerVisitor pv;
    Player::accept(&pv);
    shotOnTarget(shooter_x, shooter_y, pv.getLastX(), pv.getLastY(), vel, v);
}

void shotOnTarget(const Float& shooter_x, const Float& shooter_y,
                  const Float& target_x, const Float& target_y,
                  const Float& vel, LX_Physics::LX_Vector2D& v) noexcept
{
    const Float dx = shooter_x - target_x;
    const Float dy = shooter_y - target_y;
    const Float distance{std::sqrt(dx.v * dx.v + dy.v *dy.v)};

    v.vx = (dx / distance) * vel;
    v.vy = (dy / distance) * vel;
}


void waveOnPlayer(const Float& shooter_x, const Float& shooter_y, const Float& vel,
                  std::array<LX_Physics::LX_Vector2D, WAVE_SZ>& varr) noexcept
{
    BulletPattern::shotOnPlayer(shooter_x, shooter_y, vel, varr[0]);
    const Float HALF{0.5f};
    // Change the y speed to get a spread shot
    varr[1] = varr[0];
    varr[2] = varr[0];
    varr[1].vx -= HALF;
    varr[2].vx -= HALF;
    varr[1].vy += HALF;
    varr[2].vy -= HALF;

    // Normalize the two vectors
    normalize(varr[1]);
    normalize(varr[2]);
    varr[1] *= -vel;
    varr[2] *= -vel;
    //multiply(varr[1], -vel);
    //multiply(varr[2], -vel);
}

// Calculate the angle of rotation of a bullet
void calculateAngle(const LX_Physics::LX_Vector2D& v, double& angle) noexcept
{
    const Float ZERO{0.0f};

    if(v.vx == ZERO)
    {
        const double pi_2 = PI / 2.0;
        angle = v.vy > ZERO ? -pi_2 : pi_2;
    }
    else
    {
        const Float tan_alpha = v.vy / v.vx;
        Float alpha;

        if(tan_alpha == ZERO)
            alpha = v.vx > ZERO ? ZERO : PI_F;
        else
            alpha = Float{std::atan(tan_alpha.v)};

        angle = static_cast<double>(-alpha);
    }
}

/**********************************
    Spin Bullet (Implementation)
***********************************/

// Abstract class
const Float AbstractSpin::R_UNIT = {100.0f};


// SpinShot

const Float SpinShot::PI_2 = PI_F * Float{2.0f};

SpinShot::SpinShot(const Float& speed, const Float& a_step, const Float& start)
    : alpha_start(start), vel(speed)
{
    alpha = alpha_start;
    alpha_step = a_step;
}

void SpinShot::operator ()(const Float& X, const Float& Y,
                           LX_Physics::LX_Vector2D& v) noexcept
{
    shotOnTarget(X, Y, X + Float{std::cos(alpha.v)} * R_UNIT,
                 Y - Float{std::sin(alpha.v)} * R_UNIT, vel, v);

    if(alpha == SpinShot::PI_2)
        alpha = alpha_start;
    else
        alpha += alpha_step;
}


// RevSpinShot

RevSpinShot::RevSpinShot(const Float& speed, const Float& a_step, const Float& start)
    : SpinShot(speed, a_step, start)
{
    alpha_step = a_step;
}

void RevSpinShot::operator ()(const Float& X, const Float& Y,
                              LX_Physics::LX_Vector2D& v) noexcept
{
    shotOnTarget(X, Y, X + Float{std::cos(alpha.v)} * R_UNIT,
                 Y - Float{std::sin(alpha.v)} * R_UNIT, vel, v);

    if(alpha == -PI_2)
        alpha = alpha_start;
    else
        alpha -= alpha_step;
}


// DoubleSpinShot
DoubleSpinShot::DoubleSpinShot(const Float& speed, const Float& a_step,
                               const Float& start1, const Float& start2)
    : spshot(speed, a_step, start1), rev_spshot(speed, a_step, start2) {}

void DoubleSpinShot::operator ()(const Float& X, const Float& Y,
                                 std::array<LX_Physics::LX_Vector2D, DOUBLE_SPIN>& v) noexcept
{
    spshot(X, Y, v[0]);
    rev_spshot(X, Y, v[1]);
}

}
