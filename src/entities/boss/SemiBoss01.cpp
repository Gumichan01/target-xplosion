
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


#include "SemiBoss01.hpp"
#include "../Bullet.hpp"

#include "../../game/engine/Engine.hpp"
#include "../../game/engine/AudioHandler.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>

using namespace AudioHandler;
using namespace LX_Physics;

namespace
{
const int SEMIBOSS01_SPRITE_DID = 2;
const int SEMIBOSS01_BULLET_ID = 4;
const int SEMIBOSS01_YVEL = 2;

const int NB_SHOTS = 2;

const int XMIN = 1000;
const int X_OFFSET =  XMIN + 16;
const int YMIN = 47;
const int YMAX = 500;
const int YMIN_OFFSET = YMIN + 24;
const int YMAX_OFFSET =  YMAX - 24;
const uint32_t DELAY_TO_SHOOT = 1000;

const int SHOT1_OFFSET = 72;
const int SHOT2_OFFSET = 140;
const int BULLETX_OFFSET = 108;
const int BULLET_VELOCITY = 12;
const int BULLET_W = 32;
const int BULLET_H = 32;

}


SemiBoss01::SemiBoss01(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                       float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy), shot_delay(DELAY_TO_SHOOT),
      begin_time(0)
{
    id_strat = 0;
    hitbox.radius = 100;
    hitbox.square_radius = hitbox.radius*hitbox.radius;

    MoveAndShootStrategy *mvs = new MoveAndShootStrategy(this);
    ShotStrategy *s = new ShotStrategy(this);
    MoveStrategy *m = new MoveStrategy(this);

    s->setShotDelay(DELAY_TO_SHOOT);
    mvs->addMoveStrat(m);
    mvs->addShotStrat(s);
    addStrategy(mvs);
}


void SemiBoss01::movePosition()
{
    if(position.x < XMIN)
    {
        id_strat = 1;
        position.x = XMIN +1;
        speed.vx = 0;
        speed.vy = SEMIBOSS01_YVEL;
        MoveAndShootStrategy *mvs = getMVSStrat();
        mvs->addMoveStrat(new UpDownMoveStrategy(this, YMIN, YMAX, SEMIBOSS01_YVEL));
    }
}

bool SemiBoss01::canShoot() const
{
    /*
        If the boss is close to a specific X position and is going to the left,
        OR if the boss is close to a specific Y maximum/minimum position
        and is going to the bottom/top of the screen, then it cannot shoot
    */
    if((position.x > XMIN && position.x < X_OFFSET && speed.vx < 0)
            || (position.y < YMAX && position.y > YMAX_OFFSET && speed.vy > 0)
            || (position.y > YMIN && position.y < YMIN_OFFSET && speed.vy < 0))
    {
        return false;
    }

    return true;
}


void SemiBoss01::shootLvl1()
{
    unsigned int one_third_hp = max_health_point/3;

    if(health_point < (max_health_point - one_third_hp))
    {
        id_strat = 2;
        shot_delay = DELAY_TO_SHOOT/2;
        ShotStrategy *s = new ShotStrategy(this);
        s->setShotDelay(shot_delay);
        getMVSStrat()->addShotStrat(s);
    }
}

void SemiBoss01::shootLvl2()
{
    unsigned int one_third_hp = max_health_point/3;

    if(health_point < one_third_hp)
    {
        id_strat = 3;
        shot_delay = DELAY_TO_SHOOT/4;
        ShotStrategy *s = new ShotStrategy(this);
        s->setShotDelay(shot_delay);
        getMVSStrat()->addShotStrat(s);
    }
}

void SemiBoss01::shootLvl3()
{
    unsigned int one_sixth_hp = max_health_point/6;

    if(health_point < one_sixth_hp)
    {
        id_strat = 4;
        shot_delay = DELAY_TO_SHOOT/8;
        ShotStrategy *s = new ShotStrategy(this);
        s->setShotDelay(shot_delay);
        getMVSStrat()->addShotStrat(s);
    }
}

void SemiBoss01::strategy()
{
    switch(id_strat)
    {
    case 0:
        movePosition();
        break;

    case 1:
        shootLvl1();
        break;

    case 2:
        shootLvl2();
        break;

    case 3:
        shootLvl3();
        break;

    default:
        break;
    }

    Boss::strategy();
}


void SemiBoss01::collision(Player *play)
{
    if(!mustCheckCollision()) return;
    Boss::collision(play);
}


void SemiBoss01::frontShot()
{
    LX_AABB pos[NB_SHOTS];
    pos[0] = {position.x, position.y + SHOT1_OFFSET, BULLET_W, BULLET_H};
    pos[1] = {position.x, position.y + SHOT2_OFFSET, BULLET_W, BULLET_H};
    shot(pos[0]);
    shot(pos[1]);
}

void SemiBoss01::rearShot()
{
    LX_AABB pos[NB_SHOTS];
    pos[0] = {position.x + BULLETX_OFFSET, position.y + SHOT1_OFFSET, BULLET_W, BULLET_H};
    pos[1] = {position.x + BULLETX_OFFSET, position.y + SHOT2_OFFSET, BULLET_W, BULLET_H};
    shot(pos[0]);
    shot(pos[1]);
}

void SemiBoss01::shot(LX_AABB& pos)
{
    // If the boss cannot shoot according to its position
    // Do not shoot!
    if(!canShoot())
        return;

    Engine *g = Engine::getInstance();
    LX_Vector2D vel(speed.vx, speed.vy);
    const ResourceManager * rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, SEMIBOSS01_BULLET_ID);
    g->acceptEnemyMissile(new MegaBullet(attack_val, spr, pos, vel, BULLET_VELOCITY));
}


// Direct shot from the semi-boss
void SemiBoss01::fire()
{
    switch(id_strat)
    {
    case 0:
    case 1:
        frontShot();
        break;

    case 2:
        rearShot();
        break;

    default:
        frontShot();
        rearShot();
        break;
    }
}


void SemiBoss01::die()
{
    if(!dying)
    {
        const ResourceManager *rc = ResourceManager::getInstance();
        graphic = rc->getResource(RC_XPLOSION, SEMIBOSS01_SPRITE_DID);
        AudioHDL::getInstance()->playVoiceWave();
        addStrategy(new BossDeathStrategy(this, DEFAULT_XPLOSION_DELAY,
                                          DEFAULT_NOISE_DELAY));
    }
    Boss::die();
}
