

/*
*	Target_Xplosion - A classic shoot'em up video game
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
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/


#ifndef ENEMYRESOURCES_HPP_INCLUDED
#define ENEMYRESOURCES_HPP_INCLUDED

/**
*	@file EnemyResourceManager.hpp
*	@brief A file that manages enemy ressources
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "../asset/TX_Asset.hpp"

struct SDL_Texture;

class EnemyResourceManager
{
    std::array<SDL_Texture*,NB_ENEMIES> enemy_resources;

public:

    EnemyResourceManager();

    // Return a pointer to SDL_Texture. No need to free the texture
    SDL_Texture * getTextureAt(unsigned int index);

    ~EnemyResourceManager();
};

#endif // ENEMYRESOURCES_HPP_INCLUDED
