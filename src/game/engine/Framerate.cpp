
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "Framerate.hpp"

#include <Lunatix/Log.hpp>
#include <Lunatix/Time.hpp>


namespace Framerate
{
constexpr float SCREEN_FPS = 60.0f;
constexpr float FRAME_DELAY = ( 1000.0f / SCREEN_FPS );
constexpr unsigned int FDELAY = static_cast<unsigned int>( FRAME_DELAY ) + 1U;
const unsigned int SECOND = 1000U;   // ms


void cycle()
{
    if ( lx::Log::isDebugMode() )
    {
        static unsigned int previous_time = 0;
        static int n = 0;
        n += 1;

        if ( ( lx::Time::getTicks() - previous_time ) >= SECOND )
        {
            int fps = n;
            n = 0;
            previous_time = lx::Time::getTicks();

            lx::Log::logDebug( lx::Log::LogType::APPLICATION, "FPS: %d\n", fps );
        }
    }
}

void regulate()
{
    static unsigned int prev_time = lx::Time::getTicks();
    unsigned int ticks = ( lx::Time::getTicks() - prev_time );

    if ( ticks < FDELAY )
        lx::Time::delay( static_cast<unsigned int>( FRAME_DELAY ) - ticks );

    prev_time = lx::Time::getTicks();
}

}
