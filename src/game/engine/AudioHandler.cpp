
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

// Audio engine
#include "AudioHandler.hpp"
#include "../../asset/TX_Asset.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Audio.hpp>
#include <LunatiX/LX_ImgRect.hpp>


using namespace LX_Mixer;

namespace
{
const int AUDIOHANDLER_BOSS_M1_ID = 7;
const int AUDIOHANDLER_BOSS_M2_ID = 8;
const int AUDIOHANDLER_ALARM_ID = 4;

const int AUDIOHANDLER_SHOT_ID = 0;
const int AUDIOHANDLER_ROCKET_ID = 1;
const int AUDIOHANDLER_LASER_ID = 2;

const int AUDIOHANDLER_PEXPLOSION_ID = 11;
const int AUDIOHANDLER_SEXPLOSION_ID = 12;
const int AUDIOHANDLER_MEXPLOSION_ID = 13;
const int AUDIOHANDLER_BEXPLOSION_ID = 14;
const int AUDIOHANDLER_HITS01_ID = 15;
const int AUDIOHANDLER_HITS02_ID = 16;
const int AUDIOHANDLER_HITS03_ID = 17;
const int AUDIOHANDLER_HITS04_ID = 18;
const int AUDIOHANDLER_ALERT_NORMAL_ID = 19;
const int AUDIOHANDLER_ALERT_CRITICAL_ID = 20;
const int AUDIOHANDLER_EHITS_ID = 21;

const int AUDIOHANDLER_EXPLOSION_ID = 3;
const int AUDIOHANDLER_VOICE_BOSS_ID = 5;
const int AUDIOHANDLER_VOICE_ROCKET_ID = 6;
const int AUDIOHANDLER_VOICE_SHIELD_ID = 7;
const int AUDIOHANDLER_VOICE_PULSE_ID = 8;
const int AUDIOHANDLER_VOICE_WAVE_ID = 9;
const int AUDIOHANDLER_VOICE_MOTHER_ID = 10;

const unsigned int AUDIOHANDLER_ALARM_DELAY = 6000;

const int AUDIOHANDLER_G_CHANNELS = 256;
const int AUDIOHANDLER_N_CHANNELS = 8;
const int AUDIOHANDLER_RESERVE_CHANNELS = 64;

const int AUDIOHANDLER_ALARM_TAG  = 1;
const int AUDIOHANDLER_ALARM_CHAN = 0;

const int AUDIOHANDLER_PLAYER_TAG  = 2;
const int AUDIOHANDLER_PLAYER_FROM = 1;
const int AUDIOHANDLER_PLAYER_TO   = 59;

const int AUDIOHANDLER_VOICE_TAG  = 3;
const int AUDIOHANDLER_VOICE_FROM = 60;
const int AUDIOHANDLER_VOICE_TO   = 62;

const int AUDIOHANDLER_ALERT_TAG   = 4;
const int AUDIOHANDLER_ALERT_CHAN  = 63;

const int MAX_X = 1280;
const int MAX_PAN = 255;

}


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
      basic_shot(nullptr), rocket_shot(nullptr), laser_shot(nullptr),
      pexplosion(nullptr), sexplosion(nullptr), mexplosion(nullptr),
      bexplosion(nullptr), explosion(nullptr), txv_boss(nullptr),
      txv_rocket(nullptr), txv_shield(nullptr), txv_pulse(nullptr),
      txv_wave(nullptr), txv_mother(nullptr), hits01(nullptr),
      hits02(nullptr), hits03(nullptr), hits04(nullptr),
      alert_normal(nullptr), alert_critical(nullptr), ehits(nullptr)
{
    const TX_Asset * const ASSET = TX_Asset::getInstance();
    const ResourceManager * const RC = ResourceManager::getInstance();

    main_music     = new LX_Music(ASSET->getLevelMusic(lvid));
    alarm          = RC->getSound(AUDIOHANDLER_ALARM_ID);
    basic_shot     = RC->getSound(AUDIOHANDLER_SHOT_ID);
    rocket_shot    = RC->getSound(AUDIOHANDLER_ROCKET_ID);
    laser_shot     = RC->getSound(AUDIOHANDLER_LASER_ID);
    pexplosion     = RC->getSound(AUDIOHANDLER_PEXPLOSION_ID);
    sexplosion     = RC->getSound(AUDIOHANDLER_SEXPLOSION_ID);
    mexplosion     = RC->getSound(AUDIOHANDLER_MEXPLOSION_ID);
    bexplosion     = RC->getSound(AUDIOHANDLER_BEXPLOSION_ID);
    explosion      = RC->getSound(AUDIOHANDLER_EXPLOSION_ID);
    txv_boss       = RC->getSound(AUDIOHANDLER_VOICE_BOSS_ID);
    txv_rocket     = RC->getSound(AUDIOHANDLER_VOICE_ROCKET_ID);
    txv_shield     = RC->getSound(AUDIOHANDLER_VOICE_SHIELD_ID);
    txv_pulse      = RC->getSound(AUDIOHANDLER_VOICE_PULSE_ID);
    txv_wave       = RC->getSound(AUDIOHANDLER_VOICE_WAVE_ID);
    txv_mother     = RC->getSound(AUDIOHANDLER_VOICE_MOTHER_ID);
    hits01         = RC->getSound(AUDIOHANDLER_HITS01_ID);
    hits02         = RC->getSound(AUDIOHANDLER_HITS02_ID);
    hits03         = RC->getSound(AUDIOHANDLER_HITS03_ID);
    hits04         = RC->getSound(AUDIOHANDLER_HITS04_ID);
    alert_normal   = RC->getSound(AUDIOHANDLER_ALERT_NORMAL_ID);
    alert_critical = RC->getSound(AUDIOHANDLER_ALERT_CRITICAL_ID);
    ehits          = RC->getSound(AUDIOHANDLER_EHITS_ID);
    LX_Mixer::allocateChannels(AUDIOHANDLER_G_CHANNELS);

    if(alarm == nullptr)
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION, "AudioHDL — Cannot load the alarm");

    if(lvid%2 == 1)
        boss_music = new LX_Music(ASSET->getLevelMusic(AUDIOHANDLER_BOSS_M1_ID));
    else
        boss_music = new LX_Music(ASSET->getLevelMusic(AUDIOHANDLER_BOSS_M2_ID));

    // Channel group tags
    LX_Mixer::groupChannel(AUDIOHANDLER_ALARM_CHAN, AUDIOHANDLER_ALARM_TAG);
    LX_Mixer::groupChannel(AUDIOHANDLER_ALERT_CHAN, AUDIOHANDLER_ALERT_TAG);

    LX_Mixer::groupChannels(AUDIOHANDLER_PLAYER_FROM, AUDIOHANDLER_PLAYER_TO,
                            AUDIOHANDLER_PLAYER_TAG);

    LX_Mixer::groupChannels(AUDIOHANDLER_VOICE_FROM, AUDIOHANDLER_VOICE_TO,
                            AUDIOHANDLER_VOICE_TAG);

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
        boss_music->play(true);
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

