
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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


class Player;
class Strategy;
class EnemyHUD;
class MoveAndShootStrategy;

namespace LX_Graphics
{
class LX_AnimatedSprite;
}

namespace LX_Physics
{
struct LX_Circle;
}


class Enemy: public Character
{
    Strategy *strat;

    Enemy(const Enemy&) = delete;
    Enemy(const Enemy&&) = delete;
    Enemy& operator =(const Enemy&) = delete;
    Enemy& operator =(const Enemy&&) = delete;

protected:

    LX_Graphics::LX_AnimatedSprite * xtexture;
    MoveAndShootStrategy *mvs;
    unsigned int tick;      // Time of destruction
    unsigned int ut;        // Time of invicibility
    bool destroyable;

    inline LX_Graphics::LX_ImgRect tobox(int x, int y, int w, int h) noexcept
    {
        return LX_Graphics::LX_ImgRect{x,y,w,h};
    }

public:

    static void loadExplosionBuffer();
    static void destroyExplosionBuffer() noexcept;

    Enemy(unsigned int hp, unsigned int att, unsigned int sh,
          LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
          float vx, float vy);

    void start() noexcept;
    virtual void move() noexcept override;
    virtual void strategy() noexcept;

    virtual void boom() noexcept;
    virtual void fire() noexcept;
    virtual void reaction(Missile *target) noexcept;
    virtual void receiveDamages(unsigned int attacks) noexcept;
    using Character::collision;
    virtual void collision(Missile *mi) noexcept override;
    virtual void collision(Player *play) noexcept;

    void addStrategy(Strategy *new_strat, bool delete_previous = true) noexcept;
    virtual void die() noexcept;

    virtual ~Enemy();
};


class BigEnemy: public Enemy
{
    EnemyHUD * ehud;

    BigEnemy(const BigEnemy&) = delete;
    BigEnemy(const BigEnemy&&) = delete;
    BigEnemy& operator =(const BigEnemy&) = delete;
    BigEnemy& operator =(const BigEnemy&&) = delete;

public:

    BigEnemy(unsigned int hp, unsigned int att, unsigned int sh,
             LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
             float vx, float vy);

    virtual void draw() noexcept override;
    virtual void reaction(Missile *target) noexcept override;
    virtual ~BigEnemy();
};


#endif // ENEMY_H_INCLUDED
