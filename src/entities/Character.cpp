
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

#include "Character.hpp"
#include "Missile.hpp"
#include "../game/Scoring.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/engine/AudioHandler.hpp"
#include "../resources/WinID.hpp"
#include "../asset/TX_Asset.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Timer.hpp>


inline unsigned int MIN(int a, int b)
{
    return static_cast<unsigned int>((a < b)?a:b);
}


Character::Character(unsigned int hp, unsigned int att, unsigned int sh,
                     LX_Graphics::LX_Sprite *image, const LX_AABB& rect,
                     const LX_Physics::LX_Vector2D& sp)
    : Entity(image, rect, sp), was_killed(false), health_point(hp),
      max_health_point(hp), attack_val(att), shield(sh), dying(false),
      hit_sprite(nullptr), hit_time(0), hit(false)
{
    characterInit();
    createHitSprite();
}


void Character::characterInit()
{
    int xCenter = position.x + (position.w/2);
    int yCenter = position.y + (position.h/2);
    unsigned int rad = MIN((xCenter - position.x), (yCenter - position.y));

    hitbox = LX_Physics::LX_Circle(LX_Physics::LX_Point(xCenter, yCenter), rad);
    box_fpos = hitbox;
}


void Character::createHitSprite()
{
    const TX_Asset *a = TX_Asset::getInstance();
    const TX_Anima *an = a->getEnemyAnimation(a->getID(graphic->getFileName()));
    LX_AABB * r = (an == nullptr ? nullptr : const_cast<LX_AABB *>(&(an->v[0])));

    LX_Win::LX_Window *w = LX_Win::getWindowManager()->getWindow(WinID::getWinID());
    LX_Graphics::LX_BufferedImage bf(graphic->getFileName());
    bf.convertNegative();

    hit_sprite = bf.generateSprite(*w, r);
}

void Character::destroyHitSprite()
{
    delete hit_sprite;
}


void Character::draw()
{
    if(hit && !dying)
    {
        if((LX_Timer::getTicks() - hit_time) > HIT_DELAY)
        {
            hit = false;
            hit_time = LX_Timer::getTicks();
        }

        hit_sprite->draw(&position);
    }
    else
        Entity::draw();
}


void Character::receiveDamages(unsigned int attacks)
{
    if(health_point != 0)
    {
        if(!hit && !dying)
        {
            if((LX_Timer::getTicks() - hit_time) > HIT_DELAY)
            {
                hit = true;
                hit_time = LX_Timer::getTicks();
            }
        }

        if(shield > attacks)
            health_point -= 1;      // 1 damage point if shield > attacks
        else
        {
            if((attacks - shield) < health_point)
                health_point -= (attacks - shield) + 1;
            else
                health_point = 0;
        }
    }
}

const LX_Physics::LX_Circle& Character::getHitbox() const
{
    return hitbox;
}

void Character::kill()
{
    was_killed = true;
    unsigned long sc = max_health_point + attack_val + shield;
    Engine::getInstance()->getScore()->notify(sc, true);
    die();
}


bool Character::isDying() const
{
    return dying;
}


unsigned int Character::getHP() const
{
    return health_point;
}

unsigned int Character::getMaxHP() const
{
    return max_health_point;
}

unsigned int Character::getATT() const
{
    return attack_val;
}

unsigned int Character::getDEF() const
{
    return shield;
}

void Character::setX(int newX)
{
    Entity::setX(newX);
    hitbox.center.x = newX + position.w /2;
    box_fpos.x = hitbox.center.x;
}

void Character::setY(int newY)
{
    Entity::setY(newY);
    hitbox.center.y = newY + position.h /2;
    box_fpos.y = hitbox.center.y;
}

Character::~Character()
{
    delete hit_sprite;
}
