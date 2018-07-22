
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

#include "Airship.hpp"

#include "Player.hpp"
#include "Bullet.hpp"
#include "Bomb.hpp"
#include "../pattern/Strategy.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../game/engine/AudioHandler.hpp"
#include "../resources/ResourceManager.hpp"

#include <Lunatix/Hitbox.hpp>
#include <Lunatix/Physics.hpp>
#include <Lunatix/Texture.hpp>
#include <vector>

using namespace lx::Physics;
using namespace FloatBox;

namespace
{
const int AIRSHIP_WIDTH = 250;
const int AIRSHIP_HEIGHT = 100;

const int AIRSHIP_FRONT_XPOS = 768;
const int AIRSHIP_FRONT_YPOS = 255;
const int AIRSHIP_BOTTOM_YPOS = 261;

// Bomb
const int AIRSHIP_BOMB_ID = 10;
const int AIRSHIP_BOMB_XOFF = 116;
const int AIRSHIP_BOMB_YOFF = 68;
const int AIRSHIP_BOMB_DIM = 32;
const Float AIRSHIP_BOMB_VEL{10.0f};
const int AIRSHIP_BOMB_NUM = BulletPattern::CIRCLE_BULLETS / 2;
const unsigned int AIRSHIP_BOMB_DELAY = 500;

// Front shot
const int AIRSHIP_FSHOT_ID = 4;
const int AIRSHIP_FSHOT_XOFF = 16;
const int AIRSHIP_FSHOT_YOFF = 32;
const int AIRSHIP_FSHOT_W = 32;
const int AIRSHIP_FSHOT_H = 12;
const Float AIRSHIP_FSHOT_VEL = {10.0f};
const int AIRSHIP_FSHOT_NUM = BulletPattern::CIRCLE_BULLETS * 2;
const unsigned int AIRSHIP_FSHOT_DELAY = 500;

// Spin
const int AIRSHIP_SPIN1_ID = 8;
const int AIRSHIP_SPIN2_ID = 4;
const int AIRSHIP_SPIN_XOFF = 124;
const int AIRSHIP_SPIN_YOFF = 76;
const int AIRSHIP_SPIN_DIM = 24;
const Float AIRSHIP_SPIN_VEL = {6.0f};
const unsigned int AIRSHIP_SPIN_DELAY = 50;
const Float AIRSHIP_STEP = BulletPattern::PI_F / Float{16.0f};
const int AIRSHIP_SPIN_YMIN = 100;
const int AIRSHIP_SPIN_YMAX = 400;
const int AIRSHIP_SPIN_V = 2;

using lx::Physics::FloatPosition;
const std::vector<lx::Physics::FloatPosition> hpoints
{
    lx::Physics::FloatPosition{12.0f, 38.0f}, lx::Physics::FloatPosition{24.0f, 18.0f},
    lx::Physics::FloatPosition{120.0f, 6.0f}, lx::Physics::FloatPosition{222.0f, 18.0f},
    lx::Physics::FloatPosition{248.0f, 38.0f}, lx::Physics::FloatPosition{222.0f, 64.0f},
    lx::Physics::FloatPosition{184.0f, 70.0f}, lx::Physics::FloatPosition{156.0f, 96.0f},
    lx::Physics::FloatPosition{61.0f, 96.0f}, lx::Physics::FloatPosition{45.0f, 68.0f},
    lx::Physics::FloatPosition{24.0f, 58.0f}
};

}


Airship::Airship( unsigned int hp, unsigned int att, unsigned int sh,
                  lx::Graphics::Sprite * image, int x, int y, int w, int h,
                  float vx, float vy )
    : BigEnemy( hp, att, sh, image, x, y, w, h, vx, vy ), idstrat( 0 ),
      shape( hpoints, lx::Physics::FloatPosition{fbox<int>( x ), fbox<int>( y )} ),
      pattern1( AIRSHIP_SPIN_VEL, AIRSHIP_STEP, FNIL ),
      pattern2( AIRSHIP_SPIN_VEL, AIRSHIP_STEP, BulletPattern::PI_F / fbox( 2.0f ) )
{
    phybox.w = AIRSHIP_WIDTH;
    phybox.h = AIRSHIP_HEIGHT;
    mvs->addMoveStrat( new MoveStrategy( this ) );
    addStrategy( mvs );
}


