
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


#include "Boss01.hpp"
#include "../Player.hpp"
#include "../BasicMissile.hpp"
#include "../../game/Balance.hpp"
#include "../../pattern/BulletPattern.hpp"
#include "../../game/engine/AudioHandler.hpp"
#include "../../resources/ResourceManager.hpp"

#include <Lunatix/Physics.hpp>
#include <Lunatix/Time.hpp>


namespace
{
const int BOSS01_SIDES = 2;
const int BOSS01_LBULLET_ID = 4;
const int BOSS01_RBULLET_ID = 8;
const int BOSS01_PBULLET_ID = 9;
const int BOSS01_SPRITE_DID = 3;

// Limits of the boss's action area
const Float BOSS01_MIN_XPOS = {876.0f};
const Float BOSS01_MAX_XPOS = {896.0f};
const Float BOSS01_MIN_YPOS = {152.0f};
const Float BOSS01_MAX_YPOS = {160.0f};

// Delays fot the Circle01Strat
const unsigned int BOSS01_WSHOT_DELAY = 500;    // delay between two shots
const unsigned int BOSS01_WSHOT_TDELAY = 2100;  // total delay

// Boss01Circle02Strat
const int BOSS01_XLIM = 300;
const int BOSS01_YLIM_UP = 71;
const int BOSS01_YLIM_DOWN = 300;
const int BOSS01_SCIRCLE_VEL = 2;
const Float BOSS01_SCIRCLE_BVEL = {6.0f};
const Float BOSS01_KILL_VEL = { -7.5f};

// These values are used in order to set the position of the missiles
const int BOSS01_XOFF = 90;
const int BOSS01_YOFF1 = 54;
const int BOSS01_YOFF2 = 370;

const unsigned int MOVE_DELAY = 9000;
const unsigned int TOTAL_MOVE_DELAY = MOVE_DELAY + 2000;
const unsigned int BOSS01_SCIRCLE_DELAY = 1000;
const unsigned int BOSS01_DELAY_NOISE = 640;

const int BOSS01_VMULT = 4;
const int BOSS01_BULLET_DIM = 24;

const int BOSS01_BCIRCLE_N = 4;
const int BOSS01_BCIRCLE_XOFF = 98;
const int BOSS01_BCIRCLE_YOFF[4] = {134, 174, 260, 302};
const size_t BOSS01_BCIRCLE_NUM = BulletPattern::CIRCLE_BULLETS;

using lx::Physics::FloatPosition;
const std::vector<lx::Physics::FloatPosition> HPOINTS
{
    lx::Physics::FloatPosition{48, 224}, lx::Physics::FloatPosition{60, 162},
    lx::Physics::FloatPosition{24, 87}, lx::Physics::FloatPosition{106, 42},
    lx::Physics::FloatPosition{182, 87}, lx::Physics::FloatPosition{151, 162},
    lx::Physics::FloatPosition{162, 224}, lx::Physics::FloatPosition{151, 281},
    lx::Physics::FloatPosition{182, 357}, lx::Physics::FloatPosition{106, 406},
    lx::Physics::FloatPosition{24, 357}, lx::Physics::FloatPosition{60, 285}
};

// The half of health points of the boss
inline constexpr unsigned int halfLife( unsigned int n )
{
    return n / 2U;
}

}

using namespace DynamicGameBalance;
using namespace AudioHandler;
using namespace lx::Physics;
using namespace FloatBox;


/* ------------------------
            Boss 01
   ------------------------ */
Boss01::Boss01( unsigned int hp, unsigned int att, unsigned int sh,
                lx::Graphics::Sprite * image, int x, int y, int w, int h,
                float vx, float vy )
    : Boss( hp, att, sh, image, x, y, w, h, vx, vy ), bshield( false ), scircle_time( 0 ),
      circle01_time( 0 ), shape( HPOINTS, lx::Physics::FloatPosition{fbox( x ), fbox( y )} ), id_pos( 0 )
{
    id_strat = 1;   // Set the first strategy ID
    addStrategy( new Boss01PositionStrat( this ) );
}


