
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

#include "Bullet.hpp"

#include "Player.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Timer.hpp>

namespace
{
const int BULLET_ID = 4;
const int BULLET_MULTIPLIER = 4;

// Trail Bullet
const uint32_t DELAY_TRTIME = 100;
const uint32_t Y_MULT = 16;

// Spin Bullet
const int SPIN_BULLET_ID = 6;

// Mega Bullet
const uint32_t DELAY_MBTIME = 500;
const int BULLETS_DIM = 24;

// Gigabullets
const int NBX = 2;
const int NBY = 2;
int GX_OFFSET[NBX] = {-100,100};
int GY_OFFSET[NBY] = {-100,100};
}

using namespace LX_Physics;

Bullet::Bullet(unsigned int pow, LX_Graphics::LX_Sprite *image,
               LX_AABB& rect, LX_Vector2D& sp)
    : Missile(pow, BULLET_MULTIPLIER, image, rect, sp),
      bullet_time(LX_Timer::getTicks()) {}


void Bullet::draw()
{
    if(graphic != nullptr)
    {
        double angle = 0.0;
        if(position.w != position.h)
            BulletPattern::calculateAngle(speed, angle);

        graphic->draw(&position, angle);
    }
}

/* ------------------------------
    TrailBullet implementation
   ------------------------------ */

TrailBullet::TrailBullet(unsigned int pow, LX_Graphics::LX_Sprite *image,
                         LX_AABB& rect, LX_Vector2D& sp)
    : Bullet(pow, image, rect, sp) {}

void TrailBullet::move()
{
    if((LX_Timer::getTicks() - bullet_time) > DELAY_TRTIME)
    {
        LX_Vector2D up = speed, down = speed;

        normalize(up);
        normalize(down);
        up   *= vector_norm(speed) / 2;
        down *= vector_norm(speed) / 2;

        up.vy   -= vector_norm(speed) / Y_MULT;
        down.vy += vector_norm(speed) / Y_MULT;

        EntityHandler& hdl = EntityHandler::getInstance();
        hdl.pushEnemyMissile(*(new Bullet(power, graphic, position, up)));
        hdl.pushEnemyMissile(*(new Bullet(power, graphic, position, down)));

        bullet_time = LX_Timer::getTicks();
    }

    Missile::move();
}


/* ------------------------------
    LunaticBullet implementation
   ------------------------------ */

LunaticBullet::LunaticBullet(unsigned int pow, LX_Graphics::LX_Sprite *image,
                             LX_AABB& rect, LX_Physics::LX_Vector2D& sp)
    : Bullet(pow, image, rect, sp), colour_time(LX_Timer::getTicks()),
      CTIME_LIMIT(vector_norm(sp) * SPIN_BULLET_DELAY), is_lunatic(true) {}


void LunaticBullet::lunatic()
{
    if(is_lunatic)
    {
        float norm = vector_norm(speed);
        const ResourceManager *rc = ResourceManager::getInstance();

        BulletPattern::shotOnPlayer(position.x, position.y,
                                    -(static_cast<int>(norm)), speed);
        graphic = rc->getResource(RC_MISSILE, SPIN_BULLET_ID);
        is_lunatic = false;
    }
}


void LunaticBullet::move()
{
    if((LX_Timer::getTicks() -  colour_time) > CTIME_LIMIT)
    {
        lunatic();
        bullet_time = LX_Timer::getTicks();
    }

    Missile::move();
}


/* ------------------------------
    MegaMBullet implementation
   ------------------------------ */


MegaBullet::MegaBullet(unsigned int pow, LX_Graphics::LX_Sprite *image,
                       LX_AABB& rect, LX_Vector2D& sp, float explosion_vel)
    : Bullet(pow, image, rect, sp), circle_vel(explosion_vel) {}


void MegaBullet::move()
{
    if((LX_Timer::getTicks() - bullet_time) > DELAY_MBTIME)
    {
        if(position.y >= 0 && position.y <= Engine::getMaxYlim())
            explosion();

        die();
    }
    else
        Missile::move();
}


void MegaBullet::explosion()
{
    std::array<LX_Vector2D, CIRCLE_BULLETS> varray;
    LX_AABB rect = {position.x, position.y, BULLETS_DIM, BULLETS_DIM};

    BulletPattern::circlePattern(position.x + (position.w/2),
                                 position.y + (position.h/2),
                                 circle_vel, varray);

    EntityHandler& hdl = EntityHandler::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, BULLET_ID);

    for(LX_Vector2D& v : varray)
    {
        hdl.pushEnemyMissile(*(new Bullet(power, spr, rect, v)));
    }
}


/* ------------------------------
    GigaBullet implementation
   ------------------------------ */

GigaBullet::GigaBullet(unsigned int pow, LX_Graphics::LX_Sprite *image,
                       LX_AABB& rect, LX_Vector2D& sp,
                       int explosion_vel1, float explosion_vel2)
    : MegaBullet(pow, image, rect, sp, explosion_vel2), vel(explosion_vel1) {}


void GigaBullet::explosion()
{
    using namespace LX_Graphics;

    const ResourceManager *rc = ResourceManager::getInstance();

    EntityHandler& hdl = EntityHandler::getInstance();
    LX_Sprite *spr = rc->getResource(RC_MISSILE, BULLET_ID);
    LX_AABB rect{position.x, position.y, BULLETS_DIM, BULLETS_DIM};
    LX_Point p(position.x + position.w/2, position.y + position.h/2);
    LX_Vector2D v[4] = {LX_Vector2D(0.0f,0.0f)};
    int k = 0;

    for(int i = 0; i < NBX; i++)
    {
        for(int j = 0; j < NBY; j++)
        {
            k = i + j + (i == 0 ? 0 : 1);
            BulletPattern::shotOnTarget(p.x, p.y, p.x + GX_OFFSET[i],
                                        p.y + GY_OFFSET[j], vel, v[k]);
            hdl.pushEnemyMissile(*(new MegaBullet(power, spr, rect, v[k], circle_vel)));
        }
    }
}
