
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

#include "../Bullet.hpp"
#include "../../game/engine/EntityHandler.hpp"

// pos is supposed to be an aray of two positions
template <typename It>
void Boss::generateGenericBulletCircles(LX_AABB& pos,
                                        LX_Graphics::LX_Sprite *sp,
                                        It _beg, It _end, bool is_lunatic) noexcept
{
    EntityHandler& hdl = EntityHandler::getInstance();
    for(auto it = _beg; it != _end; ++it)
    {
        if(is_lunatic)
            hdl.pushEnemyMissile(*(new LunaticBullet(attack_val, sp, pos, *it)));
        else
            hdl.pushEnemyMissile(*(new Bullet(attack_val, sp, pos, *it)));
    }
}
