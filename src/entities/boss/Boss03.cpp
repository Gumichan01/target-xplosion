
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


#include "Boss03.hpp"

#include "../Player.hpp"
#include "../../pattern/Strategy.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Texture.hpp>

namespace
{
unsigned int BOSS03_HEAD_ID = 10;

// Position of the HEAD
int BOSS03_HEAD_XOFF = 318;
int BOSS03_HEAD_YOFF = 158;
int BOSS03_HEAD_W = 461;
int BOSS03_HEAD_H = 336;

}

// Boss03

Boss03::Boss03(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy), index(0)
{
    /// @todo Boss03 — constructor
    boss_parts[0] = new Boss03Body(hp, att, sh, image, x, y, w, h, vx, vy);
    /*boss_parts[1] = new Boss03Head(hp/2, att, sh, image, x + BOSS03_HEAD_XOFF,
                                   y + BOSS03_HEAD_YOFF, BOSS03_HEAD_W, BOSS03_HEAD_H, vx, vy);*/

    // We don't care about were it is.
    // The only thing that matters is where are the parts
    fpos = FloatPosition(0.0f,0.0f);
    position = {0,0,0,0};
    //body_sprite = graphic;
    //head_sprite = ResourceManager::getInstance()->getResource(RC_ENEMY, BOSS03_HEAD_ID);
}


void Boss03::draw()
{
    for(int i = 0; i < BOSS03_PARTS; ++i)
        boss_parts[i]->draw();
}


void Boss03::strategy()
{
    /// @todo Boss03 — strategy()
    boss_parts[index]->strategy();
}


void Boss03::move()
{
    /// @todo Boss03 — move()
    for(int i = 0; i < BOSS03_PARTS; ++i)
        boss_parts[i]->move();
}

void Boss03::collision(Missile *mi)
{
    /// @todo Boss03 — collision(missile)
    boss_parts[index]->collision(mi);
}

void Boss03::collision(Player *play)
{
    /// @todo Boss03 — collision(player)
    boss_parts[index]->collision(play);
}

void Boss03::die()
{
    /// @todo Boss03 — die()
    still_alive = false;
    //Enemy::die();
}

Boss03::~Boss03()
{
    for(int i = 0; i < BOSS03_PARTS; ++i)
        delete boss_parts[i];
}

// Boss03 Body

Boss03Body::Boss03Body(unsigned int hp, unsigned int att, unsigned int sh,
                       LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                       float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy)
{
    /// @todo Boss03Body — constructor
    addStrategy(new MoveStrategy(this));

    //body_sprite = graphic;
    //head_sprite = ResourceManager::getInstance()->getResource(RC_ENEMY, BOSS03_HEAD_ID);
}


void Boss03Body::fire()
{
    /// @todo Boss03Body — fire()
}

void Boss03Body::strategy()
{
    /// @todo Boss03Body — strategy()
    Enemy::strategy();
}


void Boss03Body::move()
{
    /// @todo Boss03Body — move()
    Enemy::move();
}

void Boss03Body::collision(Missile *mi)
{
    /// @todo Boss03Body — collision(missile)
}

void Boss03Body::collision(Player *play)
{
    /// @todo Boss03Body — collision(player)
}

void Boss03Body::die()
{
    /// @todo Boss03Body — die()
    Enemy::die();
}

