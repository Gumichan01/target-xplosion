
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


#include "SemiBoss02.hpp"
#include "../Bullet.hpp"
#include "../BasicMissile.hpp"

#include "../../game/Game.hpp"
#include "../../pattern/BulletPattern.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>

using namespace LX_Physics;


namespace
{
const int SEMIBOSS02_BULLET_ID = 8;
const int SEMIBOSS02_YVEL = 2;

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
const int HOMING_SHOT_OFFSET = SHOT1_OFFSET + (SHOT2_OFFSET - SHOT1_OFFSET);
const int BULLETX_OFFSET = 108;
const int BULLET_VELOCITY = 12;
const int HOMING_BULLET_VELOCITY = -6;

};


SemiBoss02::SemiBoss02(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
                       int x, int y, int w, int h, float vx, float vy)
    : SemiBoss01(hp, att, sh, image, audio, x, y, w, h, vx, vy),
      shot_delay(DELAY_TO_SHOOT), begin_time(0), mvs(nullptr)
{
    id_strat = 0;
    hitbox.radius = 100;
    hitbox.square_radius = hitbox.radius*hitbox.radius;

    mvs = new MoveAndShootStrategy(this);
    ShotStrategy *s = new ShotStrategy(this);
    MoveStrategy *m = new MoveStrategy(this);

    s->setShotDelay(DELAY_TO_SHOOT);
    mvs->addMoveStrat(m);
    mvs->addShotStrat(s);
    addStrategy(mvs);   /// strat and mvs are pointing to the same memory block
}

SemiBoss02::~SemiBoss02()
{
    delete mvs;
    mvs = nullptr;
}


void SemiBoss02::homingShot()
{
    const int SZ = 16;

    LX_Vector2D v;
    Game *g = Game::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_AABB rect = {position.x,(position.y + (position.w/2)), SZ, SZ};

    BulletPattern::shotOnPlayer(position.x, position.y + HOMING_SHOT_OFFSET,
                                HOMING_BULLET_VELOCITY, v);

    g->acceptEnemyMissile(new BasicMissile(attack_val,
                                           rc->getResource(RC_MISSILE, SEMIBOSS02_BULLET_ID),
                                           nullptr, rect, v));
}


void SemiBoss02::strategy()
{
    if(id_strat == 0)
        movePosition();

    Enemy::strategy();
}


// Circular shot
void SemiBoss02::shoot(const MISSILE_TYPE& m_type)
{
    LX_AABB rect[NB_SHOTS];
    Game *g = Game::getInstance();
    LX_Vector2D vel(speed.vx, speed.vy);
    const ResourceManager * rc = ResourceManager::getInstance();
    // If the boss cannot shoot according to its position
    // Do not shoot!
    if(!canShoot())
        return;

    if(m_type == BULLETV1_TYPE)
    {
        rect[0] = {position.x, position.y + SHOT1_OFFSET, 32, 32};
        rect[1] = {position.x, position.y + SHOT2_OFFSET, 32, 32};
    }
    else if(m_type == BULLETV2_TYPE)
    {
        rect[0] = {position.x + BULLETX_OFFSET, position.y + SHOT1_OFFSET, 32, 32};
        rect[1] = {position.x + BULLETX_OFFSET, position.y + SHOT2_OFFSET, 32, 32};
    }
    else if(m_type == BASIC_MISSILE_TYPE)
    {
        homingShot();
        return;
    }

    g->acceptEnemyMissile(new MegaBullet(attack_val,
                                         rc->getResource(RC_MISSILE, 4),
                                         nullptr, rect[0], vel, BULLET_VELOCITY));

    g->acceptEnemyMissile(new MegaBullet(attack_val,
                                         rc->getResource(RC_MISSILE, 4),
                                         nullptr, rect[1], vel, BULLET_VELOCITY));
}
