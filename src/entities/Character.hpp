
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

#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include "Entity.hpp"

#include <LunatiX/LX_Hitbox.hpp>

class Missile;

/// @todo (#2#) v0.4.7: Character animation at death

/**
*   @class Character
*   @brief The character entity
*
*   It gathers the player and all ennemies
*/
class Character: public Entity
{

protected :

    LX_Physics::LX_Circle hitbox;
    bool was_killed;
    unsigned int health_point;
    unsigned int max_health_point;
    unsigned int attack_val;
    unsigned int shield;

    void characterInit();

public :

    Character(unsigned int hp, unsigned int att, unsigned int sh,
              LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
              const LX_AABB& rect, const LX_Physics::LX_Vector2D& sp);

    virtual void receiveDamages(unsigned int attacks);
    virtual void collision(Missile *mi) = 0;
    void kill();
    virtual const LX_Physics::LX_Circle * getHitbox() = 0;


    unsigned int getHP() const;
    unsigned int getMaxHP() const;
    unsigned int getATT() const;
    unsigned int getDEF() const;
    bool killed() const;

    virtual void setX(int newX);
    virtual void setY(int newY);

    virtual ~Character() {}
};

#endif // CHARACTER_H_INCLUDED
