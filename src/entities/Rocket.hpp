

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

#ifndef ROCKET_H_INCLUDED
#define ROCKET_H_INCLUDED

#include "Missile.hpp"

namespace LX_ParticleEngine
{
class LX_ParticleSystem;
class LX_Particle;
}

namespace LX_Graphics
{
class LX_Sprite;
}

class Enemy;
class Character;
class Player;
class Boss02;

class Rocket : public Missile
{
    LX_ParticleEngine::LX_ParticleSystem *sys;
    LX_Graphics::LX_Sprite *particle;
    LX_Physics::LX_Vector2D vp;

    Rocket(const Rocket&) = delete;
    Rocket(const Rocket&&) = delete;
    Rocket& operator =(const Rocket&) = delete;
    Rocket& operator =(const Rocket&&) = delete;

protected:

    Float velocity{0.0f};
    virtual void visit_(Character& c) noexcept;

public:

    const static int ROCKET_RANGE = 128;

    Rocket(unsigned int pow, LX_Graphics::LX_Sprite *image, LX_Graphics::LX_ImgRect& rect,
           LX_Physics::LX_Vector2D& sp);

    virtual void draw() noexcept;
    void visit(Character& c);

    virtual ~Rocket();
};


class PlayerRocket: public Rocket
{

public:

    PlayerRocket(unsigned int pow, LX_Graphics::LX_Sprite *image, LX_Graphics::LX_ImgRect& rect,
                 LX_Physics::LX_Vector2D& sp);

    virtual void accept(Boss02& v);
    virtual void draw() noexcept;
    virtual void move() noexcept;

    virtual ~PlayerRocket() = default;
};


class EnemyRocket: public Rocket
{

public:

    EnemyRocket(unsigned int pow, LX_Graphics::LX_Sprite *image, LX_Graphics::LX_ImgRect& rect,
                LX_Physics::LX_Vector2D& sp);

    virtual void draw() noexcept;
    virtual void move() noexcept;

    virtual ~EnemyRocket() = default;
};

#endif // ROCKET_H_INCLUDED
