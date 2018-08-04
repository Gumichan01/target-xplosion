

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

#ifndef ROCKET_H_INCLUDED
#define ROCKET_H_INCLUDED

#include "Missile.hpp"

namespace lx
{

namespace ParticleEngine
{
class ParticleSystem;
class Particle;
}

namespace Graphics
{
class Sprite;
}

}

class Enemy;
class Character;
class Player;
class Boss02;

class Rocket : public Missile
{
    lx::ParticleEngine::ParticleSystem * sys;
    lx::Graphics::Sprite * particle;
    lx::Physics::Vector2D vp;

    Rocket( const Rocket& ) = delete;
    Rocket( const Rocket&& ) = delete;
    Rocket& operator =( const Rocket& ) = delete;
    Rocket& operator =( const Rocket&& ) = delete;

protected:

    Float velocity = FloatBox::FNIL;
    void visit_( Character& c ) noexcept;

public:

    const static int ROCKET_RANGE = 128;

    Rocket( unsigned int pow, lx::Graphics::Sprite * image, lx::Graphics::ImgRect& rect,
            lx::Physics::Vector2D& sp );

    virtual void draw() noexcept override;
    void visit( Character& c );

    virtual ~Rocket();
};


class PlayerRocket final: public Rocket
{

    PlayerRocket( const PlayerRocket& ) = delete;
    PlayerRocket& operator=( const PlayerRocket& ) = delete;

public:

    PlayerRocket( unsigned int pow, lx::Graphics::Sprite * image, lx::Graphics::ImgRect& rect,
                  lx::Physics::Vector2D& sp );

    virtual void accept( Boss02& v ) override;
    virtual void draw() noexcept override;
    virtual void move() noexcept override;

    virtual ~PlayerRocket() = default;
};


class EnemyRocket final: public Rocket
{

    EnemyRocket( const EnemyRocket& ) = delete;
    EnemyRocket& operator=( const EnemyRocket& ) = delete;

public:

    EnemyRocket( unsigned int pow, lx::Graphics::Sprite * image, lx::Graphics::ImgRect& rect,
                 lx::Physics::Vector2D& sp );

    virtual void draw() noexcept override;
    virtual void move() noexcept override;

    virtual ~EnemyRocket() = default;
};

#endif // ROCKET_H_INCLUDED
