
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

#include "Boss04.hpp"
#include "../Player.hpp"
#include "../../pattern/BulletPattern.hpp"
#include "../../game/Scoring.hpp"
#include "../../game/engine/Hud.hpp"
#include "../../game/engine/Engine.hpp"
#include "../../game/engine/EntityHandler.hpp"
#include "../../resources/ResourceManager.hpp"

#include <Lunatix/Physics.hpp>
#include <Lunatix/Time.hpp>

#define FL(x)  static_cast<float>(x)
#define UIL(x) static_cast<unsigned int>(x)

using namespace lx::Graphics;
using namespace lx::Physics;
using namespace FloatBox;


namespace
{

const int BOSS04_SHID = 8;
const int BOSS04_NOSHID = 9;
const int BOSS04_XSHID = 6;
const int BOSS04_YBULLET_ID = 6;
const int BOSS04_RBULLET_ID = 8;
const int BOSS04_BBULLET_ID = 5;

const int BOSS04_XLIM = 641;
const unsigned int BOSS04_RAD = 285;
// Core hitbox
const float BOSS04_XCORE = 320;
const float BOSS04_YCORE = 320;
const unsigned int BOSS04_CRAD = 133;    // radius of the core hitbox

// Sentinels
const int BOSS04_SENTINELS = 8;
const int BOSS04_SRAD = 32;

// Bullets
const int BOSS04_BULLETS_DIM = 24;
const int BOSS04_BULLETS2_DIM = 16;

lx::Physics::Circle sentinel_hbox[BOSS04_SENTINELS] =
{
    lx::Physics::Circle{lx::Physics::FloatPosition{140, 140}, BOSS04_SRAD},
    lx::Physics::Circle{lx::Physics::FloatPosition{320, 68}, BOSS04_SRAD},
    lx::Physics::Circle{lx::Physics::FloatPosition{500, 140}, BOSS04_SRAD},
    lx::Physics::Circle{lx::Physics::FloatPosition{572, 320}, BOSS04_SRAD},
    lx::Physics::Circle{lx::Physics::FloatPosition{500, 500}, BOSS04_SRAD},
    lx::Physics::Circle{lx::Physics::FloatPosition{320, 552}, BOSS04_SRAD},
    lx::Physics::Circle{lx::Physics::FloatPosition{140, 500}, BOSS04_SRAD},
    lx::Physics::Circle{lx::Physics::FloatPosition{68, 320}, BOSS04_SRAD},
};

lx::Physics::FloatPosition sentinel_src[BOSS04_SENTINELS] =
{
    lx::Physics::FloatPosition{140, 140}, lx::Physics::FloatPosition{320, 68},
    lx::Physics::FloatPosition{500, 140}, lx::Physics::FloatPosition{572, 320},
    lx::Physics::FloatPosition{500, 500}, lx::Physics::FloatPosition{320, 552},
    lx::Physics::FloatPosition{140, 500}, lx::Physics::FloatPosition{68, 320},
};

lx::Graphics::ImgRect rbullets[BOSS04_SENTINELS] =
{
    {284, 237, BOSS04_BULLETS2_DIM, BOSS04_BULLETS2_DIM},
    {284, 403, BOSS04_BULLETS2_DIM, BOSS04_BULLETS2_DIM},
    {342, 237, BOSS04_BULLETS2_DIM, BOSS04_BULLETS2_DIM},
    {342, 403, BOSS04_BULLETS2_DIM, BOSS04_BULLETS2_DIM},
    {237, 284, BOSS04_BULLETS2_DIM, BOSS04_BULLETS2_DIM},
    {237, 356, BOSS04_BULLETS2_DIM, BOSS04_BULLETS2_DIM},
    {389, 284, BOSS04_BULLETS2_DIM, BOSS04_BULLETS2_DIM},
    {389, 356, BOSS04_BULLETS2_DIM, BOSS04_BULLETS2_DIM}
};

lx::Physics::FloatPosition bfpos[BOSS04_SENTINELS];

/// Shot on target
// Shot wave duration
const unsigned int BOSS04_DSHOT = 2000;
// Duration between two shot waves
const unsigned int BOSS04_DSHOT_DELAY = 2000;
// Duration between each shot
const unsigned int BOSS04_DBSHOT = 100;
// Bullet velocity
const Float BOSS04_DSHOT_BVEL = { -16.0f};

/// Bullets
const unsigned int BOSS04_BSHOT_DELAY = 2000;
const float BOSS04_BSHOT_BVEL = -8.0f;
const int BOSS04_MBSHOT_BVEL = 10;

const int BOSS04_MBSHOT_OFFX = 312;
const int BOSS04_MBSHOT_OFFY = 311;

/// Reload

// Duration between each heal
const unsigned int BOSS04_DHEAL = 100;
const unsigned int BOSS04_DAMAGES_RATIO = 2;

/// Remove the sentinels
const unsigned int BOSS03_XSH_DELAY = 750;

/// Unleash
constexpr Float step = BulletPattern::PI_F / fbox( 24.0f );
const Float BOSS04_RF = {100.0f};
const Float BOSS04_USHOT_BVEL = { -4.0f};

const unsigned int BOSS04_USHOT_NDELAY = 200;
const unsigned int BOSS04_USHOT_HDELAY = 100;
const unsigned int BOSS04_USHOT_XDELAY = 50;

}


