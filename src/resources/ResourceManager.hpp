
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

#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

#include "EnemyResourceManager.hpp"
#include "MissileResourceManager.hpp"
#include "PlayerResourceManager.hpp"
#include "SoundResourceManager.hpp"
#include "ExplosionResourceManager.hpp"
#include "MenuResourceManager.hpp"

enum RESOURCE_TYPE: short {RC_ENEMY, RC_MISSILE, RC_XPLOSION};

namespace LX_Graphics
{
class LX_Sprite;
}

class ResourceManager
{
    EnemyResourceManager enemy_rc;
    MissileResourceManager missile_rc;
    PlayerResourceManager player_rc;
    SoundResourceManager sound_rc;
    ExplosionResourceManager explosion_rc;
    MenuResourceManager menu_rc;

    ResourceManager();
    ResourceManager(ResourceManager& m);
    ResourceManager(ResourceManager&& m);
    ResourceManager& operator =(ResourceManager m);
    ResourceManager& operator =(const ResourceManager& m);
    ResourceManager& operator =(ResourceManager&& m);
    ~ResourceManager() = default;

public:

    static void init();
    static ResourceManager *getInstance();
    static void destroy();
    LX_Graphics::LX_Sprite * getResource(const RESOURCE_TYPE& ty, unsigned int index) const;
    LX_Graphics::LX_Sprite * getPlayerResource(bool with_shield = false) const;
    LX_Graphics::LX_Sprite * getMenuResource(unsigned int index) const;
    LX_Mixer::LX_Chunk * getSound(unsigned int index) const;

};

#endif // RESOURCEMANAGER_HPP_INCLUDED

