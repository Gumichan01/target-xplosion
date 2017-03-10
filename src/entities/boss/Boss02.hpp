
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017  Luxon Jean-Pierre
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

#ifndef BOSS02_HPP_INCLUDED
#define BOSS02_HPP_INCLUDED

#include "Boss.hpp"


namespace LX_Graphics
{
class LX_Sprite;
};

namespace LX_Physics
{
class LX_Polygon;
};


class Boss02: public Boss
{
    LX_AABB global_hitbox;
    LX_Physics::LX_Polygon *poly;
    uint32_t b1time;

    MoveAndShootStrategy * getMVSStrat();
    void changeMoveStrat(const uint32_t d);
    void b0position();
    void b1position();
    void b2position();
    void b3position();
    void mesh();
    void target();
    void danmaku();

public:

    explicit Boss02(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
                    int x, int y, int w, int h, float vx, float vy);

    virtual void fire();
    virtual void strategy();
    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    //virtual void die();

    ~Boss02();
};

#endif // BOSS02_HPP_INCLUDED
