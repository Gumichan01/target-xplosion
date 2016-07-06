
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

/**
*	@file Rank.cpp
*	@brief The rank system implementation file
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Rank.hpp"

namespace
{
unsigned int player_rank;
}

// Set the default rank
void Rank::init(void)
{
    player_rank = NO_RANK;
}

// Change the rank
void Rank::setRank(unsigned int r)
{
    if(r <= S_RANK)
        player_rank = r;
}

unsigned int Rank::getRank(void)
{
    return player_rank;
}

// More attack for the player above
unsigned int Rank::attackPlayerUp(const unsigned int attack)
{
    if(player_rank == NO_RANK)
        return (attack + (attack/2));
    else if(player_rank == A_RANK)
        return (attack + (attack/3));

    return attack;
}

// More shield for the enemies
unsigned int Rank::shieldUp(const unsigned int sh)
{
    if(player_rank != NO_RANK)
        return (sh + (player_rank * sh));

    return sh;
}

// More health for bosses
unsigned int Rank::healthUp(const unsigned int hp)
{
    if(player_rank > NO_RANK && player_rank < S_RANK)
        return (hp * (player_rank +1));

    else if(player_rank == S_RANK)
        return hp * 6;

    return hp;
}
