
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

#ifndef SOUNDRESOURCEMANAGER_HPP_INCLUDED
#define SOUNDRESOURCEMANAGER_HPP_INCLUDED


namespace LX_Mixer
{
class LX_Chunk;
}

class SoundResourceManager
{

    SoundResourceManager(const SoundResourceManager&) = delete;
    SoundResourceManager(const SoundResourceManager&&) = delete;
    SoundResourceManager& operator =(const SoundResourceManager&) = delete;
    SoundResourceManager& operator =(const SoundResourceManager&&) = delete;

public:

    SoundResourceManager();
    // Return a pointer to LX_Sound. No need to free it
    LX_Mixer::LX_Chunk * getSoundAt(unsigned int index) const noexcept;
    ~SoundResourceManager();
};

#endif // SOUNDRESOURCEMANAGER_HPP_INCLUDED
