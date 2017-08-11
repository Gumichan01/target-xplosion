
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

#ifndef MISSILE_H_INCLUDED
#define MISSILE_H_INCLUDED

#include "Entity.hpp"

const int MISSILE_WIDTH = 32;
const int MISSILE_HEIGHT = 12;
const int MISSILE_SPEED = 16;
const int PLAYER_MISSILE_SPEED = 32;

const int ROCKET_WIDTH = 48;
const int ROCKET_HEIGHT = 16;
const int ROCKET_SPEED = 24;

const int BOMB_WIDTH = 47;
const int BOMB_HEIGHT = 47;
const int BOMB_SPEED = 16;

const int LASER_WIDTH = 128;
const int LASER_HEIGHT = 64;
const int LASER_LIFETIME = 2000;


enum class MissileType
{
    BASIC_MISSILE, DOUBLE_MISSILE,
    WAVE_MISSILE, ROCKET_TYPE, LASER_TYPE, BOMB_TYPE
};


class Missile : public Entity
{

protected:

    unsigned int power;         /* The power of the missile*/
    unsigned int multiplier;    /* The multiplier of the power */
    LX_AABB missile_box;

public:

    Missile(unsigned int pow, unsigned int mul, LX_Graphics::LX_Sprite *image,
            LX_AABB& rect, LX_Physics::LX_Vector2D& sp);

    unsigned int hit() const;
    virtual void move();
    const LX_AABB * getHitbox() const;

    virtual ~Missile() {}
};

#endif // MISSILE_H_INCLUDED

