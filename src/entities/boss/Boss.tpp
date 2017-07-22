
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

#include "Boss.hpp"
#include "../../game/engine/Engine.hpp"
#include "../Bullet.hpp"

// pos is supposed to be an aray of two positions
template <typename It>
void Boss::generateGenericBulletCircles(LX_AABB& pos,
                                        LX_Graphics::LX_Sprite *sp,
                                        It _beg, It _end, bool is_lunatic)
{
    Engine *g = Engine::getInstance();
    for(auto it = _beg; it != _end; ++it)
    {
        if(is_lunatic)
            g->acceptEnemyMissile(new LunaticBullet(attack_val, sp, pos, *it));
        else
            g->acceptEnemyMissile(new Bullet(attack_val, sp, pos, *it));
    }
}
