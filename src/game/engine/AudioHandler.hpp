
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

#ifndef AUDIOHANDLER_HPP_INCLUDED
#define AUDIOHANDLER_HPP_INCLUDED

namespace LX_Mixer
{
class LX_Music;
class LX_Chunk;
};

namespace AudioHandler
{
class AudioHandler
{
    LX_Mixer::LX_Music * main_music;
    LX_Mixer::LX_Music * boss_music;
    LX_Mixer::LX_Chunk *alarm;

    AudioHandler(const AudioHandler&);
    AudioHandler& operator =(const AudioHandler&);

public:

    AudioHandler();

    void playMainMusic();
    void StopMainMusic();
    void playBossMusic();
    void stopBossMusic();
    void playAlarm();

    ~AudioHandler();
};

};

#endif // AUDIOHANDLER_HPP_INCLUDED
