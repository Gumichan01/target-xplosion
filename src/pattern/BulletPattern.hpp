
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


namespace BulletPattern
{

#define FLA(x) static_cast<float>(x)

// The number of bullets in the circle
constexpr std::size_t CIRCLE_BULLETS = 24;

// The velocity of the bullets in the circle
constexpr int CIRCLE_BULLETS_DEFAULT_VEL = -8;

constexpr double PI = 3.14159265358979323846;
constexpr float PI_F = static_cast<float>(PI);
const std::size_t WAVE_SZ = 3;
const std::size_t DOUBLE_SPIN = 2;

void shotOnPlayer(const float shooter_x, const float shooter_y,
                  const float vel, LX_Physics::LX_Vector2D& v);

void shotOnTarget(const float shooter_x, const float shooter_y,
                  const float target_x, const float target_y,
                  const float vel, LX_Physics::LX_Vector2D& v);

// Angle of display of an object
void calculateAngle(const LX_Physics::LX_Vector2D& v, double& angle);

// Shoot a wave of 3 Bullets on the player
void waveOnPlayer(const float shooter_x, const float shooter_y, const float vel,
                  std::array<LX_Physics::LX_Vector2D, WAVE_SZ>& varr);

/*
    Create the circle pattern
*/
template<std::size_t SZ>
void circlePattern(const float pos_x, const float pos_y, const float vel,
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

/*****************
    Spin Bullet
******************/

class AbstractSpin
{
    AbstractSpin(const AbstractSpin&);
    AbstractSpin& operator =(const AbstractSpin&);

protected:

    static const float R_UNIT;

    float alpha = 0.0f;
    float alpha_step = 0.0f;

public:
    AbstractSpin() = default;
    virtual ~AbstractSpin() = default;
};


class SpinShot: public AbstractSpin
{
    SpinShot(const SpinShot&);
    SpinShot& operator =(const SpinShot&);

protected:

    static const float PI_2;
    float alpha_start;
    int vel;

public:
    SpinShot(int speed, float a_step, float start = 0.0f);
    virtual void operator ()(int x_src, int y_src, LX_Physics::LX_Vector2D& v);
    virtual ~SpinShot() = default;
};

class RevSpinShot: public SpinShot
{
    RevSpinShot(const RevSpinShot&);
    RevSpinShot& operator =(const RevSpinShot&);

public:
    RevSpinShot(int speed, float a_step, float start = 0.0f);
    virtual void operator ()(int x_src, int y_src, LX_Physics::LX_Vector2D& v);
    virtual ~RevSpinShot() = default;
};

// SpinShot and RevSpinShot in one function objet
class DoubleSpinShot: public AbstractSpin
{
    SpinShot spshot;
    RevSpinShot rev_spshot;

    DoubleSpinShot(const DoubleSpinShot&);
    DoubleSpinShot& operator =(const DoubleSpinShot&);

public:
    DoubleSpinShot(int speed, float a_step, float start1 = 0.0f, float start2 = 0.0f);
    void operator ()(int x_src, int y_src,
                     std::array<LX_Physics::LX_Vector2D, DOUBLE_SPIN>& v);
    virtual ~DoubleSpinShot() = default;
};

template<std::size_t SZ>
void initialize_array(int speed, float step, std::array<SpinShot*, SZ>& varray, bool rev = false)
{
    varray.fill(nullptr);
    const float PARTS = FLA(varray.size()) / 2.0f;

    for(std::size_t i = 0; i < varray.size(); ++i)
    {
        if(rev)
            varray[i] = new RevSpinShot(speed, step, FLA(i) * BulletPattern::PI_F/PARTS);
        else
            varray[i] = new SpinShot(speed, step, FLA(i) * BulletPattern::PI_F/PARTS);
    }
}

template<std::size_t SZ>
void destroy_array(std::array<SpinShot*, SZ>& varray)
{
    for(std::size_t i = 0; i < varray.size(); ++i)
    {
        delete varray[i];
        varray[i] = nullptr;
    }
}

}

#endif // BULLETPATTERN_HPP_INCLUDED
