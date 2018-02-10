
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

#include "Heaviside.hpp"
#include "Bullet.hpp"
#include "Player.hpp"
#include "../game/engine/Engine.hpp"
#include "../pattern/Strategy.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../resources/ResourceManager.hpp"

namespace
{
const uint32_t HVS_SHOT_DELAY = 500;
const int HVS_BULLET_VELOCITY = -12;

const uint32_t HVSP_SHOT_DELAY = 300;
const int HVSP_BULLET_VELOCITY = -16;
const int HVS_BULLET_DIM = 24;
const int HVS_BULLET_OFFSET_Y = 24;
const unsigned int HVS_BULLET_ID = 6;
const unsigned int HVSP_BULLET_ID = 9;
}


/// Heaviside

Heaviside::Heaviside(unsigned int hp, unsigned int att, unsigned int sh,
                     LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                     float vx, float vy)
    : TargetShooter(hp, att, sh, image, x, y, w, h, vx, vy)
{
    id = HVS_BULLET_ID;
    vel = HVS_BULLET_VELOCITY;

    ShotStrategy *st = new ShotStrategy(this);
    st->setShotDelay(HVS_SHOT_DELAY);

    // strat is already set to mvs
    mvs->addMoveStrat(new HeavisideStrat(this));
    mvs->addShotStrat(st);
}


void Heaviside::fire()
{
    using namespace LX_Physics;
    LX_AABB rect = {position.x, position.y + HVS_BULLET_OFFSET_Y,
                    HVS_BULLET_DIM, HVS_BULLET_DIM
                   };

    Player::accept(this);

    // Shoot the player only if he can be seen
    if(last_player_x + Player::PLAYER_WIDTH < position.x)
    {
        LX_Vector2D v;
        Engine *g = Engine::getInstance();
        const ResourceManager *rc = ResourceManager::getInstance();
        LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, id);

        BulletPattern::shotOnTarget(position.x, position.y, last_player_x,
                                    last_player_y, HVS_BULLET_VELOCITY, v);
        g->acceptEnemyMissile(new Bullet(attack_val, spr, rect, v));
    }
}


/// RHeaviside

RHeaviside::RHeaviside(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, int x, int y, int w,
                       int h, float vx, float vy)
    : Heaviside(hp, att, sh, image, x, y, w, h, vx, vy)
{
    mvs->addMoveStrat(new HeavisideReverseStrat(this));
}


/// Heaviside

HeavisidePurple::HeavisidePurple(unsigned int hp, unsigned int att, unsigned int sh,
                                 LX_Graphics::LX_Sprite *image, int x, int y,
                                 int w, int h, float vx, float vy)
    : Heaviside(hp, att, sh, image, x, y, w, h, vx, vy)
{
    id = HVSP_BULLET_ID;
    ShotStrategy *st = new ShotStrategy(this);
    st->setShotDelay(HVSP_SHOT_DELAY);
    mvs->addShotStrat(st);
}


void HeavisidePurple::fire()
{
    LX_AABB rect = {position.x, position.y + HVS_BULLET_OFFSET_Y,
                    HVS_BULLET_DIM, HVS_BULLET_DIM
                   };
    LX_Physics::LX_Vector2D v(HVSP_BULLET_VELOCITY, 0.0f);
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();

    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, id);
    g->acceptEnemyMissile(new TrailBullet(attack_val, spr, rect, v));
}


RHeavisidePurple::RHeavisidePurple(unsigned int hp, unsigned int att, unsigned int sh,
                                   LX_Graphics::LX_Sprite *image, int x, int y,
                                   int w, int h, float vx, float vy)
    : HeavisidePurple(hp, att, sh, image, x, y, w, h, vx, vy)
{
    mvs->addMoveStrat(new HeavisideReverseStrat(this));
}

