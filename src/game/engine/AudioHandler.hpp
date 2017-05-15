
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

/// @todo (#6#) v0.4.9: Set position of the source according the entity's position
/// It needs an update of LunatiX

/*
*   AudioHandler can handle 64 channels
*
*   - Channel no. 0: alarm
*   - Channels from 1 to 16: channel reserved for the player
*   - Channels from 17 to 20: channel reserved for the voice
*   - Channels from 21 to 64: channel reserved for the enemies
*/
namespace AudioHandler
{
const int AUDIOHANDLER_PLAYER_TAG = 2;

class AudioHDL
{
    LX_Mixer::LX_Music * main_music;
    LX_Mixer::LX_Music * boss_music;
    LX_Mixer::LX_Chunk *alarm;
    LX_Mixer::LX_Chunk *explosion;  /// @todo (#2#) v0.4.8: Handle the explosion sound
    LX_Mixer::LX_Chunk *txv_boss;
    LX_Mixer::LX_Chunk *txv_rocket;
    LX_Mixer::LX_Chunk *txv_shield;
    LX_Mixer::LX_Chunk *txv_pulse;
    LX_Mixer::LX_Chunk *txv_wave;
    LX_Mixer::LX_Chunk *txv_mother;

    explicit AudioHDL(const unsigned int lvid);
    AudioHDL(const AudioHDL&);
    AudioHDL& operator =(const AudioHDL&);

public:


    static AudioHDL * init(const unsigned int lvid);
    static AudioHDL * getInstance();
    static void destroy();

    void playMainMusic();
    void stopMainMusic();
    void playBossMusic();
    void stopBossMusic();
    void playAlarm();
    void playVoiceBoss();
    void playVoiceRocket();
    void playVoiceShield();
    void playVoicePulse();
    void playVoiceWave();
    void playVoiceMother();

    ~AudioHDL();
};

};

#endif // AUDIOHANDLER_HPP_INCLUDED
