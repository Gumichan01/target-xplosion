
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


#include "SemiBoss01.hpp"
#include "../Bullet.hpp"

#include "../../game/engine/EntityHandler.hpp"
#include "../../game/engine/AudioHandler.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Random.hpp>

using namespace AudioHandler;
using namespace LX_Physics;
using namespace FloatBox;

namespace
{
const int SEMIBOSS01_SHOTS = 2;
const unsigned int SEMIBOSS01_RADIUS = 100;

const int SEMIBOSS01_SPRITE_DID = 2;
const int SEMIBOSS01_BULLET_ID = 4;
const Float SEMIBOSS01_YVEL = {2.0f};

const int SEMIBOSS01_XMIN = 1000;
const int SEMIBOSS01_XOFF =  SEMIBOSS01_XMIN + 16;
const int SEMIBOSS01_YMIN = 47;
const int SEMIBOSS01_YMAX = 500;
const int SEMIBOSS01_YMIN_OFFSET = SEMIBOSS01_YMIN + 24;
const int SEMIBOSS01_YMAX_OFFSET =  SEMIBOSS01_YMAX - 24;
const unsigned int SEMIBOSS01_SHOT_DELAY = 1000;

const int SEMIBOSS01_OFFSET1 = 72;
const int SEMIBOSS01_OFFSET2 = 140;
const int SEMIBOSS01_BULLET_OFF = 108;
const float SEMIBOSS01_BULLET_XVEL = -4.0f;
const int SEMIBOSS01_BULLET_VEL = 6;
const int SEMIBOSS01_BULLET_W = 32;
const int SEMIBOSS01_BULLET_H = 32;
const unsigned int SEMIBOSS01_XDELAY = 512;

}


SemiBoss01::SemiBoss01(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                       float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy),
      shot_delay(SEMIBOSS01_SHOT_DELAY)
{
    id_strat = 0;
    circle_box.radius = SEMIBOSS01_RADIUS;

    ShotStrategy *s = new ShotStrategy(this);
    s->setShotDelay(SEMIBOSS01_SHOT_DELAY);

    mvs->addMoveStrat(new MoveStrategy(this));
    mvs->addShotStrat(s);
    addStrategy(mvs);
}


void SemiBoss01::movePosition() noexcept
{
    if(imgbox.p.x < SEMIBOSS01_XMIN)
    {
        id_strat = 1;
        imgbox.p.x = SEMIBOSS01_XMIN +1;
        speed.vx = fbox(0.0f);
        speed.vy = SEMIBOSS01_YVEL;

        mvs->addMoveStrat(new UpDownMoveStrategy(this, SEMIBOSS01_YMIN,
                          SEMIBOSS01_YMAX, SEMIBOSS01_YVEL));
    }
}

bool SemiBoss01::canShoot() const noexcept
{
    /*
        If the boss is close to a specific X position and is going to the left,
        OR if the boss is close to a specific Y maximum/minimum position
        and is going to the bottom/top of the screen, then it cannot shoot
    */
    const Float ZERO = fbox(0.0f);
    if((imgbox.p.x > SEMIBOSS01_XMIN && imgbox.p.x < SEMIBOSS01_XOFF && speed.vx < ZERO)
            || (imgbox.p.y < SEMIBOSS01_YMAX && imgbox.p.y > SEMIBOSS01_YMAX_OFFSET
                && speed.vy > ZERO)
            || (imgbox.p.y > SEMIBOSS01_YMIN && imgbox.p.y < SEMIBOSS01_YMIN_OFFSET
                && speed.vy < ZERO))
    {
        return false;
    }

    return true;
}

void SemiBoss01::shootLvl1() noexcept
{
    unsigned int one_third_hp = max_health_point/3;

    if(health_point < (max_health_point - one_third_hp))
    {
        id_strat = 2;
        shot_delay = SEMIBOSS01_SHOT_DELAY - SEMIBOSS01_SHOT_DELAY/4;
        ShotStrategy *s = new ShotStrategy(this);
        s->setShotDelay(shot_delay);
        mvs->addShotStrat(s);
        EntityHandler::getInstance().bulletCancel();
    }
}

void SemiBoss01::shootLvl2() noexcept
{
    unsigned int one_third_hp = max_health_point/3;

    if(health_point < one_third_hp)
    {
        id_strat = 3;
        shot_delay = SEMIBOSS01_SHOT_DELAY / 2U;
        ShotStrategy *s = new ShotStrategy(this);
        s->setShotDelay(shot_delay);

        mvs->addShotStrat(s);
        EntityHandler::getInstance().bulletCancel();
    }
}

void SemiBoss01::shootLvl3() noexcept
{
    unsigned int one_sixth_hp = max_health_point/6;

    if(health_point < one_sixth_hp)
    {
        id_strat = 4;
        shot_delay = SEMIBOSS01_SHOT_DELAY / 4U;

        ShotStrategy *s = new ShotStrategy(this);
        s->setShotDelay(shot_delay);
        mvs->addShotStrat(s);

        EntityHandler::getInstance().bulletCancel();
    }
}

void SemiBoss01::strategy() noexcept
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

void SemiBoss01::frontShot() noexcept
{
    LX_Graphics::LX_ImgRect pos[SEMIBOSS01_SHOTS];

    pos[0] = {imgbox.p.x, imgbox.p.y + SEMIBOSS01_OFFSET1,
              SEMIBOSS01_BULLET_W, SEMIBOSS01_BULLET_H
             };
    pos[1] = {imgbox.p.x, imgbox.p.y + SEMIBOSS01_OFFSET2,
              SEMIBOSS01_BULLET_W, SEMIBOSS01_BULLET_H
             };

    shot(pos[LX_Random::xrand(0U, 2U)]);
}

void SemiBoss01::rearShot() noexcept
{
    LX_Graphics::LX_ImgRect pos[SEMIBOSS01_SHOTS];

    pos[0] = {imgbox.p.x + SEMIBOSS01_BULLET_OFF, imgbox.p.y + SEMIBOSS01_OFFSET1,
              SEMIBOSS01_BULLET_W, SEMIBOSS01_BULLET_H
             };
    pos[1] = {imgbox.p.x + SEMIBOSS01_BULLET_OFF, imgbox.p.y + SEMIBOSS01_OFFSET2,
              SEMIBOSS01_BULLET_W, SEMIBOSS01_BULLET_H
             };

    shot(pos[LX_Random::xrand(0U, 2U)]);
}

void SemiBoss01::shot(LX_Graphics::LX_ImgRect& pos) noexcept
{
    // If the boss cannot shoot according to its position
    // Do not shoot!
    if(!canShoot())
        return;

    LX_Vector2D vel{SEMIBOSS01_BULLET_XVEL, speed.vy};
    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, SEMIBOSS01_BULLET_ID);

    EntityHandler& hdl = EntityHandler::getInstance();
    hdl.pushEnemyMissile(*(new MegaBullet(attack_val, spr, pos, vel,
                                          SEMIBOSS01_BULLET_VEL)));
}


// Direct shot from the semi-boss
void SemiBoss01::fire() noexcept
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


void SemiBoss01::die() noexcept
{
    if(!dying)
    {
        const ResourceManager *rc = ResourceManager::getInstance();
        graphic = rc->getResource(RC_XPLOSION, SEMIBOSS01_SPRITE_DID);
        AudioHDL::getInstance()->playVoiceWave();
        addStrategy(new BossDeathStrategy(this, DEFAULT_XPLOSION_DELAY,
                                          SEMIBOSS01_XDELAY));
    }
    Boss::die();
}