Boss04::Boss04( unsigned int hp, unsigned int att, unsigned int sh,
                lx::Graphics::Sprite * image, int x, int y, int w, int h,
                float vx, float vy )
    : Boss( hp, att, sh, image, x, y, w, h, vx, vy ),
      HEALTH_80( UIL( FL( max_health_point ) * 0.8f ) ),
      HEALTH_55( UIL( FL( max_health_point ) * 0.55f ) ),
      HEALTH_25( UIL( FL( max_health_point ) * 0.25f ) ),
      shield( true ), alpha( FNIL ), shield_points( max_health_point ),
      core_hbox{lx::Physics::FloatPosition{BOSS04_XCORE, BOSS04_YCORE}, BOSS04_CRAD},
      asprite( nullptr ), asprite_sh( nullptr ), asprite_x( nullptr ), asprite_nosh( nullptr )
{
    addStrategy( new MoveStrategy( this ) );

    // reduce the hitbox + set the core hitbox
    circle_box.radius = BOSS04_RAD;
    const lx::Physics::FloatPosition P = {phybox.p.x + core_hbox.center.x,
                                phybox.p.y + core_hbox.center.y
                               };
    moveCircleTo( core_hbox, P );

    // set the hitbox of each sentinel
    size_t i = 0;
    for ( lx::Physics::Circle& C : sentinel_hbox )
    {
        moveCircleTo( C, P );
        bfpos[i++] = C.center;
    }

    //core_fpos = core_hbox.center;

    // graphics assets of the boss
    const ResourceManager * const RC = ResourceManager::getInstance();
    asprite_sh = RC->getResource( RC_ENEMY, BOSS04_SHID );
    asprite_nosh = RC->getResource( RC_ENEMY, BOSS04_NOSHID );
    asprite_x = RC->getResource( RC_XPLOSION, BOSS04_XSHID );

    asprite = graphic;
    graphic = asprite_sh;
}


void Boss04::shotOnTarget() noexcept
{
    lx::Physics::Vector2D bvel[BOSS04_SENTINELS];
    lx::Graphics::ImgRect brect[BOSS04_SENTINELS];
    EntityHandler& hdl = EntityHandler::getInstance();
    Sprite * bsp = ResourceManager::getInstance()->getResource( RC_MISSILE, BOSS04_YBULLET_ID );

    size_t i = 0;
    for ( const lx::Physics::FloatPosition& FP : sentinel_src )
    {
        BulletPattern::shotOnPlayer( FP.x, FP.y, BOSS04_DSHOT_BVEL, bvel[i] );
        brect[i] = {{static_cast<int>( FP.x ), static_cast<int>( FP.y )},
            BOSS04_BULLETS_DIM, BOSS04_BULLETS_DIM
        };

        hdl.pushEnemyMissile( *( new Bullet( attack_val, bsp, brect[i], bvel[i] ) ) );
        i++;
    }
}

