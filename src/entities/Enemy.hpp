
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
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/


#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Character.hpp"
#include "PlayerVisitor.hpp"

class Player;
class Strategy;

namespace LX_Physics
{
struct LX_Circle;
};


class Enemy: public Character, public PlayerVisitor
{

protected:

    Strategy *strat;

public:

    Enemy(unsigned int hp, unsigned int att, unsigned int sh,
          LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
          int x, int y, int w, int h,float vx, float vy);


    void move();
    virtual void strategy();
    void receiveDamages(unsigned int attacks);

    virtual void fire();
    virtual void reaction(Missile *target);
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);

    void addStrategy(Strategy *new_strat);
    void deleteStrategy();
    virtual void boom();

    const LX_Physics::LX_Circle * getHitbox();

    virtual ~Enemy();
};

#endif // ENEMY_H_INCLUDED

