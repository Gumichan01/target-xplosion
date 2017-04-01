
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

#include "../game/engine/Engine.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Timer.hpp>

namespace
{
const int BULLET_ID = 4;
const uint32_t LIMIT = 1000;
const uint32_t DELAY_MBTIME = 500;
const int BULLETS_DIM = 24;

// Gigabullets
const int NBX = 2;
const int NBY = 2;
int GX_OFFSET[NBX] = {-100,100};
int GY_OFFSET[NBY] = {-100,100};
};


Bullet::Bullet(unsigned int pow, LX_Graphics::LX_Sprite *image,
               LX_Mixer::LX_Chunk *audio,
               LX_AABB& rect, LX_Physics::LX_Vector2D& sp)
    : Missile(pow, 2, image, audio, rect, sp), bullet_time(LX_Timer::getTicks()) {}


Bullet::~Bullet() {}


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


void Bullet::move()
{
    if(LX_Timer::getTicks() - bullet_time > LIMIT)
        bullet_time = LX_Timer::getTicks();

    Missile::move();
}


/* ------------------------------
    MegaMBullet implementation
   ------------------------------ */


MegaBullet::MegaBullet(unsigned int pow, LX_Graphics::LX_Sprite *image,
                       LX_Mixer::LX_Chunk *audio, LX_AABB& rect,
                       LX_Physics::LX_Vector2D& sp, int explosion_vel)
    : Bullet(pow, image, audio, rect, sp), mbtime(LX_Timer::getTicks()),
      circle_vel(explosion_vel) {}


void MegaBullet::move()
{
    if((LX_Timer::getTicks() - mbtime) > DELAY_MBTIME)
    {
        if(position.y >= 0 && position.y <= Engine::getInstance()->getMaxYlim())
            explosion();

        die();
    }
    else
        Missile::move();
}


void MegaBullet::explosion()
{
    LX_Physics::LX_Vector2D v[CIRCLE_BULLETS];
    LX_AABB rect = {position.x, position.y, BULLETS_DIM, BULLETS_DIM};

    BulletPattern::circlePattern(position.x + (position.w/2),
                                 position.y + (position.h/2),
                                 circle_vel, v);

    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();

    for(int i = 0; i < CIRCLE_BULLETS; i++)
    {
        g->acceptEnemyMissile(new Bullet(power, rc->getResource(RC_MISSILE, BULLET_ID)
                                         , nullptr, rect, v[i]));
    }
}


/* ------------------------------
    GigaMBullet implementation
   ------------------------------ */

GigaBullet::GigaBullet(unsigned int pow, LX_Graphics::LX_Sprite *image,
                       LX_Mixer::LX_Chunk *audio, LX_AABB& rect,
                       LX_Physics::LX_Vector2D& sp, int explosion_vel1,
                       int explosion_vel2)
    : MegaBullet(pow, image, audio, rect, sp, explosion_vel2), vel(explosion_vel1) {}

void GigaBullet::explosion()
{
    using namespace LX_Physics;

    Engine *g = Engine::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_AABB rect = {position.x, position.y, BULLETS_DIM, BULLETS_DIM};
    LX_Vector2D v[4] = {LX_Vector2D(0.0f,0.0f)};
    LX_Point p(position.x + position.w/2, position.y + position.h/2);
    int k = 0;

    for(int i = 0; i < NBX; i++)
    {
        for(int j = 0; j < NBY; j++)
        {
            k = i + j + (i == 0 ? 0 : 1);
            BulletPattern::shotOnTarget(p.x, p.y, p.x + GX_OFFSET[i],
                                        p.y + GY_OFFSET[j], vel, v[k]);
            g->acceptEnemyMissile(new MegaBullet(power,
                                                 rc->getResource(RC_MISSILE, BULLET_ID),
                                                 nullptr, rect, v[k], circle_vel));
        }
    }
}
