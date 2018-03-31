
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

#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include "Entity.hpp"

#include <LunatiX/LX_Hitbox.hpp>

class Missile;

namespace LX_Graphics
{
class LX_Sprite;
}

/**
*   @class Character
*   @brief The character entity
*
*   It gathers the player and all ennemies
*/
class Character: public Entity
{

    Character(const Character&) = delete;
    Character(const Character&&) = delete;
    Character& operator =(const Character&) = delete;
    Character& operator =(const Character&&) = delete;

protected:

    static const unsigned int HIT_DELAY = 75;
    LX_Physics::LX_Circle circle_box = {{FloatBox::FNIL, FloatBox::FNIL}, 0U};
    bool was_killed;
    unsigned int health_point;
    unsigned int max_health_point;
    unsigned int attack_val;
    unsigned int shield;
    bool was_killed;
    bool dying;

    LX_Graphics::LX_Sprite * hit_sprite;
    unsigned int hit_time;
    bool hit;

    void characterInit();
    virtual void createHitSprite();
    virtual void destroyHitSprite() noexcept;

public:

    Character(unsigned int hp, unsigned int att, unsigned int sh,
              LX_Graphics::LX_Sprite *image, const LX_Graphics::LX_ImgRect& rect,
              const LX_Physics::LX_Vector2D& sp);

    virtual void draw() noexcept override;
    virtual void receiveDamages(unsigned int attacks) noexcept;
    virtual void collision(Missile *mi) noexcept = 0;
    virtual const LX_Physics::LX_Circle& getHitbox() const noexcept;
    void kill() noexcept;

    unsigned int getHP() const noexcept;
    unsigned int getMaxHP() const noexcept;
    unsigned int getATT() const noexcept;
    unsigned int getDEF() const noexcept;
    bool isDying() const noexcept;

    virtual void setX(float nx) noexcept;
    virtual void setY(float ny) noexcept;

    virtual ~Character();
};

#endif // CHARACTER_H_INCLUDED
