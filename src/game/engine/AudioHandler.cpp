
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

// Audio engine
#include "AudioHandler.hpp"
#include "../../asset/TX_Asset.hpp"
#include "../../resources/ResourceManager.hpp"

#include <Lunatix/Audio.hpp>
#include <Lunatix/ImgRect.hpp>


#if defined(linux) || defined(__linux) || defined(__linux__)
#define TX_PANNING 1
#endif

using namespace lx::Mixer;

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

const int AUDIOHANDLER_MENU_SELECT_ID = 22;
const int AUDIOHANDLER_MENU_SELECTED_ID = 23;
const int AUDIOHANDLER_MENU_BACK_ID = 24;

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

const lx::Mixer::MixerEffect EFFECT_NONE;

}


namespace AudioHandler
{

static AudioHDL * audio_instance = nullptr;

AudioHDL * AudioHDL::init()
{
    if ( audio_instance == nullptr )
        audio_instance = new AudioHDL();

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


AudioHDL::AudioHDL()
    : main_music( nullptr ), boss_music( nullptr ), alarm( nullptr ),
      basic_shot( nullptr ), rocket_shot( nullptr ), laser_shot( nullptr ),
      pexplosion( nullptr ), sexplosion( nullptr ), mexplosion( nullptr ),
      bexplosion( nullptr ), explosion( nullptr ), txv_boss( nullptr ),
      txv_rocket( nullptr ), txv_shield( nullptr ), txv_pulse( nullptr ),
      txv_wave( nullptr ), txv_mother( nullptr ), hits01( nullptr ),
      hits02( nullptr ), hits03( nullptr ), hits04( nullptr ),
      alert_normal( nullptr ), alert_critical( nullptr ), ehits( nullptr ),
      bulletx( nullptr ), menu_select( nullptr ), menu_selected( nullptr ),
      menu_back( nullptr )
{
    const ResourceManager * const RC = ResourceManager::getInstance();

    alarm          = RC->getSound( AUDIOHANDLER_ALARM_ID );
    basic_shot     = RC->getSound( AUDIOHANDLER_SHOT_ID );
    rocket_shot    = RC->getSound( AUDIOHANDLER_ROCKET_ID );
    laser_shot     = RC->getSound( AUDIOHANDLER_LASER_ID );
    pexplosion     = RC->getSound( AUDIOHANDLER_PEXPLOSION_ID );
    sexplosion     = RC->getSound( AUDIOHANDLER_SEXPLOSION_ID );
    mexplosion     = RC->getSound( AUDIOHANDLER_MEXPLOSION_ID );
    bexplosion     = RC->getSound( AUDIOHANDLER_BEXPLOSION_ID );
    explosion      = RC->getSound( AUDIOHANDLER_EXPLOSION_ID );
    txv_boss       = RC->getSound( AUDIOHANDLER_VOICE_BOSS_ID );
    txv_rocket     = RC->getSound( AUDIOHANDLER_VOICE_ROCKET_ID );
    txv_shield     = RC->getSound( AUDIOHANDLER_VOICE_SHIELD_ID );
    txv_pulse      = RC->getSound( AUDIOHANDLER_VOICE_PULSE_ID );
    txv_wave       = RC->getSound( AUDIOHANDLER_VOICE_WAVE_ID );
    txv_mother     = RC->getSound( AUDIOHANDLER_VOICE_MOTHER_ID );
    hits01         = RC->getSound( AUDIOHANDLER_HITS01_ID );
    hits02         = RC->getSound( AUDIOHANDLER_HITS02_ID );
    hits03         = RC->getSound( AUDIOHANDLER_HITS03_ID );
    hits04         = RC->getSound( AUDIOHANDLER_HITS04_ID );
    alert_normal   = RC->getSound( AUDIOHANDLER_ALERT_NORMAL_ID );
    alert_critical = RC->getSound( AUDIOHANDLER_ALERT_CRITICAL_ID );
    ehits          = RC->getSound( AUDIOHANDLER_EHITS_ID );
    menu_select    = RC->getSound( AUDIOHANDLER_MENU_SELECT_ID );
    menu_selected  = RC->getSound( AUDIOHANDLER_MENU_SELECTED_ID );
    menu_back      = RC->getSound( AUDIOHANDLER_MENU_BACK_ID );
    lx::Mixer::allocateChannels( AUDIOHANDLER_G_CHANNELS );

    if ( alarm == nullptr )
        lx::Log::logCritical( lx::Log::LogType::APPLICATION, "AudioHDL — Cannot load the alarm" );

    // Channel group tags
    lx::Mixer::groupChannel( AUDIOHANDLER_ALARM_CHAN, AUDIOHANDLER_ALARM_TAG );
    lx::Mixer::groupChannel( AUDIOHANDLER_ALERT_CHAN, AUDIOHANDLER_ALERT_TAG );

    lx::Mixer::groupChannels( AUDIOHANDLER_PLAYER_FROM, AUDIOHANDLER_PLAYER_TO,
                             AUDIOHANDLER_PLAYER_TAG );

    lx::Mixer::groupChannels( AUDIOHANDLER_VOICE_FROM, AUDIOHANDLER_VOICE_TO,
                             AUDIOHANDLER_VOICE_TAG );

    // Reserve channels
    lx::Mixer::reserveChannels( AUDIOHANDLER_RESERVE_CHANNELS );
}

void AudioHDL::setLevel( const unsigned int lvid )
{
    delete boss_music;
    delete main_music;
    const TX_Asset * const ASSET = TX_Asset::getInstance();

    main_music = new lx::Mixer::Music( ASSET->getLevelMusic( lvid ) );

    if ( lvid % 2 == 1 )
        boss_music = new lx::Mixer::Music( ASSET->getLevelMusic( AUDIOHANDLER_BOSS_M1_ID ) );
    else
        boss_music = new lx::Mixer::Music( ASSET->getLevelMusic( AUDIOHANDLER_BOSS_M2_ID ) );
}


void AudioHDL::playMainMusic() noexcept
{
    if ( main_music != nullptr )
        main_music->play();
}

void AudioHDL::stopMainMusic() noexcept
{
    if ( main_music != nullptr )
        main_music->stop();
}

void AudioHDL::playBossMusic() noexcept
{
    if ( boss_music != nullptr )
        boss_music->play( true );
}

void AudioHDL::stopBossMusic() noexcept
{
    if ( boss_music != nullptr )
        boss_music->stop();
}

void AudioHDL::playAlarm() noexcept
{
    if ( alarm != nullptr )
        alarm->play( AUDIOHANDLER_ALARM_CHAN, 0, AUDIOHANDLER_ALARM_DELAY );
}

#ifdef TX_PANNING
void AudioHDL::playShot( const lx::Graphics::ImgCoord& pos ) noexcept
{
    if ( basic_shot != nullptr )
    {
        lx::Mixer::MixerEffect effect;
        effect.type.panning = true;
        effect.pan_right = static_cast<uint8_t>( pos.x * MAX_PAN / MAX_X );
        effect.pan_left  = MAX_PAN - effect.pan_right;
        lx::Mixer::groupPlayChunk( *basic_shot, AUDIOHANDLER_PLAYER_TAG, effect );
    }
}
#else
void AudioHDL::playShot( const lx::Graphics::ImgCoord& ) noexcept
{
    if ( basic_shot != nullptr )
    {
        lx::Mixer::groupPlayChunk( *basic_shot, AUDIOHANDLER_PLAYER_TAG );
    }
}
#endif

void AudioHDL::playRocketShot() noexcept
{
    if ( rocket_shot != nullptr )
        lx::Mixer::groupPlayChunk( *rocket_shot, AUDIOHANDLER_PLAYER_TAG );
}

void AudioHDL::playLaserShot() noexcept
{
    if ( laser_shot != nullptr )
        lx::Mixer::groupPlayChunk( *laser_shot, AUDIOHANDLER_PLAYER_TAG );
}

void AudioHDL::playPlayerExplosion() noexcept
{
    if ( pexplosion != nullptr )
        lx::Mixer::groupPlayChunk( *pexplosion, AUDIOHANDLER_PLAYER_TAG );
}

void AudioHDL::playSmallExplosion() noexcept
{
    if ( sexplosion != nullptr )
        sexplosion->play();
}

void AudioHDL::playMediumExplosion() noexcept
{
    if ( mexplosion != nullptr )
        mexplosion->play();
}

void AudioHDL::playBigExplosion() noexcept
{
    if ( bexplosion != nullptr )
        bexplosion->play();
}

#ifdef TX_PANNING
void AudioHDL::playExplosion( const lx::Graphics::ImgCoord& pos ) noexcept
{
    if ( explosion != nullptr )
    {
        lx::Mixer::MixerEffect effect;
        effect.type.panning = true;
        effect.pan_right = static_cast<uint8_t>( pos.x * MAX_PAN / MAX_X );
        effect.pan_left  = MAX_PAN - effect.pan_right;
        lx::Mixer::groupPlayChunk( *explosion, -1, effect );
    }
}


#else
void AudioHDL::playExplosion( const lx::Graphics::ImgCoord& ) noexcept
{
    if ( explosion != nullptr )
    {
        explosion->play();
    }
}
#endif


void AudioHDL::playVoiceBoss() noexcept
{
    if ( txv_boss != nullptr )
        lx::Mixer::groupPlayChunk( *txv_boss, AUDIOHANDLER_VOICE_TAG );
}

void AudioHDL::playVoiceRocket() noexcept
{
    if ( txv_rocket != nullptr )
        lx::Mixer::groupPlayChunk( *txv_rocket, AUDIOHANDLER_VOICE_TAG );
}

void AudioHDL::playVoiceShield() noexcept
{
    if ( txv_shield != nullptr )
        lx::Mixer::groupPlayChunk( *txv_shield, AUDIOHANDLER_VOICE_TAG );
}

void AudioHDL::playVoicePulse() noexcept
{
    if ( txv_pulse != nullptr )
        lx::Mixer::groupPlayChunk( *txv_pulse, AUDIOHANDLER_VOICE_TAG );
}

void AudioHDL::playVoiceWave() noexcept
{
    if ( txv_wave != nullptr )
        lx::Mixer::groupPlayChunk( *txv_wave, AUDIOHANDLER_VOICE_TAG );
}

void AudioHDL::playVoiceMother() noexcept
{
    if ( txv_mother != nullptr )
        lx::Mixer::groupPlayChunk( *txv_mother, AUDIOHANDLER_VOICE_TAG );
}


void AudioHDL::playHit( short hit_level ) noexcept
{
    switch ( hit_level )
    {
    case 1:
        lx::Mixer::groupPlayChunk( *hits01, AUDIOHANDLER_PLAYER_TAG );
        break;

    case 2:
        lx::Mixer::groupPlayChunk( *hits02, AUDIOHANDLER_PLAYER_TAG );
        break;

    case 3:
        lx::Mixer::groupPlayChunk( *hits03, AUDIOHANDLER_PLAYER_TAG );
        break;

    case 4:
        lx::Mixer::groupPlayChunk( *hits04, AUDIOHANDLER_PLAYER_TAG );
        break;

    default:
        break;
    }
}

void AudioHDL::playAlert( bool critical ) noexcept
{
    lx::Mixer::MixerEffect effect;
    effect.loops = -1;
    lx::Mixer::Chunk& ch = critical ? *alert_critical : *alert_normal;
    lx::Mixer::groupPlayChunk( ch, AUDIOHANDLER_ALERT_TAG, effect );
}

void AudioHDL::stopAlert() noexcept
{
    if ( lx::Mixer::isPlaying( AUDIOHANDLER_ALERT_CHAN ) )
        lx::Mixer::haltChannel( AUDIOHANDLER_ALERT_CHAN );
}

void AudioHDL::playEnemyHit() noexcept
{
    ehits->play();
}

void AudioHDL::playMenuSelect() noexcept
{
    menu_select->play();
}

void AudioHDL::playMenuSelected() noexcept
{
    menu_selected->play();
}

void AudioHDL::playMenuBack() noexcept
{
    menu_back->play();
}

void AudioHDL::haltAudio() noexcept
{
    lx::Mixer::haltChannel( -1 );
}

AudioHDL::~AudioHDL()
{
    delete main_music;
    delete boss_music;
    lx::Mixer::reserveChannels( 0 );
    lx::Mixer::allocateChannels( AUDIOHANDLER_N_CHANNELS );
}

}
