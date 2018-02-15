
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
}


class Boss02: public Boss
{
    LX_AABB global_hitbox;
    LX_AABB shield_hitbox;
    PolygonShape shape;
    LX_Graphics::LX_Sprite *sprite;
    LX_Graphics::LX_Sprite *sh_sprite;
    bool has_shield;
    bool shield_destroyed;
    unsigned int b1time;
    unsigned int rshield_life;
    FloatPosition gfpos;
    FloatPosition shpos;

    void changeShotStrat(const unsigned int d);
    void prepareTheAttack();
    void engage();
    void meshAttack();
    void targetAttack();
    void bulletAttack();
    void mesh();
    void target();
    void danmaku();
    void absorb(Missile *m);

public:

    explicit Boss02(unsigned int hp, unsigned int att, unsigned int sh,
                    LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                    float vx, float vy);

    virtual void fire();
    virtual void strategy();
    virtual void move();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    virtual void die();

    ~Boss02() = default;
};

#endif // BOSS02_HPP_INCLUDED
