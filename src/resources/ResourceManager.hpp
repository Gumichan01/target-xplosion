

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Luxon Jean-Pierre (Gumichan01)
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

/**
*	@file ResourceManager.hpp
*	@brief Define the ressource handler
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

struct SDL_Texture;
class EnemyResourcesManager;

typedef enum RESOURCE_TYPE {RC_ENEMY,RC_ENEMY_MISSILE,
                            RC_ITEM,RC_BOMB,RC_BULLET,
                            RC_PARTICLES
                           } RESOURCE_TYPE;

class ResourceManager
{
    EnemyResourcesManager *enemyManager;

public:

    ResourceManager();
    SDL_Texture * getResource(RESOURCE_TYPE ty, unsigned int index);
    ~ResourceManager();
};

#endif // RESOURCEMANAGER_HPP_INCLUDED













