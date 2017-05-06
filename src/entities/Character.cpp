
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

#include "Character.hpp"
#include "Missile.hpp"

#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Log.hpp> /// Remove it

inline unsigned int MIN(int a, int b)
{
    return static_cast<unsigned int>((a < b)?a:b);
}


Character::Character(unsigned int hp, unsigned int att, unsigned int sh,
                     LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Chunk *audio,
                     const LX_AABB& rect, const LX_Physics::LX_Vector2D& sp)
    : Entity(image, audio, rect, sp), was_killed(false), health_point(hp),
      max_health_point(hp), attack_val(att), shield(sh), dying(false)
{
    characterInit();
}


void Character::characterInit()
{
    int xCenter = position.x + (position.w/2);
    int yCenter = position.y + (position.h/2);
    unsigned int rad = MIN((xCenter - position.x), (yCenter - position.y));

    hitbox = LX_Physics::LX_Circle(LX_Physics::LX_Point(xCenter, yCenter), rad);
}


void Character::receiveDamages(unsigned int attacks)
{
    if(health_point != 0)           // If the character's health is not equal to 0, he may take damages
    {
        if(shield > attacks)        // If the character's shield points is greater than the attak points
            health_point -= 1;      // He will only takes 1 damage point
        else
        {
            if((attacks - shield) < health_point)           // If the character has more health point than damage
                health_point -= (attacks - shield) + 1;     // Decrease the health point
            else
                health_point = 0;                           // One shot
        }

        if(health_point == 0)
            kill();
    }
}


void Character::kill()
{
    was_killed = true;
    die();
}

void Character::die()
{
    if(!dying)
    {
        using namespace LX_Physics;
        speed = LX_Vector2D(0.0f, 0.0f);
        LX_Log::log("character — dying");
    }

    Entity::die();
}


unsigned int Character::getHP() const
{
    return health_point;
}

unsigned int Character::getMaxHP() const
{
    return max_health_point;
}

unsigned int Character::getATT() const
{
    return attack_val;
}

unsigned int Character::getDEF() const
{
    return shield;
}

bool Character::killed() const
{
    return was_killed;
}

void Character::setX(int newX)
{
    Entity::setX(newX);
    hitbox.center.x = newX + position.w /2;
}

void Character::setY(int newY)
{
    Entity::setY(newY);
    hitbox.center.y = newY + position.h /2;
}