void Boss04::bullets() noexcept
{
    lx::Physics::Vector2D v{BOSS04_BSHOT_BVEL, FNIL};
    EntityHandler& hdl = EntityHandler::getInstance();
    Sprite * bsp = ResourceManager::getInstance()->getResource( RC_MISSILE, BOSS04_RBULLET_ID );

    for ( lx::Graphics::ImgRect& rbox : rbullets )
        hdl.pushEnemyMissile( *( new Bullet( attack_val, bsp, rbox, v ) ) );
}

void Boss04::mbullets() noexcept
{
    lx::Physics::Vector2D v;
    lx::Graphics::ImgRect mbrect = {imgbox.p.x + BOSS04_MBSHOT_OFFX,
                                      imgbox.p.y + BOSS04_MBSHOT_OFFY,
                                      BOSS04_BULLETS2_DIM, BOSS04_BULLETS2_DIM
                                     };

    Sprite * bsp = ResourceManager::getInstance()->getResource( RC_MISSILE, BOSS04_BBULLET_ID );
    EntityHandler& hdl = EntityHandler::getInstance();
    hdl.pushEnemyMissile( *( new MegaBullet( attack_val, bsp, mbrect, v, BOSS04_MBSHOT_BVEL ) ) );
}

void Boss04::reload() noexcept
{
    const unsigned int V = 512;

    if ( shield_points > 0 )
    {
        if ( health_point + V > max_health_point )
            health_point = max_health_point;
        else
            health_point += V;

        hud->update();
    }
}

void Boss04::unleash() noexcept
{
    lx::Physics::Vector2D v;
    const lx::Physics::FloatPosition P = {fbox<int>( imgbox.p.x + BOSS04_MBSHOT_OFFX ),
                                fbox<int>( imgbox.p.y + BOSS04_MBSHOT_OFFY )
                               };


    BulletPattern::shotOnTarget( P.x, P.y, P.x + FloatMath::cos( alpha ) * BOSS04_RF,
                                 P.y - FloatMath::sin( alpha ) * BOSS04_RF,
                                 BOSS04_USHOT_BVEL, v );

    if ( alpha > fbox<decltype( BulletPattern::PI )>( BulletPattern::PI ) * fbox( 2.0f ) )
    {
        alpha = FNIL;
        bullets();
    }

    alpha += step;

    EntityHandler& hdl = EntityHandler::getInstance();
    const lx::Graphics::ImgCoord PP = lx::Graphics::toPixelPosition( P );
    lx::Graphics::ImgRect mbrect = {PP.x, PP.y, BOSS04_BULLETS2_DIM, BOSS04_BULLETS2_DIM};
    Sprite * bsp = ResourceManager::getInstance()->getResource( RC_MISSILE, BOSS04_BBULLET_ID );

    hdl.pushEnemyMissile( *( new MegaBullet( attack_val, bsp, mbrect, v, BOSS04_MBSHOT_BVEL ) ) );
}


void Boss04::stratPos() noexcept
{
    if ( imgbox.p.x < BOSS04_XLIM )
    {
        id_strat = 1;
        shield = false;
        graphic = asprite;
        EntityHandler::getInstance().bulletCancel();
        addStrategy( new Boss04Shot( this ) );

        for ( int i = 0; i < BOSS04_SENTINELS; i++ )
        {
            const lx::Physics::FloatPosition SENT_P =
            {
                fbox<int>( imgbox.p.x ) + sentinel_src[i].x,
                fbox<int>( imgbox.p.y ) + sentinel_src[i].y
            };

            const lx::Physics::FloatPosition BULL_P =
            {
                fbox<int>( imgbox.p.x + rbullets[i].p.x ),
                fbox<int>( imgbox.p.y + rbullets[i].p.y )
            };

            movePointTo( sentinel_src[i], SENT_P );

            {
                FloatingBox tmp = toFloatingBox( rbullets[i] );
                moveBoxTo( tmp, BULL_P );
                rbullets[i] = lx::Graphics::toImgRect( tmp );
            }
        }
    }
}

