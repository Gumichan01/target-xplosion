
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

#ifndef MISSILE_H_INCLUDED
#define MISSILE_H_INCLUDED

#include "Entity.hpp"


class Boss02;

namespace lx
{

namespace Graphics
{
class AnimatedSprite;
}

}

namespace MissileInfo
{
const int MISSILE_WIDTH = 32;
const int MISSILE_HEIGHT = 12;
const Float MISSILE_SPEED = { 16.0f };

const int ROCKET_WIDTH = 48;
const int ROCKET_HEIGHT = 16;
const Float ROCKET_SPEED = { 24.0f };

const int BOMB_WIDTH = 47;
const int BOMB_HEIGHT = 47;
const Float BOMB_SPEED = { 16.0f };

const int LASER_WIDTH = 128;
const int LASER_HEIGHT = 64;
const int LASER_LIFETIME = 2000;
}

enum class MissileType
{
    BASIC_MISSILE, DOUBLE_MISSILE,
    WAVE_MISSILE, ROCKET_TYPE, LASER_TYPE, BOMB_TYPE
};


class Missile : public Entity
{
    lx::Graphics::AnimatedSprite * bulletx;
    bool xplosion;
    unsigned int mref;

    Missile( const Missile& ) = delete;
    Missile( const Missile&& ) = delete;
    Missile& operator =( const Missile& ) = delete;
    Missile& operator =( const Missile&& ) = delete;

protected:

    unsigned int power;         /* The power of the missile*/
    unsigned int multiplier;    /* The multiplier of the power */

public:

    static void loadExplosionBuffer();
    static void destroyExplosionBuffer() noexcept;

    Missile( unsigned int pow, unsigned int mul, lx::Graphics::Sprite * image,
             lx::Graphics::ImgRect& rect, lx::Physics::Vector2D& sp );

    virtual void accept( Boss02& v );

    unsigned int hit() const noexcept;
    virtual void move() noexcept override;
    virtual void die() noexcept override;

    const lx::Physics::FloatingBox& getHitbox() const noexcept;
    bool explosion() const noexcept;

    virtual ~Missile();
};

#endif // MISSILE_H_INCLUDED
