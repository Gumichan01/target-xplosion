
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

#ifndef BOSS03_HPP_INCLUDED
#define BOSS03_HPP_INCLUDED

#include "Boss.hpp"


namespace LX_Graphics
{
class LX_Sprite;
}

namespace LX_Physics
{
//struct LX_Circle;
//class LX_Polygon;
}


class Boss03: public Enemy
{
    //LX_Graphics::LX_Sprite *body_sprite;
    //LX_Graphics::LX_Sprite *head_sprite;
    static const int BOSS03_PARTS = 2;
    Boss *boss_parts[BOSS03_PARTS];

public:

    explicit Boss03(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                    float vx, float vy);

    virtual void draw();
    virtual void fire();
    virtual void strategy();
    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    virtual void die();

    ~Boss03() = default;
};

#endif // BOSS03_HPP_INCLUDED
