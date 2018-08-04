
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

#ifndef BOSS02_HPP_INCLUDED
#define BOSS02_HPP_INCLUDED

#include "Boss.hpp"


class Missile;
class PlayerRocket;

namespace lx
{

namespace Graphics
{
class Sprite;
}

}


class Boss02 final: public Boss
{
    lx::Physics::FloatingBox global_hitbox;
    lx::Physics::FloatingBox shield_hitbox;
    PolygonShape shape;
    lx::Graphics::Sprite * sprite;
    lx::Graphics::Sprite * sh_sprite;
    bool has_shield;
    bool shield_destroyed;
    unsigned int b1time;
    unsigned int rshield_life;

    void changeShotStrat( const unsigned int d ) noexcept;
    void prepareTheAttack() noexcept;
    void engage() noexcept;
    void meshAttack() noexcept;
    void targetAttack() noexcept;
    void bulletAttack() noexcept;
    void mesh() noexcept;
    void target() noexcept;
    void danmaku() noexcept;
    void absorb( Missile * m ) noexcept;

    Boss02( const Boss02& ) = delete;
    Boss02( const Boss02&& ) = delete;
    Boss02& operator =( const Boss02& ) = delete;
    Boss02& operator =( const Boss02&& ) = delete;

public:

    explicit Boss02( unsigned int hp, unsigned int att, unsigned int sh,
                     lx::Graphics::Sprite * image, int x, int y, int w, int h,
                     float vx, float vy );

    void visit( Missile& );
    void visit( PlayerRocket& rocket );

    virtual void fire() noexcept override;
    virtual void strategy() noexcept override;
    virtual void move() noexcept override;
    virtual void collision( Missile * mi ) noexcept override;
    virtual void collision( Player * play ) noexcept override;
    virtual void die() noexcept override;

    ~Boss02() = default;
};

#endif // BOSS02_HPP_INCLUDED
