
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

#include "NetShooter.hpp"
#include "Bullet.hpp"
#include "TreeMissile.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../resources/ResourceManager.hpp"
#include "../pattern/Strategy.hpp"

#include <array>

namespace
{
const int VORTEX_SHOT_ID = 8;
const int VORTEX_NET_ID = 6;

const int VORTEX_SHOT_DELAY = 100;

const int VORTEX_BULLET_DIM = 16;
const int VORTEX_BULLET_XOFF = 8;
const int VORTEX_BULLET_YOFF = 24;

const int VORTEX_NET_XOFF = 48;
const int VORTEX_NET_YOFF = 24;
const float VORTEX_NDIV = 4.0f;
const float VORTEX_NVY = 3.0f;
}

using namespace LX_Physics;

NetShooter::NetShooter( unsigned int hp, unsigned int att, unsigned int sh,
                        LX_Graphics::LX_Sprite * image, int x, int y, int w, int h,
                        float vx, float vy )
    : Enemy( hp, att, sh, image, x, y, w, h, vx, vy )
{
    ShotStrategy * s = new ShotStrategy( this );
    s->setShotDelay( VORTEX_SHOT_DELAY );

    mvs->addMoveStrat( new MoveStrategy( this ) );
    mvs->addShotStrat( s );

    addStrategy( mvs );
}


void NetShooter::directShot() noexcept
{
    LX_Graphics::LX_ImgRect bpos;
    LX_Vector2D bvel = speed * 2.0f;

    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite * spr = rc->getResource( RC_MISSILE, VORTEX_SHOT_ID );

    bpos.p.x = imgbox.p.x + VORTEX_BULLET_XOFF;
    bpos.p.y = imgbox.p.y + VORTEX_BULLET_YOFF;
    bpos.w = VORTEX_BULLET_DIM;
    bpos.h = VORTEX_BULLET_DIM;

    EntityHandler& hdl = EntityHandler::getInstance();
    hdl.pushEnemyMissile( *( new Bullet( attack_val, spr, bpos, bvel ) ) );
}

void NetShooter::netShot() noexcept
{
    LX_Graphics::LX_ImgRect cspos =
    {
        imgbox.p.x + VORTEX_NET_XOFF,
        imgbox.p.y + VORTEX_NET_YOFF,
        VORTEX_BULLET_DIM, VORTEX_BULLET_DIM
    };

    LX_Vector2D bvel_up = { -speed.vx / VORTEX_NDIV, -VORTEX_NVY};
    LX_Vector2D bvel_down = { -speed.vx / VORTEX_NDIV, VORTEX_NVY};

    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite * spr = rc->getResource( RC_MISSILE, VORTEX_NET_ID );

    EntityHandler& hdl = EntityHandler::getInstance();
    hdl.pushEnemyMissile( *( new TreeMissile( attack_val, spr, cspos, bvel_up ) ) );
    hdl.pushEnemyMissile( *( new TreeMissile( attack_val, spr, cspos, bvel_down ) ) );
}


void NetShooter::fire() noexcept
{
    directShot();
    netShot();
}