void Boss04::stratReload() noexcept
{
    if ( health_point == max_health_point || shield_points == 0 )
    {
        shield = false;

        if ( shield_points == 0 )
        {
            id_strat = 5;
            graphic = asprite_x;
            addStrategy( new Boss04Break( this ) );
        }
        else if ( health_point == max_health_point )
        {
            id_strat = 1;
            graphic = asprite;
            addStrategy( new Boss04Shot( this ) );
        }

        EntityHandler::getInstance().bulletCancel();
    }
}

void Boss04::stratX() noexcept
{
    id_strat = 6;
    graphic = asprite_nosh;
    ShotStrategy * sht = new ShotStrategy( this );
    sht->setShotDelay( BOSS04_USHOT_NDELAY );

    if ( health_point < HEALTH_55 )
        sht->setShotDelay( BOSS04_USHOT_HDELAY );

    else if ( health_point < HEALTH_25 )
        sht->setShotDelay( BOSS04_USHOT_XDELAY );

    addStrategy( sht );
}

void Boss04::stratUnleash() noexcept
{
    static unsigned int prev_health = 0;

    if ( health_point < HEALTH_80 && prev_health >= HEALTH_80 )
    {
        EntityHandler::getInstance().bulletCancel();
        ShotStrategy * sht = new ShotStrategy( this );

        sht->setShotDelay( BOSS04_USHOT_NDELAY );
        addStrategy( sht );
    }

    if ( health_point < HEALTH_55 && prev_health >= HEALTH_55 )
    {
        EntityHandler::getInstance().bulletCancel();
        ShotStrategy * sht = new ShotStrategy( this );

        sht->setShotDelay( BOSS04_USHOT_HDELAY );
        addStrategy( sht );
    }

    if ( health_point < HEALTH_25 && prev_health >= HEALTH_25 )
    {
        EntityHandler::getInstance().bulletCancel();
        ShotStrategy * sht = new ShotStrategy( this );

        sht->setShotDelay( BOSS04_USHOT_XDELAY );
        addStrategy( sht );
    }

    prev_health = health_point;
}


void Boss04::fire() noexcept
{
    switch ( id_strat )
    {
    case 2:
        bullets();
        break;

    case 3:
        bullets();
        mbullets();
        break;

    case 4:
        reload();
        break;

    case 6:
        unleash();
        break;

    default:
        shotOnTarget();
        break;
    }
}

void Boss04::strategy() noexcept
{
    if ( id_strat == 0 )
        stratPos();

    else if ( id_strat == 4 ) // Shield
        stratReload();

    else if ( id_strat == 6 ) // Shield destroyed
        stratUnleash();

    else
    {
        if ( id_strat == 1 && health_point < HEALTH_80 ) // Shot on target
        {
            id_strat = 2;
            addStrategy( new Boss04Shot2( this ) );
            EntityHandler::getInstance().bulletCancel();
        }
        else if ( id_strat == 2 && health_point < HEALTH_55 ) // Shot on target + Bullets
        {
            id_strat = 3;
            EntityHandler::getInstance().bulletCancel();
        }
        else if ( id_strat == 3 && health_point < HEALTH_25 && shield_points > 0 )
        {
            // Shot on target + Bullets + Megabullets
            id_strat = 4;
            shield = true;
            graphic = asprite_sh;
            addStrategy( new Boss04Reload( this ) );
            EntityHandler::getInstance().bulletCancel();
        }
    }

    Boss::strategy();
}

void Boss04::move() noexcept
{
    moveCircle( core_hbox, speed );

    for ( lx::Physics::Circle& sbox : sentinel_hbox )
    {
        moveCircle( sbox, speed );
    }

    Enemy::move();
}

