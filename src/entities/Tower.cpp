
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

#include "Tower.hpp"
#include "Bullet.hpp"
#include "Player.hpp"

#include "../asset/TX_Asset.hpp"

#include "../game/Balance.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../game/engine/AudioHandler.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Timer.hpp>

#include <algorithm>


namespace
{

const unsigned int DELAY_TOWER = 500;
const int TOWER_BULLET_ID = 4;
const float TOWER_BULLET_VEL = -7.0f;

using LX_Physics::LX_FloatPosition;
const std::vector<LX_FloatPosition> HPOINTS
{
    LX_FloatPosition{119, 43}, LX_FloatPosition{193, 90},
    LX_FloatPosition{218, 84}, LX_FloatPosition{191, 106},
    LX_FloatPosition{164, 175}, LX_FloatPosition{191, 270},
    LX_FloatPosition{230, 275}, LX_FloatPosition{230, 397},
    LX_FloatPosition{6, 397}, LX_FloatPosition{6, 275},
    LX_FloatPosition{45, 270}, LX_FloatPosition{68, 175},
    LX_FloatPosition{42, 106}, LX_FloatPosition{24, 84}, LX_FloatPosition{48, 90}
};

}


using namespace DynamicGameBalance;
using namespace LX_Graphics;
using namespace LX_Physics;
using namespace FloatBox;

Tower1::Tower1( unsigned int hp, unsigned int att, unsigned int sh,
                LX_Graphics::LX_Sprite * image, int x, int y, int w, int h,
                float vx, float vy )
    : BigEnemy( hp, att, sh, image, x, y, w, h, vx, vy ),
      shape( HPOINTS, LX_Physics::LX_FloatPosition{ fbox<int>( x ), fbox<int>( y ) } )
{
    addStrategy( new Tower1Strat( this ) );
}


void Tower1::move() noexcept
{
    Enemy::move();
    LX_Physics::movePoly( shape.getPoly(), speed );
}


void Tower1::collision( Missile * mi ) noexcept
{
    if ( !mi->isDead() && !mi->explosion()
            && mi->getX() <= ( phybox.p.x + fbox( phybox.w ) ) && !dying )
    {
        if ( LX_Physics::collisionBox( phybox, mi->getHitbox() ) )
        {
            if ( LX_Physics::collisionBoxPoly( mi->getHitbox(), shape.getPoly() ) )
            {
                if ( destroyable ) reaction( mi );
                mi->die();
            }
        }
    }
}

void Tower1::collision( Player * play ) noexcept
{
    if ( play->getX() <= ( phybox.p.x + fbox<decltype( phybox.w )>( phybox.w ) ) && !dying )
    {
        if ( LX_Physics::collisionCircleBox( play->getHitbox(), phybox ) )
        {
            if ( LX_Physics::collisionCirclePoly( play->getHitbox(), shape.getPoly() ) )
            {
                play->die();
            }
        }
    }
}


void Tower1::boom() noexcept
{
    AudioHandler::AudioHDL::getInstance()->playMediumExplosion();
}


void Tower1::draw() noexcept
{
    if ( dying )
    {
        const int N = 7;
        LX_Graphics::LX_ImgRect box[N] =
        {
            {64, 64, 64, 64}, {130, 100, 64, 64}, {60, 232, 64, 64}, {60, 120, 64, 64},
            {150, 80, 64, 64}, {130, 160, 64, 64}, {100, 256, 64, 64},
        };

        imgbox.p = LX_Graphics::toPixelPosition( phybox.p );

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

void Tower1::fire() noexcept
{
    const int N = 9;
    const Float BULLET_VEL = fbox( -7.0f );

    if ( isDead() )
        return;

    LX_Graphics::LX_ImgRect rect[2] =
    {
        {imgbox.p.x + 40, imgbox.p.y + 145, 24, 20},
        {imgbox.p.x + 40, imgbox.p.y + 185, 24, 20}
    };

    LX_Physics::LX_Vector2D velocity[N] =
    {
        {BULLET_VEL, FNIL}, {BULLET_VEL, -1.0f},
        {BULLET_VEL, 1.0f}, {BULLET_VEL, -2.0f}, {BULLET_VEL, 2.0f},
        {BULLET_VEL, -3.0f}, {BULLET_VEL, 3.0f}, {BULLET_VEL, -4.0f},
        {BULLET_VEL, 4.0f}
    };

    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Sprite * spr = rc->getResource( RC_MISSILE, TOWER_BULLET_ID );
    EntityHandler& hdl = EntityHandler::getInstance();

    for ( LX_Physics::LX_Vector2D& v : velocity )
    {
        hdl.pushEnemyMissile( *( new Bullet( attack_val, spr, rect[0], v ) ) );
        hdl.pushEnemyMissile( *( new Bullet( attack_val, spr, rect[1], v ) ) );
    }
}

void Tower1::die() noexcept
{
    if ( !dying )
    {
        if ( ( phybox.p.x + fbox<decltype( imgbox.w )>( imgbox.w ) ) > FNIL )
            EntityHandler::getInstance().bulletCancel();
    }

    Enemy::die();
}


// Strategy

Tower1Strat::Tower1Strat( Enemy * newEnemy )
    : Strategy( newEnemy )
{
    reference_time = 0;
}

void Tower1Strat::proceed() noexcept
{
    if ( ( LX_Timer::getTicks() - reference_time ) > DELAY_TOWER )
    {
        target->fire();
        reference_time = LX_Timer::getTicks();
    }
    target->move();
}
