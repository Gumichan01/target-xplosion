
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


#include "SemiBoss03.hpp"
#include "../../pattern/Strategy.hpp"


SemiBoss03::SemiBoss03(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                       float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy)
{
    addStrategy(new MoveStrategy(this));
}


void SemiBoss03::strategy()
{
    Boss::strategy();   /// @todo (#1#) Semiboss03: Strategy
}


void SemiBoss03::fire()
{
    Enemy::fire();  /// @todo (#1#) Semiboss03: SHOOT!
}


void SemiBoss03::die()
{
    Enemy::die();   /// @todo (#1#) Semiboss03: DIE!
}
