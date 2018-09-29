
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

#ifndef AUDIOHANDLER_HPP_INCLUDED
#define AUDIOHANDLER_HPP_INCLUDED

namespace lx
{

namespace Mixer
{
class Music;
class Chunk;
}

namespace Graphics
{
struct ImgCoord;
}

}


/*
*   AudioHandler can handle 256 channels
*
*   - Channel no. 0         : alarm
*   - Channels from 1 to 59 : channel reserved for the player
*   - Channels from 60 to 62: channel reserved for the voice
*   - Channel 63: alert
*   - Channels from 64 to 255: anything else
*/
namespace AudioHandler
{

class AudioHDL final
{
    lx::Mixer::Music * main_music;
    lx::Mixer::Music * boss_music;
    lx::Mixer::Chunk * alarm;

    // Shots
    lx::Mixer::Chunk * basic_shot;
    lx::Mixer::Chunk * rocket_shot;
    lx::Mixer::Chunk * laser_shot;

    // Explosion
    lx::Mixer::Chunk * pexplosion;
    lx::Mixer::Chunk * sexplosion;
    lx::Mixer::Chunk * mexplosion;
    lx::Mixer::Chunk * bexplosion;
    lx::Mixer::Chunk * explosion;

    //Voice
    lx::Mixer::Chunk * txv_boss;
    lx::Mixer::Chunk * txv_rocket;
    lx::Mixer::Chunk * txv_shield;
    lx::Mixer::Chunk * txv_pulse;
    lx::Mixer::Chunk * txv_wave;
    lx::Mixer::Chunk * txv_mother;

    // Hit feedback
    lx::Mixer::Chunk * hits01;
    lx::Mixer::Chunk * hits02;
    lx::Mixer::Chunk * hits03;
    lx::Mixer::Chunk * hits04;
    lx::Mixer::Chunk * alert_normal;
    lx::Mixer::Chunk * alert_critical;
    lx::Mixer::Chunk * ehits;
    lx::Mixer::Chunk * bulletx;

    // Menu
    lx::Mixer::Chunk * menu_select;
    lx::Mixer::Chunk * menu_selected;
    lx::Mixer::Chunk * menu_back;

    explicit AudioHDL();
    AudioHDL( const AudioHDL& );
    AudioHDL& operator =( const AudioHDL& );

public:

    static AudioHDL * init();
    static AudioHDL * getInstance();
    static void destroy();
    static void haltAudio() noexcept;

    void setLevel( const unsigned int lvid );
    void playMainMusic() noexcept;
    void stopMainMusic() noexcept;
    void playBossMusic() noexcept;
    void stopBossMusic() noexcept;
    void playAlarm() noexcept;

    void playShot( const lx::Graphics::ImgCoord& pos ) noexcept;
    void playRocketShot( const lx::Graphics::ImgCoord& pos ) noexcept;
    void playLaserShot() noexcept;

    void playPlayerExplosion() noexcept;
    void playExplosion( const lx::Graphics::ImgCoord& pos ) noexcept;
    void playSmallExplosion() noexcept;
    void playMediumExplosion() noexcept;
    void playBigExplosion() noexcept;

    void playVoiceBoss() noexcept;
    void playVoiceRocket() noexcept;
    void playVoiceShield() noexcept;
    void playVoicePulse() noexcept;
    void playVoiceWave() noexcept;
    void playVoiceMother() noexcept;

    void playHit( short hit_level ) noexcept;
    void playAlert( bool critical = false ) noexcept;
    void stopAlert() noexcept;
    void playEnemyHit() noexcept;

    void playMenuSelect() noexcept;
    void playMenuSelected() noexcept;
    void playMenuBack() noexcept;

    ~AudioHDL();
};

}

#endif // AUDIOHANDLER_HPP_INCLUDED
