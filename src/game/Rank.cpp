
/*
*   Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2015  Luxon Jean-Pierre
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

/**
*	@file Rank.cpp
*	@brief The rank system implementation file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Rank.hpp"

static short player_rank;


void Rank::init(void)
{
    player_rank = NO_RANK;
}


void Rank::setRank(short r)
{
    if(r >= NO_RANK || r <= S_RANK)
    {
        player_rank = r;
    }
}


short Rank::getRank(void)
{
    return player_rank;
}






