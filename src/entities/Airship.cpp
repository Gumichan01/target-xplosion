
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

#include "Airship.hpp"

#include "Player.hpp"
#include "Bullet.hpp"
#include "Bomb.hpp"
#include "../pattern/Strategy.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../game/engine/AudioHandler.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Texture.hpp>
#include <vector>

using namespace LX_Physics;

namespace
{
const int AIRSHIP_WIDTH = 250;
const int AIRSHIP_HEIGHT = 100;

const int AIRSHIP_FRONT_XPOS = 768;
const int AIRSHIP_FRONT_YPOS = 255;
const int AIRSHIP_BOTTOM_YPOS = 261;

// Bomb
const int AIRSHIP_BOMB_ID = 10;
const int AIRSHIP_BOMB_XOFF = 116;
const int AIRSHIP_BOMB_YOFF = 68;
const int AIRSHIP_BOMB_DIM = 32;
const Float AIRSHIP_BOMB_VEL{10.0f};
const int AIRSHIP_BOMB_NUM = BulletPattern::CIRCLE_BULLETS / 2;
const unsigned int AIRSHIP_BOMB_DELAY = 500;

// Front shot
const int AIRSHIP_FSHOT_ID = 4;
const int AIRSHIP_FSHOT_XOFF = 16;
const int AIRSHIP_FSHOT_YOFF = 32;
const int AIRSHIP_FSHOT_W = 32;
const int AIRSHIP_FSHOT_H = 12;
const Float AIRSHIP_FSHOT_VEL = {10.0f};
const int AIRSHIP_FSHOT_NUM = BulletPattern::CIRCLE_BULLETS * 2;
const unsigned int AIRSHIP_FSHOT_DELAY = 500;

// Spin
const int AIRSHIP_SPIN1_ID = 8;
const int AIRSHIP_SPIN2_ID = 4;
const int AIRSHIP_SPIN_XOFF = 124;
const int AIRSHIP_SPIN_YOFF = 76;
const Float AIRSHIP_SPIN_DIM = {24.0f};
const Float AIRSHIP_SPIN_VEL = {6.0f};
const unsigned int AIRSHIP_SPIN_DELAY = 50;
const Float AIRSHIP_STEP = BulletPattern::PI_F / Float{16.0f};
const int AIRSHIP_SPIN_YMIN = 100;
const int AIRSHIP_SPIN_YMAX = 400;
const int AIRSHIP_SPIN_V = 2;

using LX_Physics::LX_FloatPosition;
const std::vector<LX_FloatPosition> hpoints
{
    LX_FloatPosition{12.0f,38.0f}, LX_FloatPosition{24.0f,18.0f},
    LX_FloatPosition{120.0f,6.0f}, LX_FloatPosition{222.0f,18.0f},
    LX_FloatPosition{248.0f,38.0f}, LX_FloatPosition{222.0f,64.0f},
    LX_FloatPosition{184.0f,70.0f}, LX_FloatPosition{156.0f,96.0f},
    LX_FloatPosition{61.0f,96.0f}, LX_FloatPosition{45.0f,68.0f},
    LX_FloatPosition{24.0f,58.0f}
};

using LX_Graphics::LX_ImgCoord;
inline constexpr LX_FloatPosition toFloatPos(const LX_ImgCoord& p) noexcept
{
    return LX_FloatPosition{Float{static_cast<float>(p.x)},
                            Float{static_cast<float>(p.y)}};
}

}


Airship::Airship(unsigned int hp, unsigned int att, unsigned int sh,
                 LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                 float vx, float vy)
    : LargeEnemy(hp, att, sh, image, x, y, w, h, vx, vy), idstrat(0),
      shape(hpoints, LX_Physics::LX_FloatPosition{toFloat(x), toFloat(y)}),
      pattern1(AIRSHIP_SPIN_VEL, AIRSHIP_STEP, fbox(0.0f)),
      pattern2(AIRSHIP_SPIN_VEL, AIRSHIP_STEP, BulletPattern::PI_F / fbox(2.0f))
{
    phybox.w = AIRSHIP_WIDTH;
    phybox.h = AIRSHIP_HEIGHT;
    mvs->addMoveStrat(new MoveStrategy(this));
    addStrategy(mvs);
}