void Boss04::collision( Missile * mi ) noexcept
{
    const lx::Physics::FloatingBox& BOX = mi->getHitbox();

    if ( id_strat == 5 )
        return;

    if ( shield_points > 0 )
    {
        if ( collisionCircleBox( circle_box, BOX ) )
        {
            if ( shield )
            {
                int _hit = static_cast<int>( mi->hit() / BOSS04_DAMAGES_RATIO );
                int d = static_cast<int>( shield_points ) - _hit;
                shield_points = static_cast<unsigned int>( d < 0 ? 0 : d );
                mi->die();
            }

            if ( collisionCircleBox( core_hbox, BOX ) )
            {
                if ( destroyable ) reaction( mi );
                mi->die();
            }
            else
            {
                for ( const lx::Physics::Circle& C : sentinel_hbox )
                {
                    if ( collisionCircleBox( C, BOX ) )
                    {
                        mi->die();
                        break;
                    }
                }
            }
        }
    }
    else
    {
        if ( collisionCircleBox( core_hbox, BOX ) )
        {
            if ( destroyable )
                reaction( mi );

            mi->die();
        }
    }
}

void Boss04::collision( Player * play ) noexcept
{
    if ( shield_points > 0 )
        Enemy::collision( play );
    else
    {
        if ( collisionCircle( core_hbox, play->getHitbox() ) )
        {
            play->die();
        }
    }
}

void Boss04::reaction( Missile * target ) noexcept
{
    if ( shield )
    {
        Score * sc = Engine::getInstance()->getScore();
        receiveDamages( target->hit() / BOSS04_DAMAGES_RATIO );
        sc->notify( Scoring::DAMAGE_SCORE );
    }
    else
        Boss::reaction( target );
}

void Boss04::die() noexcept
{
    /// @todo (#9#) v0.x.y: destruction of the boss #X
    Enemy::die();
}


/* --------------------
    Boss04 strategies
   -------------------- */

Boss04Shot::Boss04Shot( Boss04 * nboss )
    : Strategy( nboss ), BossStrategy( nboss ), boss04( nboss ),
      shot_t( 0 ), wave_t( 0 ), pause_t( 0 ), shoot( true )
{
    shot_t = lx::Time::getTicks();
    wave_t = lx::Time::getTicks();
    pause_t = lx::Time::getTicks();
}


void Boss04Shot::proceed() noexcept
{
    if ( shoot )
    {
        if ( ( lx::Time::getTicks() - wave_t ) < BOSS04_DSHOT )
        {
            if ( ( lx::Time::getTicks() - shot_t ) > BOSS04_DBSHOT )
            {
                boss04->shotOnTarget();
                shot_t = lx::Time::getTicks();
            }
        }
        else
        {
            shoot = false;
            pause_t = lx::Time::getTicks();
        }
    }
    else
    {
        if ( ( lx::Time::getTicks() - pause_t ) > BOSS04_DSHOT_DELAY )
        {
            shoot = true;
            wave_t = lx::Time::getTicks();
            shot_t = lx::Time::getTicks();
        }
    }
}

// Bullets
Boss04Shot2::Boss04Shot2( Boss04 * nboss )
    : Strategy( nboss ), BossStrategy( nboss ), bsstrat( nboss ), bbstrat( nboss )

{
    bbstrat.setShotDelay( BOSS04_BSHOT_DELAY );
}

void Boss04Shot2::proceed() noexcept
{
    bsstrat.proceed();
    bbstrat.proceed();
}


Boss04Break::Boss04Break( Boss04 * nboss )
    : Strategy( nboss ), BossStrategy( nboss ), boss04( nboss ),
      xtime( lx::Time::getTicks() ) {}

void Boss04Break::proceed() noexcept
{
    if ( ( lx::Time::getTicks() - xtime ) > BOSS03_XSH_DELAY )
    {
        boss04->stratX();
    }
}


// Reload the life points of the boss
Boss04Reload::Boss04Reload( Boss04 * nboss )
    : Strategy( nboss ), BossStrategy( nboss ), t( lx::Time::getTicks() ) {}


void Boss04Reload::proceed() noexcept
{
    if ( ( lx::Time::getTicks() - t ) > BOSS04_DHEAL )
    {
        target->fire();
        t = lx::Time::getTicks();
    }
}
