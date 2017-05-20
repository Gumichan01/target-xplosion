
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

#include "Shooter.hpp"
#include "Player.hpp"
#include "BasicMissile.hpp"

#include "../game/engine/Engine.hpp"
#include "../game/Power.hpp"
#include "../pattern/Strategy.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../resources/ResourceManager.hpp"

using namespace LX_Physics;
using namespace LX_Graphics;

namespace
{
const int SHOOTER_BULLET_ID = 8;
const int SHOOTER_BULLET_VEL = -8;
const int SHOOTER_BULLET_DIM = 24;
}


Shooter::Shooter(unsigned int hp, unsigned int att, unsigned int sh,
                 LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                 float vx, float vy)
    : Enemy(hp, att, sh, image, x, y, w, h, vx, vy), id(SHOOTER_BULLET_ID),
      vel(SHOOTER_BULLET_VEL)
{
    strat = new BasicStrategy(this);
}


void Shooter::fire()
{
    const int N = 4;
    const int MIN_VEL = 3;
    LX_AABB rect = {position.x, position.y + ((position.h - MISSILE_HEIGHT)/2),
                    SHOOTER_BULLET_DIM, SHOOTER_BULLET_DIM
                   };

    Player::accept(this);

    // Shoot the player only if he can be seen
    if(last_player_x + Player::PLAYER_WIDTH < position.x)
    {
        LX_Vector2D v[N];
        Engine *g = Engine::getInstance();
        const ResourceManager *rc = ResourceManager::getInstance();
        LX_Sprite *spr = rc->getResource(RC_MISSILE, id);

        for(unsigned int i = 0; i<= Rank::POWER_LEVEL; i++)
        {
            BulletPattern::shotOnTarget(position.x, position.y, last_player_x,
                                        last_player_y, SHOOTER_BULLET_VEL-(i*MIN_VEL),
                                        v[i]);
            g->acceptEnemyMissile(new BasicMissile(attack_val, spr, rect, v[i]));
        }
    }
}


