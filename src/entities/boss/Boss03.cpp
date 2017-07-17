
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


#include "Boss03.hpp"

#include "../Player.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Texture.hpp>

Boss03::Boss03(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy) : Boss(hp, att, sh, image, x, y, w, h, vx, vy)
{
    /// @todo Boss03 — constructor
}

void Boss03::fire()
{
    /// @todo Boss03 — fire()
}

void Boss03::strategy()
{
    /// @todo Boss03 — strategy()
    Boss::strategy();
}


void Boss03::move()
{
    /// @todo Boss03 — move()
    Boss::move();
}

void Boss03::collision(Missile *mi)
{
    /// @todo Boss03 — collision(missile)
}

void Boss03::collision(Player *play)
{
    /// @todo Boss03 — collision(player)
}

void Boss03::die()
{
    /// @todo Boss03 — die()
    Enemy::die();
}