void Airship::boom() noexcept
{
    AudioHandler::AudioHDL::getInstance()->playMediumExplosion();
}

void Airship::move() noexcept
{
    //moveBox(main_hitbox, speed);
    movePoly(shape.getPoly(), speed);
    Enemy::move();
}

void Airship::draw() noexcept
{
    if(dying)
    {
        const int N = 9;
        LX_Graphics::LX_ImgRect box[N] = {{24,32,64,64}, {64,10,64,64}, {48,64,64,64},
            {64,80,64,64}, {130,76,64,64}, {110,8,64,64}, {91,51,64,64},
            {174,24,64,64}, {226,32,64,64}
        };

        position = LX_Graphics::toImgRect(phybox);

        for(int i = 0; i < N; i++)
        {
            box[i].p.x += position.p.x;
            box[i].p.y += position.p.y;
            graphic->draw(box[i]);
        }
    }
    else
        LargeEnemy::draw();
}

void Airship::collision(Missile *mi) noexcept
{
    if(!mi->isDead() && !mi->explosion()
            && mi->getX() <= (phybox.fpoint.x + fbox(static_cast<float>(phybox.w)))
            && !dying)
    {
        if(LX_Physics::collisionBox(phybox, mi->getHitbox()))
        {
            if(LX_Physics::collisionBoxPoly(mi->getHitbox(), shape.getPoly()))
            {
                if(destroyable)
                    reaction(mi);

                mi->die();
            }
        }
    }
}

void Airship::collision(Player *play) noexcept
{
    if(play->getX() <= (phybox.fpoint.x + fbox(static_cast<float>(phybox.w))) && !dying)
    {
        if(LX_Physics::collisionCircleBox(play->getHitbox(), phybox))
        {
            if(LX_Physics::collisionCirclePoly(play->getHitbox(), shape.getPoly()))
            {
                play->die();
            }
        }
    }
}

/// Strategy

void Airship::prepare() noexcept
{
    ShotStrategy *shot = nullptr;

    if(position.p.y < AIRSHIP_FRONT_YPOS)
    {
        idstrat = 1;
        shot = new ShotStrategy(this);
        shot->setShotDelay(AIRSHIP_BOMB_DELAY);
    }
    else if(position.p.y > AIRSHIP_BOTTOM_YPOS)
    {
        idstrat = 2;
        shot = new ShotStrategy(this);
        shot->setShotDelay(AIRSHIP_FSHOT_DELAY);
    }
    else
        idstrat = 3;

    mvs->addShotStrat(shot);
}

void Airship::aposition() noexcept
{
    if(position.p.x <= AIRSHIP_FRONT_XPOS && !isDying() && !isDead())
    {
        idstrat = 4;
        speed *= 0.0f;
        ShotStrategy *shot = new ShotStrategy(this);
        shot->setShotDelay(AIRSHIP_SPIN_DELAY);
        mvs->addShotStrat(shot);
        mvs->addMoveStrat(new UpDownMoveStrategy(this, AIRSHIP_SPIN_YMIN,
                          AIRSHIP_SPIN_YMAX, AIRSHIP_SPIN_V));
    }
}

void Airship::strategy() noexcept
{
    switch(idstrat)
    {
    case 0:
        prepare();
        break;

    case 3:
        aposition();
        break;

    default:
        break;
    }

    Enemy::strategy();
}


/// Fire

