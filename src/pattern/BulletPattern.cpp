
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
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "BulletPattern.hpp"
#include "../entities/Player.hpp"
#include "../entities/PlayerVisitor.hpp"


using namespace lx::Physics;
using namespace FloatBox;


namespace BulletPattern
{

void shotOnPlayer( const Float& shooter_x, const Float& shooter_y,
                   const Float& vel, lx::Physics::Vector2D& v ) noexcept
{
    PlayerVisitor pv;
    Player::accept( pv );
    shotOnTarget( shooter_x, shooter_y, pv.getLastX(), pv.getLastY(), vel, v );
}

void shotOnTarget( const Float& shooter_x, const Float& shooter_y,
                   const Float& target_x, const Float& target_y,
                   const Float& vel, lx::Physics::Vector2D& v ) noexcept
{
    const Float& dx = shooter_x - target_x;
    const Float& dy = shooter_y - target_y;
    Float square_distance = ( dx * dx ) + ( dy * dy );
    const Float& distance = FloatMath::sqrt( square_distance );

    v.vx = ( dx / distance ) * vel;
    v.vy = ( dy / distance ) * vel;
}


void waveOnPlayer( const Float& shooter_x, const Float& shooter_y, const Float& vel,
                   std::array<lx::Physics::Vector2D, WAVE_SZ>& varr ) noexcept
{
    const Float HALF = fbox( 0.5f );
    BulletPattern::shotOnPlayer( shooter_x, shooter_y, vel, varr[0] );

    // Change the y speed to get a spread shot
    varr[1] = varr[0];
    varr[2] = varr[0];
    varr[1].vx -= HALF;
    varr[2].vx -= HALF;
    varr[1].vy += HALF;
    varr[2].vy -= HALF;

    // Normalize the two vectors
    normalize( varr[1] );
    normalize( varr[2] );
    varr[1] *= -vel;
    varr[2] *= -vel;
}

// Calculate the angle of rotation of a bullet
void calculateAngle( const lx::Physics::Vector2D& v, double& angle ) noexcept
{
    if ( v.vx == FNIL )
    {
        const double pi_2 = PI / 2.0;
        angle = v.vy > FNIL ? -pi_2 : pi_2;
    }
    else
    {
        Float tan_alpha = v.vy / v.vx;
        Float alpha;

        if ( tan_alpha == FNIL )
            alpha = v.vx > FNIL ? FNIL : PI_F;
        else
            alpha = FloatMath::atan( tan_alpha );

        angle = static_cast<double>( -alpha );
    }
}

/**********************************
    Spin Bullet (Implementation)
***********************************/

// Abstract class
const Float AbstractSpin::R_UNIT = fbox( 100.0f );


// SpinShot

const Float SpinShot::PI_2 = PI_F * Float{2.0f};

SpinShot::SpinShot( const Float& speed, const Float& a_step, const Float& start )
    : alpha_start( start ), vel( speed )
{
    alpha = alpha_start;
    alpha_step = a_step;
}

void SpinShot::operator ()( const Float& X, const Float& Y,
                            lx::Physics::Vector2D& v ) noexcept
{
    shotOnTarget( X, Y, X + FloatMath::cos( alpha ) * R_UNIT,
                  Y - FloatMath::sin( alpha ) * R_UNIT, vel, v );

    if ( alpha == SpinShot::PI_2 )
        alpha = FNIL;

    alpha += alpha_step;
}


// RevSpinShot

RevSpinShot::RevSpinShot( const Float& speed, const Float& a_step, const Float& start )
    : SpinShot( speed, a_step, start )
{
    alpha_step = a_step;
}

void RevSpinShot::operator ()( const Float& X, const Float& Y,
                               lx::Physics::Vector2D& v ) noexcept
{
    shotOnTarget( X, Y, X + FloatMath::cos( alpha ) * R_UNIT,
                  Y - FloatMath::sin( alpha ) * R_UNIT, vel, v );

    if ( alpha == -PI_2 )
        alpha = FNIL;

    alpha -= alpha_step;
}


// DoubleSpinShot
DoubleSpinShot::DoubleSpinShot( const Float& speed, const Float& a_step,
                                const Float& start1, const Float& start2 )
    : spshot( speed, a_step, start1 ), rev_spshot( speed, a_step, start2 ) {}

void DoubleSpinShot::operator ()( const Float& X, const Float& Y,
                                  std::array<lx::Physics::Vector2D, DOUBLE_SPIN>& v ) noexcept
{
    spshot( X, Y, v[0] );
    rev_spshot( X, Y, v[1] );
}

}