void AudioHDL::playShot(const LX_Graphics::LX_ImgCoord& pos)
{
    if(basic_shot != nullptr)
    {
        LX_MixerEffect effect{{true, false, false, false}, 0,0,0,0,0, false, 0};
        effect.type = {true, false, false, false};
        effect.pan_right = static_cast<uint8_t>(pos.x * MAX_PAN / MAX_X);
        effect.pan_left  = MAX_PAN - effect.pan_right;
        groupPlayChunk(*basic_shot, AUDIOHANDLER_PLAYER_TAG, effect);
    }
}

void AudioHDL::playRocketShot()
{
    if(rocket_shot != nullptr)
        groupPlayChunk(*rocket_shot, AUDIOHANDLER_PLAYER_TAG);
}

void AudioHDL::playLaserShot()
{
    if(laser_shot != nullptr)
        groupPlayChunk(*laser_shot, AUDIOHANDLER_PLAYER_TAG);
}

void AudioHDL::playPlayerExplosion()
{
    if(pexplosion != nullptr)
        groupPlayChunk(*pexplosion, AUDIOHANDLER_PLAYER_TAG);
}

void AudioHDL::playSmallExplosion()
{
    if(sexplosion != nullptr)
        sexplosion->play();
}

void AudioHDL::playMediumExplosion()
{
    if(mexplosion != nullptr)
        mexplosion->play();
}

void AudioHDL::playBigExplosion()
{
    if(bexplosion != nullptr)
        bexplosion->play();
}

void AudioHDL::playExplosion(const LX_Graphics::LX_ImgCoord& pos)
{
    if(explosion != nullptr)
    {
        /*LX_MixerEffect effect;
        effect.type = {true, false, false, false};
        effect.pan_right = static_cast<uint8_t>(pos.x * MAX_PAN / MAX_X);
        effect.pan_left  = MAX_PAN - effect.pan_right;
        effect.loops = 0;
        LX_Mixer::groupPlayChunk(*explosion, -1, effect);*/
        explosion->play();
    }
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


void AudioHDL::playHit(short hit_level)
{
    switch(hit_level)
    {
    case 1:
        LX_Mixer::groupPlayChunk(*hits01, AUDIOHANDLER_PLAYER_TAG);
        break;

    case 2:
        LX_Mixer::groupPlayChunk(*hits02, AUDIOHANDLER_PLAYER_TAG);
        break;

    case 3:
        LX_Mixer::groupPlayChunk(*hits03, AUDIOHANDLER_PLAYER_TAG);
        break;

    case 4:
        LX_Mixer::groupPlayChunk(*hits04, AUDIOHANDLER_PLAYER_TAG);
        break;

    default:
        break;
    }
}

void AudioHDL::playAlert(bool critical)
{
    LX_Mixer::LX_Chunk& ch = critical ? *alert_critical : *alert_normal;
    LX_Mixer::groupPlayChunk(ch, AUDIOHANDLER_ALERT_TAG, -1);
}

void AudioHDL::stopAlert()
{
    LX_Mixer::haltChannel(AUDIOHANDLER_ALERT_CHAN);
}

void AudioHDL::playEnemyHit()
{
    ehits->play();
}


AudioHDL::~AudioHDL()
{
    delete main_music;
    delete boss_music;
    LX_Mixer::reserveChannels(0);
    LX_Mixer::allocateChannels(AUDIOHANDLER_N_CHANNELS);
}

}
