
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

#include "Bachi.hpp"
#include "Bullet.hpp"
#include "Item.hpp"
#include "PlayerVisitor.hpp"
#include "../game/Balance.hpp"

#include "../game/engine/EntityHandler.hpp"
#include "../entities/Player.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../pattern/Strategy.hpp"
#include "../resources/ResourceManager.hpp"

#include <Lunatix/Texture.hpp>

using namespace lx::Physics;
using namespace FloatBox;
using namespace DynamicGameBalance;

namespace
{
const int BACHI_BULLET_OFFSET_X = 8;
const int BACHI_BULLET_OFFSET_Y = 16;
const int BACHI_BULLET_SIZE   = 16;
const int BACHI_BULLET = 8;

const Float BACHI_BULLET_VELOCITY = { -9.0f};
const unsigned int BACHI_SHOT_DELAY = 300;

}


Bachi::Bachi( unsigned int hp, unsigned int att, unsigned int sh,
              lx::Graphics::Sprite * image, int x, int y, int w, int h,
              float vx, float vy )
    : Enemy( hp, att, sh, image, x, y, w, h, vx, vy )
{
    ShotStrategy * st = new ShotStrategy( this );
    st->setShotDelay( BACHI_SHOT_DELAY );

    mvs->addMoveStrat( new PseudoSinusMoveStrategy( this ) );
    mvs->addShotStrat( st );
    addStrategy( mvs );
}


void Bachi::fire() noexcept
{
    PlayerVisitor visitor;
    Player::accept( visitor );
    const Float& LAST_PX = visitor.getLastX();

    if ( LAST_PX < phybox.p.x - fbox<int>( phybox.w * 2 ) )
    {
        std::array<lx::Physics::Vector2D, BulletPattern::WAVE_SZ> bullet_speed;

        lx::Graphics::ImgRect shot_area{ static_cast<int>( phybox.p.x ) + BACHI_BULLET_OFFSET_X,
                                         static_cast<int>( phybox.p.y ) + BACHI_BULLET_OFFSET_Y,
                                         BACHI_BULLET_SIZE, BACHI_BULLET_SIZE
                                       };

        BulletPattern::waveOnPlayer( phybox.p.x, phybox.p.y + fbox<int>( phybox.h / 2 ),
                                     apply_dgb( BACHI_BULLET_VELOCITY ), bullet_speed );

        EntityHandler& hdl = EntityHandler::getInstance();
        const ResourceManager * rc = ResourceManager::getInstance();
        lx::Graphics::Sprite * spr = rc->getResource( RC_MISSILE, BACHI_BULLET );

        for ( lx::Physics::Vector2D& v : bullet_speed )
        {
            hdl.pushEnemyMissile( *( new Bullet( attack_val, spr, shot_area, v ) ) );
        }
    }
}

void Bachi::reaction( Missile * target ) noexcept
{
    Enemy::reaction( target );

    if ( was_killed )
        EntityHandler::getInstance().pushItem( *( new Item( imgbox.p.x, imgbox.p.y ) ) );
}
