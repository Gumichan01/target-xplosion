
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
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
*   website : https://gumichan01.github.io/
*   mail : luxon.jean.pierre@gmail.com
*/

#include "Bullet.hpp"

#include "../game/Game.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Image.hpp>
#include <LunatiX/LX_Timer.hpp>

const uint32_t LIMIT = 1000;
const uint32_t DELAY_MBTIME = 500;


Bullet::Bullet(unsigned int pow, LX_Graphics::LX_Sprite *image,
               LX_Mixer::LX_Sound *audio,
               LX_AABB& rect, LX_Physics::LX_Vector2D& sp)
    : Missile(pow, 2, image, audio, rect, sp),bullet_time(LX_Timer::getTicks()) {}


Bullet::~Bullet() {}


void Bullet::draw()
{
    double angle;
    BulletPattern::calculateAngle(speed,angle);
    graphic->draw(&position,angle);
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
                       LX_Mixer::LX_Sound *audio,
                       LX_AABB& rect, LX_Physics::LX_Vector2D& sp,int explosion_vel)
    : Bullet(pow,image,audio,rect,sp), mbtime(LX_Timer::getTicks()),
      circle_vel(explosion_vel) {}


void MegaBullet::move()
{
    if((LX_Timer::getTicks() - mbtime) > DELAY_MBTIME)
    {
        explosion();
        die();
    }
    else
        Missile::move();
}


void MegaBullet::explosion()
{
    LX_Physics::LX_Vector2D v[CIRCLE_BULLETS];
    LX_AABB rect = {position.x,position.y,24,20};

    BulletPattern::circlePattern(position.x + (position.w/2),
                                 position.y + (position.h/2),
                                 circle_vel,v);

    Game *g = Game::getInstance();
    ResourceManager *rc = ResourceManager::getInstance();


    for(int i = 0; i < CIRCLE_BULLETS; i++)
    {
        g->acceptEnemyMissile(new Bullet(power,rc->getResource(RC_MISSILE,4)
                                         ,nullptr,rect,v[i]));
    }
}

