
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

// Audio engine
#include "AudioHandler.hpp"
#include "../../asset/TX_Asset.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Audio.hpp>
#include <LunatiX/LX_Log.hpp>

using namespace LX_Mixer;

namespace
{
const int AUDIOHANDLER_BOSS_M1_ID = 7;
const int AUDIOHANDLER_BOSS_M2_ID = 8;
const int AUDIOHANDLER_ALARM_ID = 4;
const int AUDIOHANDLER_PEXPLOSION_ID = 11;
const int AUDIOHANDLER_EXPLOSION_ID = 3;
const int AUDIOHANDLER_VOICE_BOSS_ID = 5;
const int AUDIOHANDLER_VOICE_ROCKET_ID = 6;
const int AUDIOHANDLER_VOICE_SHIELD_ID = 7;
const int AUDIOHANDLER_VOICE_PULSE_ID = 8;
const int AUDIOHANDLER_VOICE_WAVE_ID = 9;
const int AUDIOHANDLER_VOICE_MOTHER_ID = 10;

const uint32_t AUDIOHANDLER_ALARM_DELAY = 6000;

const int AUDIOHANDLER_G_CHANNELS = 64;
const int AUDIOHANDLER_N_CHANNELS = 8;
const int AUDIOHANDLER_RESERVE_CHANNELS = 18;

const int AUDIOHANDLER_ALARM_TAG = 1;
const int AUDIOHANDLER_ALARM_CHAN = 0;

const int AUDIOHANDLER_VOICE_TAG = 3;
const int AUDIOHANDLER_VOICE_FROM = 17;
const int AUDIOHANDLER_VOICE_TO = 20;

const int AUDIOHANDLER_PLAYER_FROM = 1;
const int AUDIOHANDLER_PLAYER_TO = AUDIOHANDLER_G_CHANNELS/2;

};

/// @todo (#2#) v0.4.9: explosion at the position of the entity
namespace AudioHandler
{

static AudioHDL *audio_instance = nullptr;

AudioHDL * AudioHDL::init(const unsigned int lvid)
{
    if(audio_instance == nullptr)
        audio_instance = new AudioHDL(lvid);

    return audio_instance;
}

AudioHDL * AudioHDL::getInstance()
{
    return audio_instance;
}

void AudioHDL::destroy()
{
    delete audio_instance;
    audio_instance = nullptr;
}


AudioHDL::AudioHDL(const unsigned int lvid)
    : main_music(nullptr), boss_music(nullptr), alarm(nullptr),
      pexplosion(nullptr), explosion(nullptr), txv_boss(nullptr),
      txv_rocket(nullptr), txv_shield(nullptr), txv_pulse(nullptr),
      txv_wave(nullptr), txv_mother(nullptr)
{
    const TX_Asset *a = TX_Asset::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();

    main_music = new LX_Music(a->getLevelMusic(lvid));
    alarm = rc->getSound(AUDIOHANDLER_ALARM_ID);
    pexplosion = rc->getSound(AUDIOHANDLER_PEXPLOSION_ID);
    explosion = rc->getSound(AUDIOHANDLER_EXPLOSION_ID);
    txv_boss = rc->getSound(AUDIOHANDLER_VOICE_BOSS_ID);
    txv_rocket = rc->getSound(AUDIOHANDLER_VOICE_ROCKET_ID);
    txv_shield = rc->getSound(AUDIOHANDLER_VOICE_SHIELD_ID);
    txv_pulse = rc->getSound(AUDIOHANDLER_VOICE_PULSE_ID);
    txv_wave = rc->getSound(AUDIOHANDLER_VOICE_WAVE_ID);
    txv_mother = rc->getSound(AUDIOHANDLER_VOICE_MOTHER_ID);
    LX_Mixer::allocateChannels(AUDIOHANDLER_G_CHANNELS);

    if(alarm == nullptr)
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION, "AudioHDL — Cannot load the alarm");

    if(lvid%2 == 1)
        boss_music = new LX_Music(a->getLevelMusic(AUDIOHANDLER_BOSS_M1_ID));
    else
        boss_music = new LX_Music(a->getLevelMusic(AUDIOHANDLER_BOSS_M2_ID));

    // Channel group tags
    LX_Mixer::groupChannel(AUDIOHANDLER_ALARM_CHAN, AUDIOHANDLER_ALARM_TAG);
    LX_Mixer::groupChannels(AUDIOHANDLER_VOICE_FROM, AUDIOHANDLER_VOICE_TO,
                            AUDIOHANDLER_VOICE_TAG);
    LX_Mixer::groupChannels(AUDIOHANDLER_PLAYER_FROM, AUDIOHANDLER_PLAYER_TO,
                            AUDIOHANDLER_PLAYER_TAG);

    // Reserve channels
    LX_Mixer::reserveChannels(AUDIOHANDLER_RESERVE_CHANNELS);
}

void AudioHDL::playMainMusic()
{
    if(main_music != nullptr)
        main_music->play();
}

void AudioHDL::stopMainMusic()
{
    if(main_music != nullptr)
        main_music->stop();
}

void AudioHDL::playBossMusic()
{
    if(boss_music != nullptr)
        boss_music->play(LX_Mixer::LX_MIXER_LOOP);
}

void AudioHDL::stopBossMusic()
{
    if(boss_music != nullptr)
        boss_music->stop();
}

void AudioHDL::playAlarm()
{
    if(alarm != nullptr)
        alarm->play(AUDIOHANDLER_ALARM_CHAN, 0, AUDIOHANDLER_ALARM_DELAY);
}

void AudioHDL::playPlayerExplosion()
{
    if(pexplosion != nullptr)
        pexplosion->play();
}

void AudioHDL::playExplosion()
{
    if(explosion != nullptr)
        explosion->play();
}

void AudioHDL::playVoiceBoss()
{
    if(txv_boss != nullptr)
        LX_Mixer::groupPlayChunk(*txv_boss, AUDIOHANDLER_VOICE_TAG, 0);
}

void AudioHDL::playVoiceRocket()
{
    if(txv_rocket != nullptr)
        LX_Mixer::groupPlayChunk(*txv_rocket, AUDIOHANDLER_VOICE_TAG, 0);
}

void AudioHDL::playVoiceShield()
{
    if(txv_shield != nullptr)
        LX_Mixer::groupPlayChunk(*txv_shield, AUDIOHANDLER_VOICE_TAG, 0);
}

void AudioHDL::playVoicePulse()
{
    if(txv_pulse != nullptr)
        LX_Mixer::groupPlayChunk(*txv_pulse, AUDIOHANDLER_VOICE_TAG, 0);
}

void AudioHDL::playVoiceWave()
{
    if(txv_wave != nullptr)
        LX_Mixer::groupPlayChunk(*txv_wave, AUDIOHANDLER_VOICE_TAG, 0);
}

void AudioHDL::playVoiceMother()
{
    if(txv_mother != nullptr)
        LX_Mixer::groupPlayChunk(*txv_mother, AUDIOHANDLER_VOICE_TAG, 0);
}

AudioHDL::~AudioHDL()
{
    delete main_music;
    delete boss_music;
    LX_Mixer::reserveChannels(0);
    LX_Mixer::allocateChannels(AUDIOHANDLER_N_CHANNELS);
}

}
