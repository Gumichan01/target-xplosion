

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Luxon Jean-Pierre (Gumichan01)
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file Rocket.hpp
*	@brief The Rocket implementation
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <LunatiX/LX_ParticleSystem.hpp>
#include <LunatiX/LX_Particle.hpp>
#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_FileBuffer.hpp>

#include "Rocket.hpp"

static const unsigned int NB_PARTICLES = 20;

using namespace LX_ParticleEngine;
using namespace LX_Random;
using namespace LX_FileIO;

static const int OFFSET_PARTICLE = 8;
static const int PARTICLE_WIDTH = 16;
static const int PARTICLE_HEIGHT = 8;

static LX_FileBuffer *particle_buffer;


Rocket::Rocket(unsigned int pow, SDL_Texture *image,
               LX_Mixer::LX_Chunk *audio,
               SDL_Rect& rect, LX_Physics::LX_Vector2D& sp)
    : Missile(pow, 3, image, audio, rect, sp),
    sys(new LX_ParticleSystem(NB_PARTICLES))
{
    initParticles();
}


void Rocket::createParticlesRessources()
{
    particle_buffer = new LX_FileBuffer("image/smoke.png");
}


void Rocket::destroyParticlesRessources()
{
    delete particle_buffer;
    particle_buffer = nullptr;
}


void Rocket::initParticles(void)
{
    LX_Particle *p = nullptr;

    for(unsigned int i = 0; i < NB_PARTICLES; i++)
    {
        p = new LX_Particle(position.x - OFFSET_PARTICLE + (crand()%25),
                            position.y - OFFSET_PARTICLE + (crand()%25),
                            PARTICLE_WIDTH,PARTICLE_HEIGHT);
        p->setTexture(particle_buffer);
        sys->addParticle(p);
    }
}


void Rocket::move()
{
    Missile::move();
}


void Rocket::displayAdditionnalData()
{
    sys->updateParticles();

    unsigned int n = sys->nbEmptyParticles();
    LX_ParticleEngine::LX_Particle *p = nullptr;

    for(unsigned int i = 0; i < n; i++)
    {
        p = new LX_Particle(position.x - OFFSET_PARTICLE + (crand()%25),
                            position.y - OFFSET_PARTICLE + (crand()%25),
                            PARTICLE_WIDTH,PARTICLE_HEIGHT);
        p->setTexture(particle_buffer);

        if(sys->addParticle(p) == false)
            delete p;
    }
    sys->displayParticles();
}


Rocket::~Rocket()
{
    delete sys;
}

