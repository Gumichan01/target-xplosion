

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2014  Luxon Jean-Pierre
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

#include "Rocket.hpp"
#include "../engine/LX_ParticleSystem.hpp"
#include "../engine/LX_Particle.hpp"
#include "../engine/LX_Random.hpp"

using namespace LX_ParticleEngine;
using namespace LX_Random;

#define NB_PARTICLES 20


Rocket::Rocket(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,
               int x, int y, int w, int h,int dX, int dY)
    : Missile(pow, 2, image, audio, x, y, w, h, dX, dY)
{
    initParticles();
}


Rocket::Rocket(unsigned int pow, SDL_Texture *image, LX_Chunk *audio,
               SDL_Rect *rect,LX_Vector2D *sp)
    : Missile(pow, 2, image, audio, rect, sp)
{
    initParticles();
}


void Rocket::initParticles(void)
{
    sys = new LX_ParticleSystem(NB_PARTICLES);
    const unsigned int n = NB_PARTICLES;
    LX_Particle *p = NULL;

    for(unsigned int i = 0; i < n; i++)
    {
        p = new LX_Particle(position.x - 5 + (crand()%8),position.y - 5 + (crand()%8),5,5);
        p->setTexture("image/smoke.png",0);
        sys->addParticle(p);
    }
}


void Rocket::move()
{
    Missile::move();
}


void Rocket::displayAdditionnalData()
{
    unsigned int n;
    LX_Particle *p = NULL;

    sys->updateParticles();
    n = sys->nbEmptyParticles();

    for(unsigned int i = 0; i < n; i++)
    {
        p = new LX_Particle(position.x - 5 + (crand()%25),position.y - 5 + (crand()%25),5,5);
        p->setTexture("image/smoke.png",0);

        if(sys->addParticle(p) == false)
        {
            delete p;
        }
    }

    sys->displayParticles();
}


Rocket::~Rocket()
{
    delete sys;
}




