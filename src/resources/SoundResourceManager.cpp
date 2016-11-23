
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
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
*   website : https://gumichan01.github.io/
*   mail : luxon.jean.pierre@gmail.com
*/

#include "SoundResourceManager.hpp"
#include "../asset/TX_Asset.hpp"

#include <LunatiX/LX_Chunk.hpp>

namespace
{
std::array<LX_Mixer::LX_Sound*,NB_SOUNDS> sound_resources;
};

SoundResourceManager::SoundResourceManager()
{
    const TX_Asset *asset = TX_Asset::getInstance();
    sound_resources.fill(nullptr);

    for(unsigned int i = 0; i < sound_resources.size(); i++)
    {
        std::string str = asset->getSound(i);
        sound_resources[i] = new LX_Mixer::LX_Chunk(str);
    }
}

LX_Mixer::LX_Sound * SoundResourceManager::getSoundAt(unsigned int index) const
{
    if(index > sound_resources.size() || sound_resources[index] == nullptr)
        return nullptr;

    return sound_resources[index];
}

SoundResourceManager::~SoundResourceManager()
{
    // Free the resources
    for(unsigned int i = 0; i < sound_resources.size(); i++)
    {
        if(sound_resources[i] != nullptr)
            delete sound_resources[i];
    }
}
