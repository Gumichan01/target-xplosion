
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

#include "Bullet.hpp"

#include "Player.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../resources/ResourceManager.hpp"

#include <Lunatix/Texture.hpp>
#include <Lunatix/Physics.hpp>
#include <Lunatix/Hitbox.hpp>
#include <Lunatix/Time.hpp>

namespace
{
const int BULLET_ID = 4;
const int BULLET_MULTIPLIER = 4;

// Trail Bullet
const unsigned int DELAY_TRTIME = 100;
const unsigned int Y_MULT = 16;

// Spin Bullet
const int SPIN_BULLET_ID = 6;

// Mega Bullet
const unsigned int DELAY_MBTIME = 500;
const int BULLETS_DIM = 24;

// Gigabullets
const int NBX = 2;
const int NBY = 2;
Float GX_OFFSET[NBX] = {Float{ -100.0f}, Float{100.0f}};
Float GY_OFFSET[NBY] = {Float{ -100.0f}, Float{100.0f}};

}

using namespace lx::Physics;
using namespace FloatBox;

Bullet::Bullet( unsigned int pow, lx::Graphics::Sprite * image,
                lx::Graphics::ImgRect& rect, lx::Physics::Vector2D& sp )
    : Missile( pow, BULLET_MULTIPLIER, image, rect, sp ),
      bullet_time( lx::Time::getTicks() ) {}


void Bullet::draw() noexcept
{
    imgbox = lx::Graphics::toImgRect( phybox );

    if ( graphic != nullptr )
    {
        double angle = 0.0;

        if ( imgbox.w != imgbox.h )
            BulletPattern::calculateAngle( speed, angle );

        graphic->draw( imgbox, angle );
    }
}

/* ------------------------------
    TrailBullet implementation
   ------------------------------ */

TrailBullet::TrailBullet( unsigned int pow, lx::Graphics::Sprite * image,
                          lx::Graphics::ImgRect& rect, lx::Physics::Vector2D& sp )
    : Bullet( pow, image, rect, sp ) {}

void TrailBullet::move() noexcept
{
    if ( ( lx::Time::getTicks() - bullet_time ) > DELAY_TRTIME )
    {
        lx::Physics::Vector2D up = speed, down = speed;

        normalize( up );
        normalize( down );
        up   *= vector_norm( speed ) / Float{2.0f};
        down *= vector_norm( speed ) / Float{2.0f};

        const Float TMP{static_cast<float>( Y_MULT )};
        up.vy   -= vector_norm( speed ) / TMP;
        down.vy += vector_norm( speed ) / TMP;

        EntityHandler& hdl = EntityHandler::getInstance();
        hdl.pushEnemyMissile( *( new Bullet( power, graphic, imgbox, up ) ) );
        hdl.pushEnemyMissile( *( new Bullet( power, graphic, imgbox, down ) ) );

        bullet_time = lx::Time::getTicks();
    }

    Missile::move();
}


/* ------------------------------
    LunaticBullet implementation
   ------------------------------ */

LunaticBullet::LunaticBullet( unsigned int pow, lx::Graphics::Sprite * image,
                              lx::Graphics::ImgRect& rect, lx::Physics::Vector2D& sp )
    : Bullet( pow, image, rect, sp ), colour_time( lx::Time::getTicks() ),
      CTIME_LIMIT( vector_norm( sp ) * SPIN_BULLET_DELAY ), is_lunatic( true ) {}


void LunaticBullet::lunatic() noexcept
{
    if ( is_lunatic )
    {
        float norm = vector_norm( speed );
        const ResourceManager * rc = ResourceManager::getInstance();

        BulletPattern::shotOnPlayer( phybox.p.x, phybox.p.y, fbox( -norm ), speed );
        graphic = rc->getResource( RC_MISSILE, SPIN_BULLET_ID );
        is_lunatic = false;
    }
}


void LunaticBullet::move() noexcept
{
    if ( ( lx::Time::getTicks() - colour_time ) > CTIME_LIMIT )
    {
        lunatic();
        bullet_time = lx::Time::getTicks();
    }

    Missile::move();
}


/* ------------------------------
    MegaMBullet implementation
   ------------------------------ */


MegaBullet::MegaBullet( unsigned int pow, lx::Graphics::Sprite * image,
                        lx::Graphics::ImgRect& rect, lx::Physics::Vector2D& sp, float explosion_vel )
    : Bullet( pow, image, rect, sp ), circle_vel( fbox( explosion_vel ) ) {}


void MegaBullet::move() noexcept
{
    if ( ( lx::Time::getTicks() - bullet_time ) > DELAY_MBTIME )
    {
        if ( phybox.p.y >= FNIL && phybox.p.y <= Engine::getMaxYlim() )
            explosion();

        die();
    }
    else
        Missile::move();
}


void MegaBullet::explosion() noexcept
{
    std::array<lx::Physics::Vector2D, BulletPattern::CIRCLE_BULLETS> varray;
    lx::Graphics::ImgRect rect = {imgbox.p.x, imgbox.p.y, BULLETS_DIM, BULLETS_DIM};

    BulletPattern::circlePattern( phybox.p.x + fbox<int>( phybox.w / 2 ),
                                  phybox.p.y + fbox<int>( phybox.h / 2 ),
                                  circle_vel, varray );

    EntityHandler& hdl = EntityHandler::getInstance();
    const ResourceManager * rc = ResourceManager::getInstance();
    lx::Graphics::Sprite * spr = rc->getResource( RC_MISSILE, BULLET_ID );

    for ( lx::Physics::Vector2D& v : varray )
    {
        hdl.pushEnemyMissile( *( new Bullet( power, spr, rect, v ) ) );
    }
}


/* ------------------------------
    GigaBullet implementation
   ------------------------------ */

GigaBullet::GigaBullet( unsigned int pow, lx::Graphics::Sprite * image,
                        lx::Graphics::ImgRect& rect, lx::Physics::Vector2D& sp,
                        float explosion_mbv1, float explosion_mbv2 )
    : MegaBullet( pow, image, rect, sp, explosion_mbv2 ), vel( fbox( explosion_mbv1 ) ) {}


void GigaBullet::explosion() noexcept
{
    using namespace lx::Graphics;
    const ResourceManager * rc = ResourceManager::getInstance();

    EntityHandler& hdl = EntityHandler::getInstance();
    Sprite * spr = rc->getResource( RC_MISSILE, BULLET_ID );
    lx::Graphics::ImgRect rect{{imgbox.p.x, imgbox.p.y}, BULLETS_DIM, BULLETS_DIM};

    lx::Physics::FloatPosition p = phybox.p;
    p.x += fbox<int>( phybox.w / 2 );
    p.y += fbox<int>( phybox.h / 2 );

    lx::Physics::Vector2D v[4] = {lx::Physics::Vector2D{{0.0f}, {0.0f}}};
    int k = 0;

    for ( int i = 0; i < NBX; i++ )
    {
        for ( int j = 0; j < NBY; j++ )
        {
            k = i + j + ( i == 0 ? 0 : 1 );
            BulletPattern::shotOnTarget( p.x, p.y, p.x + GX_OFFSET[i],
                                         p.y + GY_OFFSET[j], fbox( vel ), v[k] );
            hdl.pushEnemyMissile( *( new MegaBullet( power, spr, rect, v[k], circle_vel ) ) );
        }
    }
}
