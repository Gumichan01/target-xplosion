

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


#ifndef ENEMYMISSILERESOURCESMANAGER_HPP_INCLUDED
#define ENEMYMISSILERESOURCESMANAGER_HPP_INCLUDED

/**
*	@file MissileResourceManager.hpp
*	@brief A file that manages enemy missile ressources
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <array>
#include "../xml/XMLReader.hpp"

#define RC_MISSILES (PLAYER_MISSILES+ENEMY_MISSILES)

struct SDL_Surface;
struct SDL_Texture;

class MissileResourceManager
{
    std::array<SDL_Surface*,RC_MISSILES> missile_resources;

public:

    MissileResourceManager();

    /*
        Return a fresh SDL_Texture. This texture is allocated.
        So, the user is responsible of freeing the texture
    */
    SDL_Texture * loadTextureAt(unsigned int index);

    ~MissileResourceManager();
};


#endif // ENEMYMISSILERESOURCESMANAGER_HPP_INCLUDED






