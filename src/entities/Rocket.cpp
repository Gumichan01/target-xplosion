
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
#include "../game/Game.hpp"
#include "../asset/TX_Asset.hpp"
#include "../pattern/BulletPattern.hpp"
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
const int ROCKET_MULTIPLIER = 8;
const int OFFSET_PARTICLE = 8;
const int PARTICLE_WIDTH = 16;
const int PARTICLE_HEIGHT = 8;
const int PARTICLE_ID = 1;
};

Rocket::Rocket(unsigned int pow, LX_Graphics::LX_Sprite *image,
               LX_Mixer::LX_Sound *audio,
               LX_AABB& rect, LX_Physics::LX_Vector2D& sp)
    : Missile(pow, ROCKET_MULTIPLIER, image, audio, rect, sp),
      sys(new LX_ParticleSystem(NB_PARTICLES)), particle(nullptr), velocity(0)
{
    LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(WinID::getWinID());
    const TX_Asset *asset = TX_Asset::getInstance();
    particle = new LX_Graphics::LX_Sprite(asset->getExplosionSpriteFile(PARTICLE_ID),*w);
    velocity = LX_Physics::vector_norm(speed);
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

        if(sys->addParticle(p) == false)
            delete p;
    }
    sys->displayParticles();
}


void Rocket::visit_(Character * c)
{
    const int cx = c->getX() + (c->getWidth()/2);
    const int cy = c->getY() + (c->getHeight()/2);
    LX_Physics::LX_Vector2D u;
    BulletPattern::shotOnTarget(position.x, position.y, cx, cy, -velocity, u);

    if(u != speed)
    {
        LX_Physics::LX_Vector2D v = speed + u;
        float d = LX_Physics::vector_norm(v);
        speed = v * (1/d) * velocity;
    }
}


Rocket::~Rocket()
{
    delete particle;
    delete sys;
}

/// Player's rocket

PlayerRocket::PlayerRocket(unsigned int pow, LX_Graphics::LX_Sprite *image,
                           LX_Mixer::LX_Sound *audio, LX_AABB& rect,
                           LX_Physics::LX_Vector2D& sp)
    : Rocket(pow, image, audio, rect, sp) {}


void PlayerRocket::draw()
{
    double angle;
    Rocket::draw();
    BulletPattern::calculateAngle(speed, angle);
    graphic->draw(&position, angle);
}

void PlayerRocket::move()
{
    Game::getInstance()->targetEnemy(this);
    Missile::move();
}

void PlayerRocket::visit(Enemy * e)
{
    if(position.x < e->getX())
        Rocket::visit_(e);
}

/// Enemy rocket

EnemyRocket::EnemyRocket(unsigned int pow, LX_Graphics::LX_Sprite *image,
                         LX_Mixer::LX_Sound *audio, LX_AABB& rect,
                         LX_Physics::LX_Vector2D& sp)
    : Rocket(pow, image, audio, rect, sp) {}


void EnemyRocket::draw()
{
    double angle;
    Rocket::draw();
    BulletPattern::calculateAngle(speed, angle);

    if(speed.vy == 0.0f)
        graphic->draw(&position, angle, LX_Graphics::LX_MIRROR_VERTICAL);
    else
        graphic->draw(&position, angle, LX_Graphics::LX_MIRROR_HORIZONTAL);
}

void EnemyRocket::move()
{
    Game *g = Game::getInstance();

    if(position.y > Game::getMinYlim() && position.y < g->getMaxYlim())
        g->targetPlayer(this);

    Missile::move();
}

void EnemyRocket::visitp(Player * p)
{
    if(position.x > (p->getX() + p->getWidth()))
        Rocket::visit_(p);
}
