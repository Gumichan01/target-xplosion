
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
#include "../Rocket.hpp"
#include "../TreeMissile.hpp"

#include "../../game/Game.hpp"
#include "../../pattern/Strategy.hpp"
#include "../../pattern/BulletPattern.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>

using namespace LX_Physics;


namespace
{
const int XMIN = 1000;
const int YMIN = 47;
const int YMAX = 500;
const int SEMIBOSS02_YVEL = 2;

const int SEMIBOSS02_SPRITE_DID = 4;
const int SEMIBOSS02_DELAY_NOISE = 750;

const int SEMIBOSS02_BULLET_ID = 4;
const int SEMIBOSS02_BULLET_XVEL = -8;
const int SEMIBOSS02_BULLET_YVEL = 4;
const int SEMIBOSS02_BULLET_W = 19;
const int SEMIBOSS02_BULLET_H = 19;

const int SEMIBOSS02_ROCKET_ID = 1;
const float SEMIBOSS02_ROCKET_VELOCITY = -4.0f;
const int SEMIBOSS02_ROCKET_W = 32;
const int SEMIBOSS02_ROCKET_H = 16;

const uint32_t SEMIBOSS02_MSTRAT1_DELAY = 1000;
const uint32_t SEMIBOSS02_MSTRAT2_DELAY = 750;

const int NB_SHOTS = 2;
const int SHOT1_OFFSET = 72;
const int SHOT2_OFFSET = 140;
const int BULLETX_OFFSET = 108;
};


SemiBoss02::SemiBoss02(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
                       int x, int y, int w, int h, float vx, float vy)
    : Boss(hp, att, sh, image, audio, x, y, w, h, vx, vy)
{
    addStrategy(new MoveStrategy(this));
}


void SemiBoss02::bposition()
{
    if(position.x < XMIN)
    {
        id_strat = 1;
        position.x = XMIN +1;
        speed.vx = 0;
        speed.vy = SEMIBOSS02_YVEL;
        MoveAndShootStrategy *mvs = new MoveAndShootStrategy(this);
        ShotStrategy *shot = new ShotStrategy(this);
        shot->setShotDelay(SEMIBOSS02_MSTRAT1_DELAY);
        mvs->addShotStrat(shot);
        mvs->addMoveStrat(new UpDownMoveStrategy(this, YMIN, YMAX, SEMIBOSS02_YVEL));
        addStrategy(mvs);
    }
}

void SemiBoss02::btarget()
{
    const uint32_t HALF = max_health_point / 2;

    if(health_point < HALF)
    {
        id_strat = 2;
        MoveAndShootStrategy *mvs = getMVSStrat();
        ShotStrategy *shot = new ShotStrategy(this);
        shot->setShotDelay(SEMIBOSS02_MSTRAT2_DELAY);
        mvs->addShotStrat(shot);
        Game::getInstance()->screenCancel();
    }
}

void SemiBoss02::mesh()
{
    LX_AABB rect[NB_SHOTS];
    Game *g = Game::getInstance();
    const ResourceManager * rc = ResourceManager::getInstance();

    float vx, vy;
    vx = SEMIBOSS02_BULLET_XVEL;
    vy = SEMIBOSS02_BULLET_YVEL;
    LX_Vector2D v[] = {LX_Vector2D(vx, vy), LX_Vector2D(vx, -vy)};

    rect[0] = {position.x + BULLETX_OFFSET, position.y + SHOT1_OFFSET,
               SEMIBOSS02_BULLET_W, SEMIBOSS02_BULLET_H
              };
    rect[1] = {position.x + BULLETX_OFFSET, position.y + SHOT2_OFFSET,
               SEMIBOSS02_BULLET_W, SEMIBOSS02_BULLET_H
              };

    LX_Graphics::LX_Sprite *s = rc->getResource(RC_MISSILE, SEMIBOSS02_BULLET_ID);
    g->acceptEnemyMissile(new TreeMissile(attack_val, s, nullptr, rect[0], v[0]));
    g->acceptEnemyMissile(new TreeMissile(attack_val, s, nullptr, rect[1], v[1]));
}

void SemiBoss02::target()
{
    static int i = 0;
    LX_AABB rect[NB_SHOTS];
    Game *g = Game::getInstance();
    const ResourceManager * rc = ResourceManager::getInstance();
    LX_Vector2D v(SEMIBOSS02_ROCKET_VELOCITY, 0.0f);

    rect[0] = {position.x, position.y + SHOT1_OFFSET,
               SEMIBOSS02_ROCKET_W, SEMIBOSS02_ROCKET_H
              };
    rect[1] = {position.x, position.y + SHOT2_OFFSET,
               SEMIBOSS02_ROCKET_W, SEMIBOSS02_ROCKET_H
              };

    i = 1 - i;
    LX_Graphics::LX_Sprite *s = rc->getResource(RC_MISSILE, SEMIBOSS02_ROCKET_ID);
    g->acceptEnemyMissile(new EnemyRocket(attack_val, s, nullptr, rect[i], v));
}

void SemiBoss02::fire()
{
    if(id_strat == 2)
        target();

    mesh();
}


void SemiBoss02::strategy()
{
    if(id_strat == 0)
        bposition();
    else if(id_strat == 1)
        btarget();

    Enemy::strategy();
}

void SemiBoss02::die()
{
    if(!dying)
    {
        const ResourceManager *rc = ResourceManager::getInstance();
        graphic = rc->getResource(RC_XPLOSION, SEMIBOSS02_SPRITE_DID);
        Game::getInstance()->stopBossMusic();
        addStrategy(new DeathStrategy(this, DEFAULT_XPLOSION_DELAY,
                                      SEMIBOSS02_DELAY_NOISE));
    }

    Boss::die();
}