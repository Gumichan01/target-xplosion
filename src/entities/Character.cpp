
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


namespace
{

inline constexpr unsigned int MIN(const Float& a, const Float& b)
{
    return static_cast<unsigned int>(a.v < b.v ? a.v : b.v);
}

}

using namespace FloatBox;


Character::Character(unsigned int hp, unsigned int att, unsigned int sh,
                     LX_Graphics::LX_Sprite *image, const LX_Graphics::LX_ImgRect& rect,
                     const LX_Physics::LX_Vector2D& sp)
    : Entity(image, rect, sp), circle_box(), health_point(hp), max_health_point(hp),
      attack_val(att), shield(sh), was_killed(false), dying(false),
      hit_sprite(nullptr), hit_time(0), hit(false)
{
    characterInit();
    createHitSprite();
}


void Character::characterInit()
{
    using LX_Physics::LX_Circle;
    using LX_Physics::LX_FloatPosition;

    const Float XCENTER = phybox.p.x + fbox(imgbox.w / 2);
    const Float YCENTER = phybox.p.y + fbox(imgbox.h / 2);
    const unsigned int RAD = MIN((XCENTER - phybox.p.x), (YCENTER - phybox.p.y));
    circle_box = LX_Circle{LX_FloatPosition{XCENTER, YCENTER}, RAD};
}


void Character::createHitSprite()
{
    using LX_Graphics::LX_BufferedImage;

    const LX_Graphics::LX_ImgRect RNULL{{0,0},0,0};
    const TX_Asset * const ASSET = TX_Asset::getInstance();
    const unsigned int FILE_ID = ASSET->getID(graphic->getFileName());
    const TX_Anima * const ANIMA = ASSET->getEnemyAnimation(FILE_ID);
    const LX_Graphics::LX_ImgRect& r = (ANIMA == nullptr) ? RNULL : ANIMA->v[0];

    LX_Win::LX_Window& w = LX_Win::getWindowManager().getWindow(WinID::getWinID());
    hit_sprite = LX_BufferedImage(graphic->getFileName()).convertNegative().
                 generateSprite(w, r);
}

void Character::destroyHitSprite() noexcept
{
    delete hit_sprite;
}


void Character::draw() noexcept
{
    if(hit && !dying)
    {
        if((LX_Timer::getTicks() - hit_time) > HIT_DELAY)
        {
            hit = false;
            hit_time = LX_Timer::getTicks();
        }

        imgbox.p = LX_Graphics::toPixelPosition(phybox.p);
        hit_sprite->draw(imgbox);
    }
    else
        Entity::draw();
}


void Character::receiveDamages(unsigned int attacks) noexcept
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

const LX_Physics::LX_Circle& Character::getHitbox() const noexcept
{
    return circle_box;
}

void Character::kill() noexcept
{
    was_killed = true;
    unsigned long sc = max_health_point + attack_val + shield;
    Engine::getInstance()->getScore()->notify(sc, true);
    die();
}


bool Character::isDying() const noexcept
{
    return dying;
}


unsigned int Character::getHP() const noexcept
{
    return health_point;
}

unsigned int Character::getMaxHP() const noexcept
{
    return max_health_point;
}

unsigned int Character::getATT() const noexcept
{
    return attack_val;
}

unsigned int Character::getDEF() const noexcept
{
    return shield;
}

void Character::setX(float nx) noexcept
{
    Entity::setX(nx);
    circle_box.center.x = fbox(nx + static_cast<float>(imgbox.w) / 2.0f);
}

void Character::setY(float ny) noexcept
{
    Entity::setY(ny);
    circle_box.center.y = fbox(ny + static_cast<float>(imgbox.h) / 2.0f);
}

Character::~Character()
{
    delete hit_sprite;
}
