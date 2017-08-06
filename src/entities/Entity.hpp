
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

#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_AABB.hpp>
#include <cstdint>


namespace LX_Graphics
{
class LX_Sprite;
}

namespace LX_Physics
{
struct LX_Circle;
}

class Enemy;

struct FloatPosition
{
    float x;
    float y;

    FloatPosition();
    FloatPosition(float fx, float fy);
    FloatPosition(const FloatPosition& fp);
    explicit FloatPosition(const LX_AABB& b);
    FloatPosition& operator =(const FloatPosition& fp);
    FloatPosition& operator =(const LX_AABB& aabb);
    FloatPosition& operator =(const LX_Physics::LX_Circle& circle);
    FloatPosition& operator +=(const LX_Physics::LX_Vector2D& v);

    void toPixelUnit(LX_AABB& aabb);
    void toPixelUnit(LX_Physics::LX_Circle& circle);
};


class Entity
{

protected:

    LX_Graphics::LX_Sprite *graphic;
    LX_AABB position;
    FloatPosition fpos;
    LX_Physics::LX_Vector2D speed;
    bool still_alive;

public:

    Entity();

    Entity(LX_Graphics::LX_Sprite *image, const LX_AABB& rect,
           const LX_Physics::LX_Vector2D& sp);

    virtual void move() = 0;
    virtual void die();
    virtual void draw();
    bool isDead() const;

    virtual void setX(int newX);
    virtual void setY(int newY);
    void setXvel(float xvel);
    void setYvel(float yvel);

    float getXvel() const;
    float getYvel() const;
    virtual int getX() const;
    virtual int getY() const;
    virtual int getWidth() const;
    virtual int getHeight() const;

    virtual ~Entity() {}
};


#endif // ENTITY_H_INCLUDED
