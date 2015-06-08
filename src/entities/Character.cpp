
/*
*	Target_Xplosion - The classic shoot'em up video game
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
*	@file Character.cpp
*	@brief The charcater file
*	@author Luxon Jean-Pierre (Gumichan01)
*
*/

#include "Character.hpp"
#include "../engine/LX_Vector2D.hpp"


Character::Character(unsigned int hp, unsigned int att, unsigned int sh,
                     SDL_Texture *image, LX_Chunk *audio,
                     int x, int y, int w, int h,int dX, int dY)
    : Entity(image, audio, x, y, w, h, dX, dY)
{
    characterInit(hp,att,sh);
}



Character::Character(unsigned int hp, unsigned int att, unsigned int sh,
                     SDL_Texture *image, LX_Chunk *audio, SDL_Rect *rect,LX_Vector2D *sp)
    : Entity(image, audio, rect, sp)
{
    characterInit(hp,att,sh);
}


void Character::characterInit(unsigned int hp, unsigned int att, unsigned int sh)
{
    health_point = hp;
    max_health_point = hp;
    attack_val = att;
    shield = sh;
    laser_delay = LASER_LIFETIME;
}


void Character::receive_damages(unsigned int attacks)
{
    if(health_point != 0)           // if the character's health is not equal to 0, he may take damages
    {
        if(shield > attacks)        // if the character's shield is better
            health_point -= 1;      // he takes a symbolic one damage point
        else
        {
            if( (attacks - shield) < health_point )         // If the character has more health point than damage
                health_point -= (attacks - shield) + 1;     // Decrease the health point
            else
                health_point = 0;                           // One shoot
        }

        if(health_point == 0)
        {
            kill();
        }
    }
}


void Character::kill(void)
{
    wasKilled = true;
    die();
}


unsigned int Character::getHP()
{
    return health_point;
}


unsigned int Character::getMAX_HP()
{
    return max_health_point;
}


unsigned int Character::getATT()
{
    return attack_val;
}


unsigned int Character::getDEF()
{
    return shield;
}


bool Character::killed()
{
    return wasKilled;
}


void Character::setHP(unsigned int newHP)
{
    health_point = newHP;
}


void Character::setMAX_HP(unsigned int newMAX_HP)
{
    max_health_point = newMAX_HP;
}


void Character::setATT(unsigned int newATT)
{
    attack_val = newATT;
}


void Character::setDEF(unsigned int newDEF)
{
    shield = newDEF;
}


