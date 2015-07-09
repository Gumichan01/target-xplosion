#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED



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
*	@file Level.hpp
*	@brief The level library of Target Xplosion
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <iostream>
#include <queue>


#define ROCKET_LEVEL_MIN 5
#define BOMB_LEVEL_MIN 10
#define LASER_LEVEL_MIN 15


struct EnemyData;

using namespace std;

class Level
{
    queue<EnemyData*> enemy_queue;
    bool loaded;
    static unsigned int id;
    static unsigned long max_score;

    void pushData(const EnemyData *data);

public :

    Level();
    Level(const unsigned int lvl);

    bool load(const unsigned int lvl);
    bool isLoaded(void);
    bool statEnemyData(EnemyData *data);
    void popData(void);
    static unsigned int getLevelNum(void);
    static unsigned long getMaxScore(void);

    ~Level();
};


#endif // LEVEL_HPP_INCLUDED

