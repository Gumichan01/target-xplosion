
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
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file Framerate.cpp
*	@brief The Framerate regulator
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Framerate.hpp"

#include <SDL2/SDL_timer.h>
#include <LunatiX/LX_Log.hpp>

namespace Framerate
{
const unsigned int SCREEN_FPS = 60;
const Uint32 FRAME_DELAY = (1000 / SCREEN_FPS) + 1;
float delta = FRAME_DELAY;

void frame()
{
    static long t = 0;
    delta = static_cast<float>(SDL_GetTicks() - t);
    t = SDL_GetTicks();
}

float getDeltaTime()
{
    return delta;
}


void cycle()
{
    static long previous_time = 0;
    static int n = 0;
    n += 1;

    if(static_cast<long>(SDL_GetTicks() - previous_time) >= 1000L)
    {
        int fps = n;
        n = 0;
        previous_time = SDL_GetTicks();

        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"FPS: %d\n",fps);
    }
}

};