void Airship::bomb() noexcept
{
    LX_Graphics::LX_ImgRect bpos
    {
        position.p.x + AIRSHIP_BOMB_XOFF,
        position.p.y + AIRSHIP_BOMB_YOFF,
        AIRSHIP_BOMB_DIM, AIRSHIP_BOMB_DIM
    };

    const LX_Physics::LX_FloatPosition FLPOS{toFloat(bpos.p.x), toFloat(bpos.p.x)};
    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, AIRSHIP_BOMB_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    std::array<LX_Vector2D, AIRSHIP_BOMB_NUM> varray;
    BulletPattern::circlePattern(FLPOS.x, FLPOS.y, AIRSHIP_BOMB_VEL, varray);

    auto const _beg = varray.begin() + 1;
    auto const _end = varray.begin() + (varray.size()/2);

    for(auto it = _beg; it != _end; ++it)
    {
        hdl.pushEnemyMissile(*(new EnemyBomb(attack_val, spr, bpos, *it)));
    }
}

void Airship::frontShot() noexcept
{
    LX_Graphics::LX_ImgRect fspos
    {
        position.p.x + AIRSHIP_FSHOT_XOFF, position.p.y + AIRSHIP_FSHOT_YOFF,
        AIRSHIP_FSHOT_W, AIRSHIP_FSHOT_H
    };

    const LX_Physics::LX_FloatPosition FLPOS{toFloat(fspos.p.x), toFloat(fspos.p.x)};
    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, AIRSHIP_FSHOT_ID);

    EntityHandler& hdl = EntityHandler::getInstance();
    std::array<LX_Vector2D, AIRSHIP_FSHOT_NUM> varray;
    BulletPattern::circlePattern(FLPOS.x, FLPOS.y, AIRSHIP_FSHOT_VEL, varray);

    const auto _beg = varray.begin() + varray.size() - varray.size() / 4;
    const auto _end = varray.begin() + varray.size() / 4 + 1;

    for(auto it = _beg; it != _end; ++it)
    {
        if(it == varray.end())
            it = varray.begin();

        hdl.pushEnemyMissile(*(new Bullet(attack_val, spr, fspos, *it)));
    }
}

void Airship::doubleSpinShot() noexcept
{
    const int N = 4;
    const std::size_t AIRSHIP_N = 2;

    using namespace LX_Graphics;
    LX_Graphics::LX_ImgRect mbrect = {position.p.x + AIRSHIP_SPIN_XOFF,
                                      position.p.y + AIRSHIP_SPIN_YOFF,
                                      AIRSHIP_SPIN_DIM, AIRSHIP_SPIN_DIM
                                     };

    const LX_Physics::LX_FloatPosition p = toFloatPos(mbrect.p);

    LX_Sprite *sprite[AIRSHIP_N];
    sprite[0] = ResourceManager::getInstance()->getResource(RC_MISSILE, AIRSHIP_SPIN1_ID);
    sprite[1] = ResourceManager::getInstance()->getResource(RC_MISSILE, AIRSHIP_SPIN2_ID);

    // Execute the pattern
    LX_Vector2D v[N];
    pattern1(p.x, p.y, v[0]);
    pattern2(p.x, p.y, v[1]);
    v[2] = -v[0];
    v[3] = -v[1];

    EntityHandler& hdl = EntityHandler::getInstance();

    for(int i = 0; i < N; ++i)
    {
        hdl.pushEnemyMissile(*(new Bullet(attack_val, sprite[i%2], mbrect, v[i])));
    }
}

void Airship::fire() noexcept
{
    switch(idstrat)
    {
    case 1:
        bomb();
        break;

    case 2:
        frontShot();
        break;

    case 4:
        doubleSpinShot();
        break;

    default:
        break;
    }
}

/// End Fire


void Airship::die() noexcept
{
    if(!dying)
    {
        if((phybox.fpoint.x + fbox(static_cast<float>(phybox.w)) ) > fbox(0.0f))
            EntityHandler::getInstance().bulletCancel();
    }

    Enemy::die();
}
