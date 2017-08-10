
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
#include "../../game/engine/AudioHandler.hpp"
#include "../../pattern/BulletPattern.hpp"
#include "../Bullet.hpp"

#include <array>

using namespace LX_Physics;
using namespace AudioHandler;

namespace
{
const int SEMIBOSS03_XMIN = 800;
const int SEMIBOSS03_YVEL = 2;
const int SEMIBOSS03_WBULLET_ID = 8;
const int SEMIBOSS03_SBULLET_ID = 9;
const int SEMIBOSS03_DEATH_ID = 9;
// Up and down movement
const int SEMIBOSS03_YMIN = 47;
const int SEMIBOSS03_YMAX = 500;

/// Strategy #1
const float PERCENT_75 = 0.75f;
const int SEMIBOSS03_STRAT1_DELAY = 1000;

/// Strategy #2
const float PERCENT_50 = 0.50f;
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

/// Strategy #3
const int SEMIBOSS03_SHOTS = 2;
const float PERCENT_25 = 0.25f;
const int SEMIBOSS03_STRAT3_DELAY = 250;
const size_t SEMIBOSS03_SBULLETS_NUM = 24;
const size_t SEMIBOSS03_SBULLETS_VEL = 10;
const int SEMIBOSS03_SBULL_W = 48;
const int SEMIBOSS03_SBULL_H = 16;

const int SEMIBOSS03_XOFF = 108;
const int SEMIBOSS03_YOFF = 106;
long spin_counter;

/// Death
const int SEMIBOSS03_DELAY_NOISE = 750;
const int SEMIBOSS03_XBULLET_VEL = 4;
const size_t SEMIBOSS03_XBULLET_N = 12;

}


SemiBoss03::SemiBoss03(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                       float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy)
{
    addStrategy(new MoveStrategy(this));
    spin_counter = 0;
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

void SemiBoss03::spinShotStratEasy()
{
    const uint32_t HEALTH_75 = static_cast<float>(max_health_point) * PERCENT_75;

    if(health_point < HEALTH_75)
    {
        id_strat = 2;
        MoveAndShootStrategy *mvs = getMVSStrat();
        ShotStrategy *shot = new ShotStrategy(this);
        // Reduce the delay between two shots
        shot->setShotDelay(SEMIBOSS03_STRAT1_DELAY * SEMIBOSS03_DIV2);
        mvs->addShotStrat(shot);
        Engine::getInstance()->bulletCancel();
    }
}


void SemiBoss03::spinShotStratNormal()
{
    const uint32_t HEALTH_50 = static_cast<float>(max_health_point) * PERCENT_50;

    if(health_point < HEALTH_50)
    {
        id_strat = 3;
        MoveAndShootStrategy *mvs = getMVSStrat();
        ShotStrategy *shot = new ShotStrategy(this);
        // Reduce the delay between two shots
        shot->setShotDelay(SEMIBOSS03_STRAT3_DELAY);
        mvs->addShotStrat(shot);
        Engine::getInstance()->bulletCancel();
    }
}

void SemiBoss03::spinShotStratHard()
{
    const uint32_t HEALTH_25 = static_cast<float>(max_health_point) * PERCENT_25;

    if(health_point < HEALTH_25)
    {
        id_strat = 4;
        MoveAndShootStrategy *mvs = getMVSStrat();
        ShotStrategy *shot = new ShotStrategy(this);
        // Reduce the delay between two shots
        shot->setShotDelay(SEMIBOSS03_STRAT3_DELAY * SEMIBOSS03_DIV2);
        mvs->addShotStrat(shot);
        Engine::getInstance()->bulletCancel();
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
        spinShotStratEasy();
        break;

    case 2:
        spinShotStratNormal();
        break;

    case 3:
        spinShotStratHard();
        break;

    default:
        break;
    }

    Boss::strategy();
}

void SemiBoss03::waveShot()
{
    LX_AABB wpos[SEMIBOSS03_SHOTS];
    LX_Vector2D v[SEMIBOSS03_WAVE_BULLETS];

    wpos[0] = {position.x, position.y + SEMIBOSS03_YOFF1,
               SEMIBOSS03_WBULL_W, SEMIBOSS03_WBULL_H
              };
    wpos[1] = {position.x, position.y + SEMIBOSS03_YOFF2,
               SEMIBOSS03_WBULL_W, SEMIBOSS03_WBULL_H
              };

    // Main bullet
    v[0] = LX_Vector2D(SEMIBOSS03_MBULLET_VEL, 0.0f);

    // Additionnal bullets
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
        g->acceptEnemyMissile(new Bullet(attack_val, spr, wpos[0], v[j]));
        g->acceptEnemyMissile(new Bullet(attack_val, spr, wpos[1], v[j]));
    }
}

void SemiBoss03::spinShot()
{
    LX_AABB spos = {position.x + SEMIBOSS03_XOFF, position.y + SEMIBOSS03_YOFF,
                    SEMIBOSS03_SBULL_W, SEMIBOSS03_SBULL_H
                   };

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, SEMIBOSS03_SBULLET_ID);

    LX_Vector2D v;
    Engine *g = Engine::getInstance();
    std::array<LX_Vector2D, SEMIBOSS03_SBULLETS_NUM> varray;
    BulletPattern::circlePattern(spos.x, spos.y, SEMIBOSS03_SBULLETS_VEL, varray);

    const long M = varray.size()/2 + spin_counter;

    for(long i = spin_counter; i <= M; i++)
    {
        long j = (i < 0) ? varray.size() + i : i;
        g->acceptEnemyMissile(new Bullet(attack_val, spr, spos, varray[j]));
    }

    if(spin_counter == -(static_cast<long>(varray.size()/2)) )
        spin_counter = varray.size()/2 -1;
    else
        spin_counter--;
}

void SemiBoss03::explosionShot()
{
    LX_AABB spos = {position.x + SEMIBOSS03_XOFF, position.y + SEMIBOSS03_YOFF,
                    SEMIBOSS03_SBULL_W, SEMIBOSS03_SBULL_W
                   };

    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, SEMIBOSS03_SBULLET_ID);

    LX_Vector2D v;
    Engine *g = Engine::getInstance();
    std::array<LX_Vector2D, SEMIBOSS03_XBULLET_N> varray;
    BulletPattern::circlePattern(spos.x, spos.y, SEMIBOSS03_XBULLET_VEL, varray);

    for(LX_Vector2D& vec: varray)
    {
        g->acceptEnemyMissile(new MegaBullet(attack_val, spr, spos, vec,
                                             SEMIBOSS03_XBULLET_VEL));
    }
}


void SemiBoss03::fire()
{
    /*switch(id_strat)
    {
    case 1:
    case 2:
        waveShot();
        break;*/

    //case 3:
    //case 4:
        //explosionShot();
        spinShot();
        //break;
    //}
}


void SemiBoss03::die()
{
    if(!dying)
    {
        const ResourceManager *rc = ResourceManager::getInstance();
        graphic = rc->getResource(RC_XPLOSION, SEMIBOSS03_DEATH_ID);

        AudioHDL::getInstance()->playVoiceWave();
        addStrategy(new BossDeathStrategy(this, DEFAULT_XPLOSION_DELAY,
                                          SEMIBOSS03_DELAY_NOISE));
    }

    Boss::die();
}

SemiBoss03::~SemiBoss03()
{
    explosionShot();
}
