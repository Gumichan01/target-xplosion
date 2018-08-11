
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

#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include "Entity.hpp"

#include <Lunatix/Hitbox.hpp>

class Missile;

namespace lx
{

namespace Graphics
{
class Sprite;
}

}

/**
*   @class Character
*   @brief The character entity
*
*   It gathers the player and all ennemies
*/
class Character: public Entity
{
    lx::Time::Timer crtimer;

    Character( const Character& ) = delete;
    Character( const Character&& ) = delete;
    Character& operator =( const Character& ) = delete;
    Character& operator =( const Character&& ) = delete;

protected:

    static const unsigned int HIT_DELAY = 75;
    lx::Physics::Circle circle_box = { { FloatBox::FNIL, FloatBox::FNIL }, 0U };
    unsigned int health_point;
    unsigned int max_health_point;
    unsigned int attack_val;
    unsigned int shield;
    bool was_killed;
    bool dying;

    lx::Graphics::Sprite * hit_sprite;
    bool hit;

    void characterInit();
    virtual void createHitSprite();
    virtual void destroyHitSprite() noexcept;

public:

    Character( unsigned int hp, unsigned int att, unsigned int sh,
               lx::Graphics::Sprite * image, const lx::Graphics::ImgRect& rect,
               const lx::Physics::Vector2D& sp );

    virtual void draw() noexcept override;
    virtual void receiveDamages( unsigned int attacks ) noexcept;
    virtual void collision( Missile * mi ) noexcept = 0;
    const lx::Physics::Circle& getHitbox() const noexcept;
    void kill() noexcept;

    unsigned int getHP() const noexcept;
    unsigned int getMaxHP() const noexcept;
    unsigned int getATT() const noexcept;
    unsigned int getDEF() const noexcept;
    bool isDying() const noexcept;

    virtual void setX( float nx ) noexcept override;
    virtual void setY( float ny ) noexcept override;

    virtual ~Character();
};

#endif // CHARACTER_H_INCLUDED
