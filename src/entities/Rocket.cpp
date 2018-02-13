
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


#include "Rocket.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "../asset/TX_Asset.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../resources/WinID.hpp"

#include <LunatiX/LX_ParticleSystem.hpp>
#include <LunatiX/LX_Particle.hpp>
#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Graphics.hpp>

const unsigned int NB_PARTICLES = 20;

using namespace LX_ParticleEngine;
using namespace LX_Random;

namespace
{
const int ROCKET_MULTIPLIER = 12;
const int OFFSET_PARTICLE = 8;
const int PARTICLE_WIDTH = 16;
const int PARTICLE_HEIGHT = 8;
const int PARTICLE_ID = 1;
}

Rocket::Rocket(unsigned int pow, LX_Graphics::LX_Sprite *image,
               LX_AABB& rect, LX_Physics::LX_Vector2D& sp)
    : Missile(pow, ROCKET_MULTIPLIER, image, rect, sp),
      sys(new LX_ParticleSystem(NB_PARTICLES)), particle(nullptr), vp(),
      velocity(0)
{
    const TX_Asset *asset = TX_Asset::getInstance();
    LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(WinID::getWinID());
    particle = new LX_Graphics::LX_Sprite(asset->getExplosionSpriteFile(PARTICLE_ID),*w);
    velocity = LX_Physics::vector_norm(speed);
    vp = speed;
}


void Rocket::draw()
{
    sys->updateParticles();

    const LX_Physics::LX_Vector2D v(0.0f, 0.0f);
    unsigned int n = sys->nbEmptyParticles();

    for(unsigned int i = 0; i < n; i++)
    {
        LX_ParticleEngine::LX_Particle *p;
        LX_AABB box = {position.x - OFFSET_PARTICLE + (crand()%25),
                       position.y - OFFSET_PARTICLE + (crand()%25),
                       PARTICLE_WIDTH, PARTICLE_HEIGHT
                      };

        p = new LX_Particle(*particle, box, v);

        if(!sys->addParticle(p))
            delete p;
    }
    sys->displayParticles();
}


void Rocket::visit_(Character * c)
{
    const int cx = c->getX() + (c->getWidth() / 2);
    const int cy = c->getY() + (c->getHeight() / 2);
    LX_Physics::LX_Vector2D u;
    BulletPattern::shotOnTarget(position.x, position.y, cx, cy, -velocity, u);

    if(u != speed)
    {
        LX_Physics::LX_Vector2D v = vp + speed + u;
        float d = LX_Physics::vector_norm(v);
        vp += speed;
        vp *= velocity / LX_Physics::vector_norm(vp);
        speed = v * velocity / d;
    }
}

void Rocket::visit(Character * c)
{
    visit_(c);
}

Rocket::~Rocket()
{
    delete particle;
    delete sys;
}

/// Player's rocket

PlayerRocket::PlayerRocket(unsigned int pow, LX_Graphics::LX_Sprite *image,
                           LX_AABB& rect, LX_Physics::LX_Vector2D& sp)
    : Rocket(pow, image, rect, sp) {}


void PlayerRocket::draw()
{
    double angle;
    Rocket::draw();
    BulletPattern::calculateAngle(speed, angle);

    if(speed.vx < 0.0f)
        graphic->draw(&position, angle + BulletPattern::PI);
    else
        graphic->draw(&position, angle);
}

void PlayerRocket::move()
{
    EntityHandler::getInstance().targetEnemy(*this);
    Missile::move();
}


/// Enemy rocket

EnemyRocket::EnemyRocket(unsigned int pow, LX_Graphics::LX_Sprite *image,
                         LX_AABB& rect, LX_Physics::LX_Vector2D& sp)
    : Rocket(pow, image, rect, sp) {}


void EnemyRocket::draw()
{
    double angle;
    Rocket::draw();
    BulletPattern::calculateAngle(speed, angle);

    if(speed.vx < 0.0f && speed.vy != 0)
        graphic->draw(&position, angle + BulletPattern::PI_F);
    else
        graphic->draw(&position, angle);
}

void EnemyRocket::move()
{
    /// @todo target player fix
    //EntityHandler::getInstance().targetPlayer(*this);
    Missile::move();
}