// Shoot two lines of bullets around the boss
void Boss01::sideCircleShot() noexcept
{
    lx::Graphics::ImgRect rect[BOSS01_SIDES];
    int sp_offset = static_cast<int>( speed.vy );

    rect[0] =
    {
        imgbox.p.x + BOSS01_XOFF, imgbox.p.y + BOSS01_YOFF1 + sp_offset,
        BOSS01_BULLET_DIM, BOSS01_BULLET_DIM
    };
    rect[1] =
    {
        imgbox.p.x + BOSS01_XOFF, imgbox.p.y + BOSS01_YOFF2 + sp_offset,
        BOSS01_BULLET_DIM, BOSS01_BULLET_DIM
    };

    std::array<lx::Physics::Vector2D, BOSS01_BCIRCLE_NUM> varray;
    BulletPattern::circlePattern( fbox( rect[0].p.x ), fbox( rect[0].p.y ),
                                  apply_dgb( BOSS01_SCIRCLE_BVEL ), varray );

    const ResourceManager * rc = ResourceManager::getInstance();
    lx::Graphics::Sprite * spr = rc->getResource( RC_MISSILE, BOSS01_RBULLET_ID );
    EntityHandler& hdl = EntityHandler::getInstance();

    for ( lx::Physics::Vector2D& v : varray )
    {
        hdl.pushEnemyMissile( *( new BasicMissile( attack_val, spr, rect[0], v ) ) );
        hdl.pushEnemyMissile( *( new BasicMissile( attack_val, spr, rect[1], v ) ) );
    }
}


void Boss01::shootToKill() noexcept
{
    lx::Graphics::ImgRect rect[BOSS01_BCIRCLE_N];

    for ( int i = 0; i < BOSS01_BCIRCLE_N; i++ )
    {
        // X position and dimension
        rect[i].p.x = imgbox.p.x + BOSS01_BCIRCLE_XOFF;
        rect[i].p.y = imgbox.p.y + BOSS01_BCIRCLE_YOFF[i];
        rect[i].w = BOSS01_BULLET_DIM;
        rect[i].h = BOSS01_BULLET_DIM;
    }

    lx::Physics::FloatPosition p =
    {
        phybox.p.x + fbox<int>( phybox.w / 2 ),
        phybox.p.y + fbox<int>( phybox.h / 2 )
    };

    lx::Physics::Vector2D v;
    BulletPattern::shotOnPlayer( p.x, p.y, apply_dgb( BOSS01_KILL_VEL ), v );

    const ResourceManager * const rc = ResourceManager::getInstance();
    lx::Graphics::Sprite * s = rc->getResource( RC_MISSILE, BOSS01_PBULLET_ID );
    EntityHandler& hdl = EntityHandler::getInstance();

    for ( lx::Graphics::ImgRect& box : rect )
    {
        hdl.pushEnemyMissile( *( new Bullet( attack_val, s, box, v ) ) );
    }
}

void Boss01::bulletCircleShot() noexcept
{
    lx::Graphics::ImgRect rect[BOSS01_BCIRCLE_N];

    for ( int i = 0; i < BOSS01_BCIRCLE_N; i++ )
    {
        // X position and dimension
        rect[i].p.x = imgbox.p.x + BOSS01_BCIRCLE_XOFF;
        rect[i].p.y = imgbox.p.y + BOSS01_BCIRCLE_YOFF[i];
        rect[i].w = BOSS01_BULLET_DIM;
        rect[i].h = BOSS01_BULLET_DIM;
    }

    int j = id_pos++;
    std::array<lx::Physics::Vector2D, BOSS01_BCIRCLE_NUM> varray;
    BulletPattern::circlePattern( fbox<int>( rect[j].p.x ),
                                  fbox<int>( rect[j].p.y ),
                                  apply_dgb( BOSS01_SCIRCLE_BVEL ), varray );

    const ResourceManager * const rc = ResourceManager::getInstance();
    lx::Graphics::Sprite * spr = rc->getResource( RC_MISSILE, BOSS01_LBULLET_ID );
    EntityHandler& hdl = EntityHandler::getInstance();

    for ( lx::Physics::Vector2D& v : varray )
    {
        hdl.pushEnemyMissile( *( new Bullet( attack_val, spr, rect[j], v ) ) );
    }

    if ( id_pos == BOSS01_BCIRCLE_N )
        id_pos = 0;
}

