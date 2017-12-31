
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
}


/*
*   AudioHandler can handle 64 channels
*
*   - Channel no. 0         : alarm
*   - Channels from 1 to 16 : channel reserved for the player
*   - Channels from 17 to 20: channel reserved for the voice
*   - Channel 21: alert
*   - Channels from 22 to 147: anything else
*/
namespace AudioHandler
{

class AudioHDL
{
    LX_Mixer::LX_Music *main_music;
    LX_Mixer::LX_Music *boss_music;
    LX_Mixer::LX_Chunk *alarm;

    // Shots
    LX_Mixer::LX_Chunk *basic_shot;
    LX_Mixer::LX_Chunk *rocket_shot;
    LX_Mixer::LX_Chunk *laser_shot;

    // Explosion
    LX_Mixer::LX_Chunk *pexplosion;
    LX_Mixer::LX_Chunk *sexplosion;
    LX_Mixer::LX_Chunk *mexplosion;
    LX_Mixer::LX_Chunk *bexplosion;
    LX_Mixer::LX_Chunk *explosion;

    //Voice
    LX_Mixer::LX_Chunk *txv_boss;
    LX_Mixer::LX_Chunk *txv_rocket;
    LX_Mixer::LX_Chunk *txv_shield;
    LX_Mixer::LX_Chunk *txv_pulse;
    LX_Mixer::LX_Chunk *txv_wave;
    LX_Mixer::LX_Chunk *txv_mother;

    // Hit feedback
    LX_Mixer::LX_Chunk *hits01;
    LX_Mixer::LX_Chunk *hits02;
    LX_Mixer::LX_Chunk *hits03;
    LX_Mixer::LX_Chunk *hits04;
    LX_Mixer::LX_Chunk *alert_normal;
    LX_Mixer::LX_Chunk *alert_critical;
    LX_Mixer::LX_Chunk *ehits;
    LX_Mixer::LX_Chunk *bulletx;

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

    void playShot();
    void playRocketShot();
    void playLaserShot();

    void playPlayerExplosion();
    void playExplosion();
    void playSmallExplosion();
    void playMediumExplosion();
    void playBigExplosion();

    void playVoiceBoss();
    void playVoiceRocket();
    void playVoiceShield();
    void playVoicePulse();
    void playVoiceWave();
    void playVoiceMother();

    void playHit(short hit_level);
    void playAlert(bool critical=false);
    void stopAlert();
    void playEnemyHit();

    ~AudioHDL();
};

}

#endif // AUDIOHANDLER_HPP_INCLUDED
