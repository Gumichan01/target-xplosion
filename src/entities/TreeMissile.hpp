
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

#ifndef TREEMISSILE_HPP_INCLUDED
#define TREEMISSILE_HPP_INCLUDED

#include "BasicMissile.hpp"

class TreeMissile final: public Missile
{
    static const int MULTIPLIER = 3;
    unsigned int t;

    TreeMissile( const TreeMissile& ) = delete;
    TreeMissile( const TreeMissile&& ) = delete;
    TreeMissile& operator =( const TreeMissile& ) = delete;
    TreeMissile& operator =( const TreeMissile&& ) = delete;

public:

    TreeMissile( unsigned int pow, lx::Graphics::Sprite * image,
                 lx::Graphics::ImgRect& rect, lx::Physics::Vector2D& sp );

    virtual void move() noexcept override;
    virtual ~TreeMissile() = default;
};

#endif // TREEMISSILE_HPP_INCLUDED
