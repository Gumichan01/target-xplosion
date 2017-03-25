
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
const int AUDIOHANDLER_BOSS1_ID = 7;
const int AUDIOHANDLER_BOSS2_ID = 8;
const int AUDIOHANDLER_ALARM_ID = 4;
const uint32_t AUDIOHANDLER_ALARM_DELAY = 5000;
};

namespace AudioHandler
{

AudioHandler::AudioHandler(const unsigned int lvid)
    : main_music(nullptr), boss_music(nullptr), alarm(nullptr)
{
    const TX_Asset *a = TX_Asset::getInstance();
    const ResourceManager *rc = ResourceManager::getInstance();
    main_music = new LX_Music(a->getLevelMusic(lvid));
    alarm = dynamic_cast<LX_Chunk*>(rc->getSound(AUDIOHANDLER_ALARM_ID));

    if(alarm == nullptr)
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION, "AudioHandler — Cannot load the alarm");

    if(lvid%2 == 1)
        boss_music = new LX_Music(a->getLevelMusic(AUDIOHANDLER_BOSS1_ID));
    else
        boss_music = new LX_Music(a->getLevelMusic(AUDIOHANDLER_BOSS2_ID));
}

void AudioHandler::playMainMusic()
{
    if(main_music != nullptr)
        main_music->play();
}

void AudioHandler::stopMainMusic()
{
    if(main_music != nullptr)
        main_music->stop();
}

void AudioHandler::playBossMusic()
{
    if(boss_music != nullptr)
        boss_music->play();
}

void AudioHandler::stopBossMusic()
{
    if(boss_music != nullptr)
        boss_music->stop();
}

void AudioHandler::playAlarm()
{
    if(alarm != nullptr)
        alarm->play(-1, 0, AUDIOHANDLER_ALARM_DELAY);
}

AudioHandler::~AudioHandler()
{
    delete main_music;
    delete boss_music;
}

}