// Default shot, circle bullets
void Boss01::fire() noexcept
{
    switch ( id_strat )
    {
    case 2:
        bulletCircleShot();
        break;

    case 3:
        sideCircleShot();
        break;

    case 4:
        sideCircleShot();
        shootToKill();
        break;

    default:
        break;
    }
}


void Boss01::bposition() noexcept
{
    if ( phybox.p.x >= BOSS01_MIN_XPOS && phybox.p.x <= BOSS01_MAX_XPOS
            && phybox.p.y >= BOSS01_MIN_YPOS && phybox.p.y <= BOSS01_MAX_YPOS )
    {
        // Use the second strategy
        id_strat = 2;
        bshield = false;
        addStrategy( new Boss01Circle01Strat( this ) );
        circle01_time = lx::Time::getTicks();
    }
}

void Boss01::circle01() noexcept
{
    if ( ( lx::Time::getTicks() - circle01_time ) > BOSS01_WSHOT_TDELAY )
    {
        // Use the third strategy
        id_strat = ( health_point < halfLife( max_health_point ) ) ? 4 : 3;
        speed *= FNIL;
        addStrategy( new Boss01Circle02Strat( this ) );
        scircle_time = lx::Time::getTicks();
    }
}

void Boss01::circle02() noexcept
{
    if ( ( lx::Time::getTicks() - scircle_time ) > TOTAL_MOVE_DELAY )
    {
        // First strategy
        id_strat = 1;
        bshield = true;
        addStrategy( new Boss01PositionStrat( this ) );
    }
}


void Boss01::strategy() noexcept
{
    if ( !dying )
    {
        switch ( id_strat )
        {
        case 1:
            bposition();
            break;

        case 2:
            circle01();
            break;

        case 3:
            circle02();
            break;

        default:
            break;
        }
    }

    Boss::strategy();
}


void Boss01::move() noexcept
{
    movePoly( shape.getPoly(), speed );
    Enemy::move();
}

void Boss01::collision( Missile * mi ) noexcept
{
    const lx::Physics::FloatingBox& FBOX = mi->getHitbox();

    // no shield + no dead missile + missile can hit + basic collision
    if ( !mi->isDead() && !mi->explosion() && mustCheckCollision()
            && FBOX.p.x <= ( phybox.p.x + fbox( phybox.w ) )
            && collisionBox( phybox, FBOX ) )
    {
        if ( collisionBoxPoly( FBOX, shape.getPoly() ) )
        {
            if ( destroyable && !bshield )
                reaction( mi );

            mi->die();
        }
    }
}

void Boss01::collision( Player * play ) noexcept
{
    if ( !mustCheckCollision() )
        return;

    const lx::Physics::Circle& BOX = play->getHitbox();

    if ( !play->isDead() && play->getX() <= ( phybox.p.x + fbox( phybox.w ) )
            && collisionCircleBox( BOX, phybox ) )
    {
        if ( collisionCirclePoly( BOX, shape.getPoly() ) )
            play->die();
    }
}

void Boss01::die() noexcept
{
    if ( !dying )
    {
        const ResourceManager * rc = ResourceManager::getInstance();
        graphic = rc->getResource( RC_XPLOSION, BOSS01_SPRITE_DID );
        AudioHDL::getInstance()->stopBossMusic();
        AudioHDL::getInstance()->playVoiceMother();
        addStrategy( new BossDeathStrategy( this, DEFAULT_XPLOSION_DELAY,
                                            BOSS01_DELAY_NOISE ) );
    }

    Boss::die();
}


