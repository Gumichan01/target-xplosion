

/*
*   Target_Xplosion - A classic shoot'em up video game
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


#ifndef BACHI_HPP_INCLUDED
#define BACHI_HPP_INCLUDED


/**
*	@file Bachi.hpp
*	@brief File that defines a bee (bachi in Japanese)
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Enemy.hpp"


class Bachi : public Enemy
{
    void initBachi();

public:

    Bachi(unsigned int hp, unsigned int att, unsigned int sh,
          SDL_Texture *image, LX_Mixer::LX_Sound *audio,
          Sint16 x, Sint16 y, Uint16 w, Uint16 h,float vx, float vy);

    void fire();
    void reaction(Missile *target);

    ~Bachi();

};

#endif // BACHI_HPP_INCLUDED

