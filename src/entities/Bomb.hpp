
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

#ifndef BOMB_H_INCLUDED
#define BOMB_H_INCLUDED

#include "Missile.hpp"


namespace LX_Graphics
{
class LX_Sprite;
}

class Bomb: public Missile
{

    Bomb(const Bomb&) = delete;
    Bomb(const Bomb&&) = delete;
    Bomb& operator =(const Bomb&) = delete;
    Bomb& operator =(const Bomb&&) = delete;

protected:

    bool explosion;
    unsigned int ref_time;
    unsigned int lifetime;
    LX_Graphics::LX_Sprite *xtexture;

    bool _dieOutOfScreen() noexcept;
    void _die() noexcept;

public:

    Bomb(unsigned int pow, LX_Graphics::LX_Sprite *image,
         LX_Graphics::LX_ImgRect& rect, LX_Physics::LX_Vector2D& sp);

    static void loadExplosionBuffer();
    static void destroyExplosionBuffer() noexcept;

    virtual void move() noexcept override;
    virtual void die() noexcept override;

    virtual ~Bomb();
};

class EnemyBomb final: public Bomb
{
public:

    EnemyBomb(unsigned int pow, LX_Graphics::LX_Sprite *image,
              LX_Graphics::LX_ImgRect& rect, LX_Physics::LX_Vector2D& sp);

    virtual void move() noexcept override;
    virtual void die() noexcept override;

    virtual ~EnemyBomb() = default;
};

#endif // BOMB_H_INCLUDED
