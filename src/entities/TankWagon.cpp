
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


#include "TankWagon.hpp"
#include "Missile.hpp"
#include "Player.hpp"
#include "Bomb.hpp"
#include "../pattern/Strategy.hpp"
#include "../game/engine/AudioHandler.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../resources/ResourceManager.hpp"

#include <Lunatix/Texture.hpp>
#include <Lunatix/Hitbox.hpp>
#include <Lunatix/Physics.hpp>

namespace
{
const unsigned int TANK_SHOT_DELAY = 1000;
const int TANK_BOMB_DIM = 64;
const int TANK_BOMB_YOFF = 76;
const int TANK_BOMB_ID = 10;

using lx::Physics::FloatPosition;
const std::vector<lx::Physics::FloatPosition> hpoints
{
    lx::Physics::FloatPosition{133.0f, 6.0f}, lx::Physics::FloatPosition{375.0f, 6.0f},
    lx::Physics::FloatPosition{479.0f, 89.0f}, lx::Physics::FloatPosition{475.0f, 224.0f},
    lx::Physics::FloatPosition{32.0f, 224.0f}, lx::Physics::FloatPosition{100.0f, 182.0f},
    lx::Physics::FloatPosition{20.0f, 132.0f}, lx::Physics::FloatPosition{20.0f, 58.0f},
};
}

using namespace FloatBox;

TankWagon::TankWagon( unsigned int hp, unsigned int att, unsigned int sh,
                      lx::Graphics::Sprite * image, int x, int y, int w, int h,
                      float vx, float vy )
    : BigEnemy( hp, att, sh, image, x, y, w, h, vx, vy ),
      shape( hpoints, lx::Physics::FloatPosition{fbox<int>( x ), fbox<int>( y )} )
{
    ShotStrategy * shot = new ShotStrategy( this );
    shot->setShotDelay( TANK_SHOT_DELAY );
    mvs->addShotStrat( shot );
    mvs->addMoveStrat( new MoveStrategy( this ) );
    addStrategy( mvs );
}


void TankWagon::draw() noexcept
{
    if ( dying )
    {
        std::vector<lx::Graphics::ImgRect> boxes =
        {
            {48, 64, 64, 64}, {128, 20, 64, 64}, {96, 128, 64, 64},
            {128, 160, 64, 64}, {260, 182, 64, 64}, {220, 16, 64, 64},
            {182, 102, 64, 64}, {348, 56, 64, 64}, {452, 64, 64, 64}
        };

        BigEnemy::drawInDieMode( boxes );
    }
    else
        BigEnemy::draw();
}

void TankWagon::move() noexcept
{
    movePoly( shape.getPoly(), speed );
    Enemy::move();
}

void TankWagon::collision( Missile * mi ) noexcept
{
    BigEnemy::collision_( mi, shape );
}

void TankWagon::collision( Player * play ) noexcept
{
    BigEnemy::collision_( play, shape );
}

void TankWagon::fire() noexcept
{
    lx::Graphics::ImgRect bpos
    {
        imgbox.p.x - TANK_BOMB_DIM,
        imgbox.p.y + TANK_BOMB_YOFF,
        TANK_BOMB_DIM, TANK_BOMB_DIM
    };

    lx::Physics::Vector2D SPEED = { -5.0f, 0.f };
    const ResourceManager * const rc = ResourceManager::getInstance();

    lx::Graphics::Sprite * spr = rc->getResource( RC_MISSILE, TANK_BOMB_ID );
    EntityHandler& hdl = EntityHandler::getInstance();
    hdl.pushEnemyMissile( *( new EnemyBomb( attack_val, spr, bpos, SPEED ) ) );
}