/* ------------------------
        Boss Strategy
   ------------------------ */

/* Position */
Boss01PositionStrat::Boss01PositionStrat( Boss01 * newEnemy )
    : Strategy( newEnemy ), BossStrategy( newEnemy ) {}

Boss01PositionStrat::~Boss01PositionStrat() {}


void Boss01PositionStrat::proceed() noexcept
{
    const float SPEED_X2 = 2.0f;
    const float SPEED_X3 = 3.0f;
    lx::Physics::Vector2D v{2.0f, 1.0f};

    if ( boss->getHP() < halfLife( halfLife( boss->getMaxHP() ) ) )
        v *= SPEED_X3;

    else if ( boss->getHP() < halfLife( boss->getMaxHP() ) )
        v *= SPEED_X2;

    // X position
    if ( boss->getX() > BOSS01_MAX_XPOS )
        boss->setXvel( -v.vx );

    else if ( boss->getX() < BOSS01_MIN_XPOS )
        boss->setXvel( v.vx );

    else
        boss->setXvel( 0 );

    // Y position
    if ( boss->getY() > BOSS01_MAX_YPOS )
        boss->setYvel( -v.vy );

    else if ( boss->getY() < BOSS01_MIN_YPOS )
        boss->setYvel( v.vy );

    else
        boss->setYvel( 0 );

    // Move normally
    boss->move();
}


/* Shoot */
Boss01Circle01Strat::Boss01Circle01Strat( Boss01 * newEnemy )
    : Strategy( newEnemy ), BossStrategy( newEnemy ), begin_circle01( 0 ), first( true ) {}

Boss01Circle01Strat::~Boss01Circle01Strat() {}


void Boss01Circle01Strat::proceed() noexcept
{
    unsigned int delay = BOSS01_WSHOT_DELAY;
    unsigned int total_delay = BOSS01_WSHOT_TDELAY;

    if ( first )
    {
        begin_circle01 = lx::Time::getTicks();
        first = false;
    }

    if ( boss->getHP() < halfLife( boss->getMaxHP() ) )
        total_delay *= 2;

    if ( boss->getHP() < halfLife( halfLife( boss->getMaxHP() ) ) )
    {
        total_delay *= 4;
        delay -= 100;
    }

    if ( ( lx::Time::getTicks() - begin_circle01 ) < total_delay )
    {
        static unsigned int wtime_tmp = 0;
        if ( ( lx::Time::getTicks() - wtime_tmp ) > delay )
        {
            target->fire();
            wtime_tmp = lx::Time::getTicks();
        }
    }
}


/* Row */
Boss01Circle02Strat::Boss01Circle02Strat( Boss01 * newEnemy )
    : Strategy( newEnemy ), BossStrategy( newEnemy ), first( true ), begin_scircle( 0 ),
      mv( new UpDownMoveStrategy( newEnemy, BOSS01_YLIM_UP, BOSS01_YLIM_DOWN,
                                  BOSS01_SCIRCLE_VEL ) ) {}

Boss01Circle02Strat::~Boss01Circle02Strat()
{
    delete mv;
}

void Boss01Circle02Strat::proceed() noexcept
{
    static unsigned int t = 0;

    if ( first )
    {
        begin_scircle = lx::Time::getTicks();
        first = false;
    }

    if ( ( lx::Time::getTicks() - begin_scircle ) > MOVE_DELAY )
    {
        // Go to the left
        int v = 2;
        boss->setXvel( -v * BOSS01_VMULT );
        boss->setYvel( 0 );
    }
    else
    {
        if ( ( lx::Time::getTicks() - t ) > BOSS01_SCIRCLE_DELAY )
        {
            target->fire();
            t = lx::Time::getTicks();
        }
    }

    // On the left of the screen
    if ( boss->getX() < BOSS01_XLIM )
    {
        boss->setXvel( 0 );
    }

    mv->proceed();
}
