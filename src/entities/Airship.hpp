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

#ifndef AIRSHIP_H
#define AIRSHIP_H

#include "Enemy.hpp"
#include "../pattern/BulletPattern.hpp"

namespace LX_Graphics
{
class LX_Sprite;
}


class Airship : public LargeEnemy
{
    int idstrat;
    LX_AABB main_hitbox;
    PolygonShape shape;
    BulletPattern::SpinShot pattern1, pattern2;

    // Strategy
    void prepare();
    void aposition();

    // Fire
    void bomb();
    void frontShot();
    void doubleSpinShot();

public:
    Airship(unsigned int hp, unsigned int att, unsigned int sh,
            LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
            float vx, float vy);

    virtual void boom();
    virtual void move();
    virtual void draw();
    virtual void collision(Missile *mi);
    virtual void collision(Player *play);
    virtual void strategy();
    virtual void fire();
    virtual void die();

    ~Airship() = default;
};

#endif // AIRSHIP_H
