
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

#include "TargetShooter.hpp"
#include "Player.hpp"
#include "BasicMissile.hpp"

#include "../game/Balance.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../game/Power.hpp"
#include "../pattern/Strategy.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../resources/ResourceManager.hpp"


using namespace LX_Physics;
using namespace LX_Graphics;
using namespace DynamicGameBalance;
using namespace MissileInfo;

namespace
{
const int SHOOTER_BULLET_ID = 8;
const float SHOOTER_BULLET_VEL = -8.0f;
const int SHOOTER_BULLET_DIM = 24;
}


TargetShooter::TargetShooter(unsigned int hp, unsigned int att, unsigned int sh,
                             LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                             float vx, float vy)
    : Enemy(hp, att, sh, image, x, y, w, h, vx, vy), id(SHOOTER_BULLET_ID),
      vel(SHOOTER_BULLET_VEL)
{
    const unsigned int DELAY_TSHOOTER_MISSILE = 1000;

    ShotStrategy *sht = new ShotStrategy(this);
    MoveStrategy *mv  = new MoveStrategy(this);

    sht->setShotDelay(DELAY_TSHOOTER_MISSILE);
    mvs->addMoveStrat(mv);
    mvs->addShotStrat(sht);
    addStrategy(mvs);
}


void TargetShooter::fire()
{
    const int N = 4;
    const float MIN_VEL = 3;
    LX_AABB rect = {position.x, position.y + ((position.h - MISSILE_HEIGHT)/2),
                    SHOOTER_BULLET_DIM, SHOOTER_BULLET_DIM
                   };

    Player::accept(this);

    // Shoot the player only if he can be seen
    if(last_player_x + Player::PLAYER_WIDTH < position.x)
    {
        LX_Vector2D v[N];
        EntityHandler& hdl = EntityHandler::getInstance();
        const ResourceManager *rc = ResourceManager::getInstance();
        LX_Sprite *spr = rc->getResource(RC_MISSILE, id);
        vel = apply_dgb(SHOOTER_BULLET_VEL);

        for(int i = 0; i < N; i++)
        {
            float i_f = static_cast<float>(i);
            BulletPattern::shotOnTarget(position.x, position.y, last_player_x,
                                        last_player_y, vel - (i_f * MIN_VEL),
                                        v[i]);

            hdl.pushEnemyMissile(*(new BasicMissile(attack_val, spr, rect, v[i])));
        }
    }
}

