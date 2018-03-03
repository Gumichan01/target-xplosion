
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

#ifndef BULLETPATTERN_HPP_INCLUDED
#define BULLETPATTERN_HPP_INCLUDED

#include <LunatiX/LX_Vector2D.hpp>
#include <array>
#include <cmath>


namespace BulletPattern
{

// The number of bullets in the circle
constexpr std::size_t CIRCLE_BULLETS = 24;

// The velocity of the bullets in the circle
constexpr int CIRCLE_BULLETS_DEFAULT_VEL = -8;

constexpr double PI = 3.14159265358979323846;
constexpr Float PI_F = Float{static_cast<float>(PI)};
const std::size_t WAVE_SZ = 3;
const std::size_t DOUBLE_SPIN = 2;

void shotOnPlayer(const Float& shooter_x, const Float& shooter_y,
                  const Float& vel, LX_Physics::LX_Vector2D& v) noexcept;

void shotOnTarget(const Float& shooter_x, const Float& shooter_y,
                  const Float& target_x, const Float& target_y,
                  const Float& vel, LX_Physics::LX_Vector2D& v) noexcept;

// Angle of display of an object
void calculateAngle(const LX_Physics::LX_Vector2D& v, double& angle) noexcept;

// Shoot a wave of 3 Bullets on the player
void waveOnPlayer(const Float& shooter_x, const Float& shooter_y, const Float& vel,
                  std::array<LX_Physics::LX_Vector2D, WAVE_SZ>& varr) noexcept;

/*
    Create the circle pattern
*/
template<std::size_t SZ>
void circlePattern(const Float& pos_x, const Float& pos_y, const Float& vel,
                   std::array<LX_Physics::LX_Vector2D, SZ>& varray) noexcept
{
    const Float BSTEP{PI_F / static_cast<float>(varray.size() / 2)};
    const Float BSR{128.0f};
    Float alpha{0.0f};

    for(LX_Physics::LX_Vector2D& v : varray)
    {
        shotOnTarget(pos_x, pos_y, pos_x + Float{std::cos(alpha.v)} * BSR,
                     pos_y - Float{std::sin(alpha.v)} * BSR, vel, v);
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

    static const Float R_UNIT;

    Float alpha = {0.0f};
    Float alpha_step = {0.0f};

public:
    AbstractSpin() = default;
    virtual ~AbstractSpin() = default;
};


class SpinShot: public AbstractSpin
{
    SpinShot(const SpinShot&);
    SpinShot& operator =(const SpinShot&);

protected:

    static const Float PI_2;
    Float alpha_start;
    Float vel;

public:
    SpinShot(const Float& speed, const Float& a_step, const Float& start = {0.0f});
    virtual void operator ()(const Float& x_src, const Float& y_src, LX_Physics::LX_Vector2D& v) noexcept;
    virtual ~SpinShot() = default;
};

class RevSpinShot: public SpinShot
{
    RevSpinShot(const RevSpinShot&);
    RevSpinShot& operator =(const RevSpinShot&);

public:
    RevSpinShot(const Float& speed, const Float& a_step, const Float& start = {0.0f});
    virtual void operator ()(const Float& x_src, const Float& y_src, LX_Physics::LX_Vector2D& v) noexcept;
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
    DoubleSpinShot(const Float& speed, const Float& a_step,
    const Float& start1 = {0.0f}, const Float& start2 = {0.0f});
    void operator ()(const Float& x_src, const Float& y_src,
                     std::array<LX_Physics::LX_Vector2D, DOUBLE_SPIN>& v) noexcept;
    virtual ~DoubleSpinShot() = default;
};

template<std::size_t SZ>
void initialize_array(const Float& speed, const Float& step, std::array<SpinShot*, SZ>& varray,
                      bool rev = false) noexcept
{
    varray.fill(nullptr);
    const Float PARTS{static_cast<float>(varray.size()) / 2.0f};

    for(std::size_t i = 0; i < varray.size(); ++i)
    {
        const Float I{static_cast<float>(i)};
        if(rev)
            varray[i] = new RevSpinShot(speed, step, I * BulletPattern::PI_F / PARTS);
        else
            varray[i] = new SpinShot(speed, step, I * BulletPattern::PI_F / PARTS);
    }
}

template<std::size_t SZ>
void destroy_array(std::array<SpinShot*, SZ>& varray) noexcept
{
    for(std::size_t i = 0; i < varray.size(); ++i)
    {
        delete varray[i];
        varray[i] = nullptr;
    }
}

}

#endif // BULLETPATTERN_HPP_INCLUDED