void Airship::boom() noexcept
{
    AudioHandler::AudioHDL::getInstance()->playMediumExplosion();
}

void Airship::move() noexcept
{
    movePoly( shape.getPoly(), speed );
    Enemy::move();
}

void Airship::draw() noexcept
{
    if ( dying )
    {
        const int N = 9;
        lx::Graphics::ImgRect box[N] =
        {
            {24, 32, 64, 64}, {64, 10, 64, 64}, {48, 64, 64, 64},
            {64, 80, 64, 64}, {130, 76, 64, 64}, {110, 8, 64, 64},
            {91, 51, 64, 64}, {174, 24, 64, 64}, {226, 32, 64, 64}
        };

        imgbox = lx::Graphics::toImgRect( phybox );

        for ( int i = 0; i < N; i++ )
        {
            box[i].p.x += imgbox.p.x;
            box[i].p.y += imgbox.p.y;
            graphic->draw( box[i] );
        }
    }
    else
        BigEnemy::draw();
}

void Airship::collision( Missile * mi ) noexcept
{
    if ( !mi->isDead() && !mi->explosion()
            && mi->getX() <= ( phybox.p.x + fbox<decltype( phybox.w )>( phybox.w ) )
            && !dying )
    {
        if ( lx::Physics::collisionBox( phybox, mi->getHitbox() ) )
        {
            if ( lx::Physics::collisionBoxPoly( mi->getHitbox(), shape.getPoly() ) )
            {
                if ( destroyable )
                    reaction( mi );

                mi->die();
            }
        }
    }
}

void Airship::collision( Player * play ) noexcept
{
    if ( play->getX() <= ( phybox.p.x + fbox<decltype( phybox.w )>( phybox.w ) ) && !dying )
    {
        if ( lx::Physics::collisionCircleBox( play->getHitbox(), phybox ) )
        {
            if ( lx::Physics::collisionCirclePoly( play->getHitbox(), shape.getPoly() ) )
            {
                play->die();
            }
        }
    }
}

/// Strategy

void Airship::prepare() noexcept
{
    ShotStrategy * shot = nullptr;

    if ( imgbox.p.y < AIRSHIP_FRONT_YPOS )
    {
        idstrat = 1;
        shot = new ShotStrategy( this );
        shot->setShotDelay( AIRSHIP_BOMB_DELAY );
    }
    else if ( imgbox.p.y > AIRSHIP_BOTTOM_YPOS )
    {
        idstrat = 2;
        shot = new ShotStrategy( this );
        shot->setShotDelay( AIRSHIP_FSHOT_DELAY );
    }
    else
        idstrat = 3;

    mvs->addShotStrat( shot );
}

void Airship::aposition() noexcept
{
    if ( imgbox.p.x <= AIRSHIP_FRONT_XPOS && !isDying() && !isDead() )
    {
        idstrat = 4;
        speed *= FNIL;
        ShotStrategy * shot = new ShotStrategy( this );
        shot->setShotDelay( AIRSHIP_SPIN_DELAY );
        mvs->addShotStrat( shot );
        mvs->addMoveStrat( new UpDownMoveStrategy( this, AIRSHIP_SPIN_YMIN,
                           AIRSHIP_SPIN_YMAX, AIRSHIP_SPIN_V ) );
    }
}

void Airship::strategy() noexcept
{
    switch ( idstrat )
    {
    case 0:
        prepare();
        break;

    case 3:
        aposition();
        break;

    default:
        break;
    }

    Enemy::strategy();
}


/// Fire

