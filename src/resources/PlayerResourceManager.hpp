

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

#ifndef PLAYERRESOURCEMANAGER_HPP_INCLUDED
#define PLAYERRESOURCEMANAGER_HPP_INCLUDED

/**
*	@file PlayerResourceManager.hpp
*	@brief A file that manages the player ressource
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <array>
#include "../xml/XMLReader.hpp"

namespace LX_FileIO
{
class LX_FileBuffer;
};

struct SDL_Texture;


class PlayerResourceManager
{
    LX_FileIO::LX_FileBuffer *player_without_sh;
    LX_FileIO::LX_FileBuffer *player_with_sh;

public:

    PlayerResourceManager();
    SDL_Texture * loadTexture(bool shield = false);
    ~PlayerResourceManager();


};







#endif // PLAYERRESOURCEMANAGER_HPP_INCLUDED


























