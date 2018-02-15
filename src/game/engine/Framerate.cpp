
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

#include "Framerate.hpp"

#include <LunatiX/LX_Log.hpp>
#include <LunatiX/LX_Timer.hpp>


namespace Framerate
{
const float SCREEN_FPS = 60.0f;
const float FRAME_DELAY = (1000.0f / SCREEN_FPS) + 1.0f;
const unsigned int SECOND = 1000;


void cycle()
{
    static unsigned int previous_time = 0;
    static int n = 0;
    n += 1;

    if((LX_Timer::getTicks() - previous_time) >= SECOND)
    {
        int fps = n;
        n = 0;
        previous_time = LX_Timer::getTicks();

        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION, "FPS: %d\n", fps);
    }
}

void regulate()
{
    static unsigned int prev_time = LX_Timer::getTicks();
    unsigned int ticks = (LX_Timer::getTicks() - prev_time);

    if(ticks < FRAME_DELAY)
        LX_Timer::delay(static_cast<unsigned int>(FRAME_DELAY) - ticks);

    prev_time = LX_Timer::getTicks();
}

}
