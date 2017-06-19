
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

#include "Vortex.hpp"
#include "Bullet.hpp"
#include "../game/engine/Engine.hpp"
#include "../resources/ResourceManager.hpp"
#include "../pattern/Strategy.hpp"

namespace
{
const int VORTEX_SHOT_ID = 8;
const int VORTEX_BULLET_ID = 6;

const int VORTEX_SHOT_DELAY = 500;
const int VORTEX_SHOT_SPEED = 8;
const int VORTEX_BULLET_DIM = 16;
const int VORTEX_BULLET_XOFF = 8;
const int VORTEX_BULLET_YOFF = 24;
}

using namespace LX_Physics;

Vortex::Vortex(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy)
    : Enemy(hp, att, sh, image, x, y, w, h, vx, vy)
{
    MoveAndShootStrategy *mvs = new MoveAndShootStrategy(this);
    ShotStrategy *s = new ShotStrategy(this);
    s->setShotDelay(VORTEX_SHOT_DELAY);
    mvs->addMoveStrat(new MoveStrategy(this));
    mvs->addShotStrat(s);
    strat = mvs;
}


void Vortex::directShot()
{
    LX_AABB bpos;
    LX_Vector2D bvel(speed.vx * 2, speed.vy * 2);

    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, VORTEX_SHOT_ID);

    bpos.x = position.x + VORTEX_BULLET_XOFF;
    bpos.y = position.y + VORTEX_BULLET_YOFF;
    bpos.w = VORTEX_BULLET_DIM;
    bpos.h = VORTEX_BULLET_DIM;

    g->acceptEnemyMissile(new Bullet(attack_val, spr, bpos, bvel));
}

void Vortex::fire()
{
    directShot();
    //void circleShot();
}
