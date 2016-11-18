
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

#ifndef RANK_HPP_INCLUDED
#define RANK_HPP_INCLUDED

const unsigned int NO_RANK = 0;
const unsigned int C_RANK = 0;
const unsigned int B_RANK = 1;
const unsigned int A_RANK = 2;
const unsigned int S_RANK = 3;

namespace Rank
{
void init();
void setRank(unsigned int r);
unsigned int getRank();

unsigned int attackPlayerUp(const unsigned int attack);
unsigned int shieldUp(const unsigned int sh);
unsigned int healthUp(const unsigned int hp);
};

#endif // RANK_HPP_INCLUDED