void Airship::bomb() noexcept
{
    lx::Graphics::ImgRect bpos
    {
        imgbox.p.x + AIRSHIP_BOMB_XOFF,
        imgbox.p.y + AIRSHIP_BOMB_YOFF,
        AIRSHIP_BOMB_DIM, AIRSHIP_BOMB_DIM
    };

    const lx::Physics::FloatPosition& FLPOS = lx::Physics::toFloatPosition( bpos.p );
    const ResourceManager * const rc = ResourceManager::getInstance();
    lx::Graphics::Sprite * spr = rc->getResource( RC_MISSILE, AIRSHIP_BOMB_ID );
    EntityHandler& hdl = EntityHandler::getInstance();

    std::array<lx::Physics::Vector2D, AIRSHIP_BOMB_NUM> varray;
    BulletPattern::circlePattern( FLPOS.x, FLPOS.y, AIRSHIP_BOMB_VEL, varray );

    auto const _beg = varray.begin() + 1;
    auto const _end = varray.begin() + ( varray.size() / 2 );

    for ( auto it = _beg; it != _end; ++it )
    {
        hdl.pushEnemyMissile( *( new EnemyBomb( attack_val, spr, bpos, *it ) ) );
    }

    // Play the sound
    AudioHandler::AudioHDL::getInstance()->playSmallExplosion();
}

void Airship::frontShot() noexcept
{
    lx::Graphics::ImgRect fspos
    {
        imgbox.p.x + AIRSHIP_FSHOT_XOFF, imgbox.p.y + AIRSHIP_FSHOT_YOFF,
        AIRSHIP_FSHOT_W, AIRSHIP_FSHOT_H
    };

    const lx::Physics::FloatPosition& FLPOS = lx::Physics::toFloatPosition( fspos.p );
    const ResourceManager * const rc = ResourceManager::getInstance();
    lx::Graphics::Sprite * spr = rc->getResource( RC_MISSILE, AIRSHIP_FSHOT_ID );

    EntityHandler& hdl = EntityHandler::getInstance();
    std::array<lx::Physics::Vector2D, AIRSHIP_FSHOT_NUM> varray;
    BulletPattern::circlePattern( FLPOS.x, FLPOS.y, AIRSHIP_FSHOT_VEL, varray );

    const auto _beg = varray.begin() + varray.size() - varray.size() / 4;
    const auto _end = varray.begin() + varray.size() / 4 + 1;

    for ( auto it = _beg; it != _end; ++it )
    {
        if ( it == varray.end() )
            it = varray.begin();

        hdl.pushEnemyMissile( *( new Bullet( attack_val, spr, fspos, *it ) ) );
    }
}

void Airship::doubleSpinShot() noexcept
{
    const int N = 4;
    const std::size_t AIRSHIP_N = 2;

    using namespace lx::Graphics;
    lx::Graphics::ImgRect mbrect = {imgbox.p.x + AIRSHIP_SPIN_XOFF,
                                      imgbox.p.y + AIRSHIP_SPIN_YOFF,
                                      AIRSHIP_SPIN_DIM, AIRSHIP_SPIN_DIM
                                     };

    const lx::Physics::FloatPosition& p = lx::Physics::toFloatPosition( mbrect.p );

    lx::Graphics::Sprite * sprite[AIRSHIP_N];
    sprite[0] = ResourceManager::getInstance()->getResource( RC_MISSILE, AIRSHIP_SPIN1_ID );
    sprite[1] = ResourceManager::getInstance()->getResource( RC_MISSILE, AIRSHIP_SPIN2_ID );

    // Execute the pattern
    lx::Physics::Vector2D v[N];
    pattern1( p.x, p.y, v[0] );
    pattern2( p.x, p.y, v[1] );
    v[2] = -v[0];
    v[3] = -v[1];

    EntityHandler& hdl = EntityHandler::getInstance();

    for ( int i = 0; i < N; ++i )
    {
        hdl.pushEnemyMissile( *( new Bullet( attack_val, sprite[i % 2], mbrect, v[i] ) ) );
    }
}

void Airship::fire() noexcept
{
    switch ( idstrat )
    {
    case 1:
        bomb();
        break;

    case 2:
        frontShot();
        break;

    case 4:
        doubleSpinShot();
        break;

    default:
        break;
    }
}

/// End Fire

void Airship::die() noexcept
{
    if ( !dying )
    {
        if ( ( phybox.p.x + fbox<decltype( phybox.w )>( phybox.w ) ) > FNIL )
            EntityHandler::getInstance().bulletCancel();
    }

    Enemy::die();
}
