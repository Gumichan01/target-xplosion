
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
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/


#include "SemiBoss02.hpp"
#include "../Bullet.hpp"
#include "../Rocket.hpp"
#include "../TreeMissile.hpp"

#include "../../game/Balance.hpp"
#include "../../game/engine/EntityHandler.hpp"
#include "../../game/engine/AudioHandler.hpp"
#include "../../resources/ResourceManager.hpp"

#include <Lunatix/Random.hpp>
#include <Lunatix/Physics.hpp>
#include <Lunatix/Time.hpp>

using namespace lx::Physics;
using namespace AudioHandler;
using namespace DynamicGameBalance;
using namespace FloatBox;

namespace
{
const int SEMIBOSS02_XMIN = 1000;
const int SEMIBOSS02_YMIN = 47;
const int SEMIBOSS02_YMAX = 500;
const Float SEMIBOSS02_YVEL = { 2.0f };

const int SEMIBOSS02_SPRITE_DID = 4;
const int SEMIBOSS02_DELAY_NOISE = 512;

const int SEMIBOSS02_BULLET_ID = 4;
const float SEMIBOSS02_BULLET_XVEL = -3.2f;
const float SEMIBOSS02_BULLET_YVEL = 3.2f;
const int SEMIBOSS02_BULLET_W = 19;
const int SEMIBOSS02_BULLET_H = 19;

const int SEMIBOSS02_ROCKET_ID = 1;
const float SEMIBOSS02_ROCKET_VEL = -3.5f;
const int SEMIBOSS02_ROCKET_W = 48;
const int SEMIBOSS02_ROCKET_H = 16;

const unsigned int SEMIBOSS02_MSTRAT1_DELAY = 1000;
const unsigned int SEMIBOSS02_MSTRAT2_DELAY = 750;

const int SEMIBOSS02_SHOTS = 2;
const int SHOT1_OFFSET = 72;
const int SHOT2_OFFSET = 140;
const int BULLETX_OFFSET = 108;
}


SemiBoss02::SemiBoss02( unsigned int hp, unsigned int att, unsigned int sh,
                        lx::Graphics::Sprite * image, int x, int y, int w, int h,
                        float vx, float vy )
    : Boss( hp, att, sh, image, x, y, w, h, vx, vy )
{
    addStrategy( new MoveStrategy( this ) );
}


void SemiBoss02::bposition() noexcept
{
    if ( imgbox.p.x < SEMIBOSS02_XMIN )
    {
        id_strat = 1;

        imgbox.p.x = SEMIBOSS02_XMIN + 1;
        speed.vx = FNIL;
        speed.vy = SEMIBOSS02_YVEL;

        ShotStrategy * shot = new ShotStrategy( this );
        shot->setShotDelay( SEMIBOSS02_MSTRAT1_DELAY );

        mvs->addShotStrat( shot );
        mvs->addMoveStrat( new UpDownMoveStrategy( this, SEMIBOSS02_YMIN,
                           SEMIBOSS02_YMAX, SEMIBOSS02_YVEL ) );

        addStrategy( mvs );
    }
}

void SemiBoss02::btarget() noexcept
{
    const unsigned int HALF = max_health_point / 2;

    if ( health_point < HALF )
    {
        id_strat = 2;

        ShotStrategy * shot = new ShotStrategy( this );
        shot->setShotDelay( SEMIBOSS02_MSTRAT2_DELAY );
        mvs->addShotStrat( shot );

        EntityHandler::getInstance().bulletCancel();
    }
}

void SemiBoss02::mesh() noexcept
{
    float vx = SEMIBOSS02_BULLET_XVEL, vy = SEMIBOSS02_BULLET_YVEL;
    lx::Physics::Vector2D v[] = {lx::Physics::Vector2D{vx, vy}, lx::Physics::Vector2D{vx, -vy}};

    lx::Graphics::ImgRect rect[SEMIBOSS02_SHOTS];
    rect[0] = { imgbox.p.x + BULLETX_OFFSET, imgbox.p.y + SHOT1_OFFSET,
                SEMIBOSS02_BULLET_W, SEMIBOSS02_BULLET_H
              };
    rect[1] = { imgbox.p.x + BULLETX_OFFSET, imgbox.p.y + SHOT2_OFFSET,
                SEMIBOSS02_BULLET_W, SEMIBOSS02_BULLET_H
              };

    const ResourceManager * const rc = ResourceManager::getInstance();
    lx::Graphics::Sprite * s = rc->getResource( RC_MISSILE, SEMIBOSS02_BULLET_ID );

    float vel = apply_dgb( vector_norm( v[0] ) );
    EntityHandler& hdl = EntityHandler::getInstance();
    hdl.pushEnemyMissile( *( new MegaBullet( attack_val, s, rect[0], v[0], vel ) ) );
    hdl.pushEnemyMissile( *( new MegaBullet( attack_val, s, rect[1], v[1], vel ) ) );
}

void SemiBoss02::target() noexcept
{
    static int i = 0;
    lx::Physics::Vector2D v{ SEMIBOSS02_ROCKET_VEL, FNIL };
    lx::Graphics::ImgRect rect[SEMIBOSS02_SHOTS];

    rect[0] = { imgbox.p.x, imgbox.p.y + SHOT1_OFFSET,
                SEMIBOSS02_ROCKET_W, SEMIBOSS02_ROCKET_H
              };
    rect[1] = { imgbox.p.x, imgbox.p.y + SHOT2_OFFSET,
                SEMIBOSS02_ROCKET_W, SEMIBOSS02_ROCKET_H
              };

    i = 1 - i;

    EntityHandler& hdl = EntityHandler::getInstance();
    const ResourceManager * const rc = ResourceManager::getInstance();
    lx::Graphics::Sprite * s = rc->getResource( RC_MISSILE, SEMIBOSS02_ROCKET_ID );
    hdl.pushEnemyMissile( *( new EnemyRocket( attack_val, s, rect[i], v ) ) );
}

void SemiBoss02::fire() noexcept
{
    if ( id_strat == 2 )
        target();

    mesh();
}

void SemiBoss02::strategy() noexcept
{
    if ( id_strat == 0 )
        bposition();
    else if ( id_strat == 1 )
        btarget();

    Boss::strategy();
}


void SemiBoss02::die() noexcept
{
    if ( !dying )
    {
        const ResourceManager * const rc = ResourceManager::getInstance();
        graphic = rc->getResource( RC_XPLOSION, SEMIBOSS02_SPRITE_DID );

        AudioHDL::getInstance()->playVoiceWave();
        addStrategy( new BossDeathStrategy( this, DEFAULT_XPLOSION_DELAY,
                                            SEMIBOSS02_DELAY_NOISE ) );
    }

    Boss::die();
}
