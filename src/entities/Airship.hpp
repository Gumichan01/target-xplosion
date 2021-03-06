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

#ifndef AIRSHIP_H
#define AIRSHIP_H

#include "Enemy.hpp"
#include "../pattern/BulletPattern.hpp"

namespace lx
{

namespace Graphics
{
class Sprite;
}

}


class Airship final: public BigEnemy
{
    int idstrat;
    PolygonShape shape;
    BulletPattern::SpinShot pattern1, pattern2;

    // Strategy
    void prepare() noexcept;
    void aposition() noexcept;

    // Fire
    void bomb() noexcept;
    void frontShot() noexcept;
    void doubleSpinShot() noexcept;

    Airship( const Airship& ) = delete;
    Airship( const Airship&& ) = delete;
    Airship& operator =( const Airship& ) = delete;
    Airship& operator =( const Airship&& ) = delete;

public:

    Airship( unsigned int hp, unsigned int att, unsigned int sh,
             lx::Graphics::Sprite * image, int x, int y, int w, int h,
             float vx, float vy );

    virtual void move() noexcept override;
    virtual void draw() noexcept override;
    virtual void collision( Missile * mi ) noexcept override;
    virtual void collision( Player * play ) noexcept override;
    virtual void strategy() noexcept override;
    virtual void fire() noexcept override;
    virtual void die() noexcept override;

    ~Airship() = default;
};

#endif // AIRSHIP_H
