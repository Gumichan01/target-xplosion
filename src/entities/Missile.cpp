
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

#include "Missile.hpp"
#include "../game/Power.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/engine/AudioHandler.hpp"

#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Log.hpp>


Missile::Missile(unsigned int pow, unsigned int mul, LX_Graphics::LX_Sprite *image,
                 LX_AABB& rect, LX_Physics::LX_Vector2D& sp)
    : Entity(image, rect, sp), power(pow), multiplier(mul),
      missile_box(
{
    rect.x, rect.y, rect.w, rect.h
})
{
    // A missile that has no graphical repreesntation cannot exist
    if(graphic == nullptr)
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,"missile - No graphical resource");
}


unsigned int Missile::hit() const
{
    return(power*multiplier);
}


void Missile::move()
{
    fpos += speed;
    fpos.toPixelUnit(position);
    fpos.toPixelUnit(missile_box);
}


const LX_AABB * Missile::getHitbox() const
{
    return &missile_box;
}

