
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

#ifndef HEAVISIDE_HPP_INCLUDED
#define HEAVISIDE_HPP_INCLUDED

#include "TargetShooter.hpp"


class Heaviside: public TargetShooter
{

    Heaviside( const Heaviside& ) = delete;
    Heaviside( const Heaviside&& ) = delete;
    Heaviside& operator =( const Heaviside& ) = delete;
    Heaviside& operator =( const Heaviside&& ) = delete;

public:

    Heaviside( unsigned int hp, unsigned int att, unsigned int sh,
               lx::Graphics::Sprite * image, int x, int y, int w, int h,
               float vx, float vy );

    virtual void fire() noexcept override;
    virtual ~Heaviside() = default;
};

// Heaviside with the reverse pattern
class RHeaviside final: public Heaviside
{

    RHeaviside( const RHeaviside& ) = delete;
    RHeaviside( const RHeaviside&& ) = delete;
    RHeaviside& operator =( const RHeaviside& ) = delete;
    RHeaviside& operator =( const RHeaviside&& ) = delete;

public:

    RHeaviside( unsigned int hp, unsigned int att, unsigned int sh,
                lx::Graphics::Sprite * image, int x, int y, int w, int h,
                float vx, float vy );

    virtual ~RHeaviside() = default;
};

class HeavisidePurple: public Heaviside
{

    HeavisidePurple( const HeavisidePurple& ) = delete;
    HeavisidePurple( const HeavisidePurple&& ) = delete;
    HeavisidePurple& operator =( const HeavisidePurple& ) = delete;
    HeavisidePurple& operator =( const HeavisidePurple&& ) = delete;

public:

    HeavisidePurple( unsigned int hp, unsigned int att, unsigned int sh,
                     lx::Graphics::Sprite * image, int x, int y, int w, int h,
                     float vx, float vy );

    virtual void fire() noexcept;
    virtual ~HeavisidePurple() = default;
};

class RHeavisidePurple final: public HeavisidePurple
{

    RHeavisidePurple( const RHeavisidePurple& ) = delete;
    RHeavisidePurple( const RHeavisidePurple&& ) = delete;
    RHeavisidePurple& operator =( const RHeavisidePurple& ) = delete;
    RHeavisidePurple& operator =( const RHeavisidePurple&& ) = delete;

public:

    RHeavisidePurple( unsigned int hp, unsigned int att, unsigned int sh,
                      lx::Graphics::Sprite * image, int x, int y, int w, int h,
                      float vx, float vy );

    virtual ~RHeavisidePurple() = default;
};

#endif // HEAVISIDE_HPP_INCLUDED
