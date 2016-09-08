
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
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/


#include "Rocket.hpp"
#include "Enemy.hpp"
#include "../game/Game.hpp"
#include "../asset/TX_Asset.hpp"
#include "../pattern/BulletPattern.hpp"

#include <LunatiX/LX_ParticleSystem.hpp>
#include <LunatiX/LX_Particle.hpp>
#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Graphics.hpp>

const unsigned int NB_PARTICLES = 20;

using namespace LX_ParticleEngine;
using namespace LX_Random;

const int OFFSET_PARTICLE = 8;
const int PARTICLE_WIDTH = 16;
const int PARTICLE_HEIGHT = 8;
const int PARTICLE_ID = 1;


Rocket::Rocket(unsigned int pow, LX_Graphics::LX_Sprite *image,
               LX_Mixer::LX_Sound *audio,
               LX_AABB& rect, LX_Physics::LX_Vector2D& sp)
    : Missile(pow, 3, image, audio, rect, sp),
      sys(new LX_ParticleSystem(NB_PARTICLES)),_particle(nullptr),velocity(0)
{
    LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(0);
    TX_Asset *asset = TX_Asset::getInstance();
    _particle = new LX_Graphics::LX_Sprite(asset->getExplosionSpriteFile(PARTICLE_ID),*w);
    velocity = LX_Physics::vector_norm(speed);
}


void Rocket::move()
{
    Game::getInstance()->acceptMissile(this);
    Missile::move();
}


void Rocket::draw()
{
    sys->updateParticles();

    const LX_Physics::LX_Vector2D v(0.0f,0.0f);
    unsigned int n = sys->nbEmptyParticles();
    LX_ParticleEngine::LX_Particle *p = nullptr;

    for(unsigned int i = 0; i < n; i++)
    {
        LX_AABB box = {position.x - OFFSET_PARTICLE + (crand()%25),
                       position.y - OFFSET_PARTICLE + (crand()%25),
                       PARTICLE_WIDTH,PARTICLE_HEIGHT
                      };

        p = new LX_Particle(*_particle,box,v);

        if(sys->addParticle(p) == false)
            delete p;
    }
    sys->displayParticles();
    Entity::draw();
}


void Rocket::visit(Enemy * e)
{
    const int ex = e->getX() + (e->getWidth()/2);
    const int ey = e->getY() + (e->getHeight()/2);
    LX_Physics::LX_Vector2D u;
    BulletPattern::shotOnTarget(position.x,position.y,ex,ey,-velocity,u);

    if(u != speed)
    {
        LX_Physics::LX_Vector2D v = speed + u;
        float d = LX_Physics::vector_norm(v);
        speed = v * (1/d) * velocity;
    }
}


Rocket::~Rocket()
{
    delete _particle;
    delete sys;
}

