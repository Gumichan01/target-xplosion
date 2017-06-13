
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
#include "../../game/engine/Engine.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../Bullet.hpp"

using namespace LX_Physics;

namespace
{
const int SEMIBOSS03_XMIN = 1000;
const int SEMIBOSS03_YVEL = 2;
const int SEMIBOSS03_WBULLET_ID = 8;

// Up and down movement
const int SEMIBOSS03_YMIN = 47;
const int SEMIBOSS03_YMAX = 500;

/// Strategy #1
const float PERCENT_75 = 0.75f;
const int SEMIBOSS03_STRAT1_DELAY = 500;

/// Strategy #2
const int SEMIBOSS03_WAVE_BULLETS = 5;
// Main speed of the wave bullet
const float SEMIBOSS03_MBULLET_VEL = -16.0f;
const float SEMIBOSS03_DIV34 = 0.75f;
const float SEMIBOSS03_DIV2 = 0.5f;
const float SEMIBOSS03_DIV4 = 0.25f;

const int SEMIBOSS03_YOFF1 = 72;
const int SEMIBOSS03_YOFF2 = 140;
const int SEMIBOSS03_WBULL_W = 32;
const int SEMIBOSS03_WBULL_H = 24;
}


SemiBoss03::SemiBoss03(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                       float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy)
{
    addStrategy(new MoveStrategy(this));
}


void SemiBoss03::bpos()
{
    if(position.x <= SEMIBOSS03_XMIN)
    {
        id_strat = 1;
        position.x += 1;
        speed *= 0.0f;
        speed.vy = SEMIBOSS03_YVEL;

        MoveAndShootStrategy *mvs = new MoveAndShootStrategy(this);
        ShotStrategy *shot = new ShotStrategy(this);

        shot->setShotDelay(SEMIBOSS03_STRAT1_DELAY);
        mvs->addShotStrat(shot);
        mvs->addMoveStrat(new UpDownMoveStrategy(this, SEMIBOSS03_YMIN,
                          SEMIBOSS03_YMAX, SEMIBOSS03_YVEL));

        // Update the strategy
        addStrategy(mvs);
    }
}

void SemiBoss03::spreadShotStrat()
{
    const uint32_t HEALTH_75 = static_cast<float>(max_health_point) * PERCENT_75;

    if(health_point < HEALTH_75)
    {
        id_strat = 2;
        /// @todo new strat
    }
}

void SemiBoss03::strategy()
{
    switch(id_strat)
    {
    case 0:
        bpos();
        break;

    case 1:
        spreadShotStrat();
        break;

    default:
        break;
    }

    Boss::strategy();   /// @todo (#1#) Semiboss03: Strategy
}

void SemiBoss03::waveShot()
{
    const int NB_SHOTS = 2;
    LX_AABB spos[NB_SHOTS];
    LX_Vector2D v[SEMIBOSS03_WAVE_BULLETS];

    spos[0] = {position.x, position.y + SEMIBOSS03_YOFF1,
               SEMIBOSS03_WBULL_W, SEMIBOSS03_WBULL_H
              };
    spos[1] = {position.x, position.y + SEMIBOSS03_YOFF2,
               SEMIBOSS03_WBULL_W, SEMIBOSS03_WBULL_H
              };

    // Main bullet
    v[0] = LX_Vector2D(SEMIBOSS03_MBULLET_VEL, 0.0f);

    // Additionnal bullets
    for(int i = 1; i < SEMIBOSS03_WAVE_BULLETS; ++i)
    {
        v[i] = v[0];
        normalize(v[i]);
    }

    v[1].vx = SEMIBOSS03_MBULLET_VEL * SEMIBOSS03_DIV2;
    v[1].vy = -(SEMIBOSS03_MBULLET_VEL * SEMIBOSS03_DIV2);
    v[2].vx = SEMIBOSS03_MBULLET_VEL * SEMIBOSS03_DIV34;
    v[2].vy = -(SEMIBOSS03_MBULLET_VEL * SEMIBOSS03_DIV4);
    v[3].vx = SEMIBOSS03_MBULLET_VEL * SEMIBOSS03_DIV2;
    v[3].vy = (SEMIBOSS03_MBULLET_VEL * SEMIBOSS03_DIV2);
    v[4].vx = SEMIBOSS03_MBULLET_VEL * SEMIBOSS03_DIV34;
    v[4].vy = (SEMIBOSS03_MBULLET_VEL * SEMIBOSS03_DIV4);

    // Put the bullets
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, SEMIBOSS03_WBULLET_ID);

    for(int j = 0; j < SEMIBOSS03_WAVE_BULLETS; ++j)
    {
        g->acceptEnemyMissile(new Bullet(attack_val, spr, spos[0], v[j]));
        g->acceptEnemyMissile(new Bullet(attack_val, spr, spos[1], v[j]));
    }
}

void SemiBoss03::fire()
{
    switch(id_strat)
    {
    case 1:
        waveShot();
        break;
    }
    //Enemy::fire();  /// @todo (#1#) Semiboss03: SHOOT!
}


void SemiBoss03::die()
{
    Enemy::die();   /// @todo (#1#) Semiboss03: DIE!
}
