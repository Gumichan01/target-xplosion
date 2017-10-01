
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

#include <LunatiX/LX_Timer.hpp>
#include <array>

using namespace LX_Physics;
using namespace AudioHandler;
using namespace BulletPattern;

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
const unsigned int SEMIBOSS03_STRAT1_DELAY = 1000;

/// Strategy #2
const float PERCENT_50 = 0.50f;

// Main speed of the wave bullet
const float SEMIBOSS03_MBULLET_VEL = -6.4f;
const unsigned int SEMIBOSS03_STRAT2_DELAY = 800;
const float SEMIBOSS03_DIV2 = 0.5f;
const int SEMIBOSS03_YOFF1 = 72;
const int SEMIBOSS03_YOFF2 = 140;
const int SEMIBOSS03_WBULL_W = 16;
const int SEMIBOSS03_WBULL_H = 16;

/// Strategy #3
const int SEMIBOSS03_SHOTS = 2;
const float PERCENT_25 = 0.25f;
const int SEMIBOSS03_STRAT3_DELAY = 600;
const int SEMIBOSS03_SBULL_W = 48;
const int SEMIBOSS03_SBULL_H = 16;

const int SEMIBOSS03_XOFF = 108;
const int SEMIBOSS03_YOFF = 106;
//long spin_counter;

/// Death
const unsigned int SEMIBOSS03_DELAY_NOISE = 512;
const int SEMIBOSS03_XBULLET_VEL = 4;
const size_t SEMIBOSS03_XBULLET_N = 6;

/// Spin circles

const float SEMIBOSS03_SPIN_STEP = BulletPattern::PI_F / 5.0f;
//const size_t SEMIBOSS03_SPIN_NUM = 24;
const size_t SEMIBOSS03_SPIN_VEL = 10;

}


SemiBoss03::SemiBoss03(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                       float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy), mult(nullptr), sbt(nullptr),
      shot(nullptr)
{
    addStrategy(new MoveStrategy(this));
    BulletPattern::initialize_array(SEMIBOSS03_SPIN_VEL, SEMIBOSS03_SPIN_STEP, vspin);
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
        shot = new ShotStrategy(this);

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
        shot->setShotDelay(SEMIBOSS03_STRAT2_DELAY);
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
        sbt = new SemiBoss03Target(this);
        shot->setShotDelay(SEMIBOSS03_STRAT3_DELAY);

        mult = new MultiStrategy(this);
        mult->addStrat(*mvs);
        mult->addStrat(*sbt);

        strat = nullptr;
        addStrategy(mult);
        Engine::getInstance()->bulletCancel();
    }
}

void SemiBoss03::spinShotStratHard()
{
    const uint32_t HEALTH_25 = static_cast<float>(max_health_point) * PERCENT_25;

    if(health_point < HEALTH_25)
    {
        id_strat = 4;
        shot->setShotDelay(SEMIBOSS03_STRAT3_DELAY * SEMIBOSS03_DIV2);
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

    wpos[0] = {position.x, position.y + SEMIBOSS03_YOFF1,
               SEMIBOSS03_WBULL_W, SEMIBOSS03_WBULL_H
              };
    wpos[1] = {position.x, position.y + SEMIBOSS03_YOFF2,
               SEMIBOSS03_WBULL_W, SEMIBOSS03_WBULL_H
              };

    std::array<LX_Vector2D, BulletPattern::WAVE_SZ> varray;
    BulletPattern::waveOnPlayer(hitbox.center.x, hitbox.center.y,
                                SEMIBOSS03_MBULLET_VEL, varray);

    // Put the bullets
    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, SEMIBOSS03_WBULLET_ID);

    for(LX_Vector2D& v: varray)
    {
        g->acceptEnemyMissile(new Bullet(attack_val, spr, wpos[0], v));
        g->acceptEnemyMissile(new Bullet(attack_val, spr, wpos[1], v));
    }
}

// Refactor it
void SemiBoss03::spinShot()
{
    LX_AABB spos = {position.x + SEMIBOSS03_XOFF, position.y + SEMIBOSS03_YOFF,
                    SEMIBOSS03_SBULL_W, SEMIBOSS03_SBULL_H
                   };

    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, SEMIBOSS03_SBULLET_ID);

    LX_Vector2D v;
    for(BulletPattern::SpinShot* spin: vspin)
    {
        (*spin)(spos.x, spos.y, v);
        g->acceptEnemyMissile(new Bullet(attack_val, spr, spos, v));
    }
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
                                             SEMIBOSS03_XBULLET_VEL/2));
    }
}


void SemiBoss03::fire()
{
    spinShot();
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
    BulletPattern::destroy_array(vspin);
    explosionShot();
    shot = nullptr; /// No memory leak because it was freed by strat in Enemy
    delete sbt;

}

/// strat

SemiBoss03Target::SemiBoss03Target(SemiBoss03 * nboss)
    : Strategy(nboss), BossStrategy(nboss), b(nboss) {}


void SemiBoss03Target::proceed()
{
    if((LX_Timer::getTicks() - reference_time) > SEMIBOSS03_STRAT1_DELAY)
    {
        b->waveShot();
        reference_time = LX_Timer::getTicks();
    }
}
