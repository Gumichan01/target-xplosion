
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

#include "Background.hpp"
#include "../resources/WinID.hpp"
#include "../asset/TX_Asset.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Timer.hpp>

namespace
{
const uint32_t SECOND = 500;
const float DIV3 = 3.0f;
const float MAX_SPEED = 24.0f;
const float MAX_SPEED2 = MAX_SPEED - (MAX_SPEED/DIV3);
const float MAX_SPEED3 = MAX_SPEED/DIV3;
const FloatPosition fpos(0.0f, 0.0f);
}

Background::Background(unsigned int lvl, LX_AABB& rect, int sp)
    : speed_fgd(sp), speed_mgd(sp - (sp/DIV3)), speed_bgd(sp),
    area_fgd(rect), area_mgd(rect), area_bgd(rect),
    pos_fgd(rect.x, rect.y), pos_mgd(rect.x, rect.y), pos_bgd(rect.x, rect.y),
    foreground(nullptr), middleground(nullptr), background(nullptr),
    inc_speed(false), is_parallax(false), t(0)
{
    LX_Win::LX_Window *win = LX_Win::getWindowManager()->getWindow(WinID::getWinID());
    const TX_Asset *a = TX_Asset::getInstance();
    const TX_ParallaxAsset *passet = a->getLevelParallax(lvl);

    if(passet != nullptr)
    {
        is_parallax  = true;
        foreground   = new LX_Graphics::LX_Sprite(passet->parallax01_bg, *win);
        middleground = new LX_Graphics::LX_Sprite(passet->parallax02_bg, *win);
        background   = new LX_Graphics::LX_Sprite(passet->parallax03_bg, *win);
        speed_bgd   /= 3;
    }
    else
        background   = new LX_Graphics::LX_Sprite(a->getLevelBg(lvl), *win);
}

// Move the background
void Background::scroll()
{
    if(inc_speed && (-speed_fgd) < MAX_SPEED)
        increaseSpeed();

    if(is_parallax)
    {
        if(pos_fgd.x <= -area_fgd.w)
            pos_fgd.x = 0.0f;
        else
            pos_fgd.x += speed_fgd;

        if(pos_mgd.x <= -area_mgd.w)
            pos_mgd.x = 0.0f;
        else
            pos_mgd.x += speed_mgd;

        pos_fgd.toPixelUnit(area_fgd);
        pos_mgd.toPixelUnit(area_mgd);
    }

    if(pos_bgd.x <= -area_bgd.w)
        pos_bgd.x = 0.0f;
    else
        pos_bgd.x += speed_bgd;

    pos_bgd.toPixelUnit(area_bgd);
}

void Background::draw()
{
    LX_AABB area2 = area_bgd;
    area2.x += area2.w;
    background->draw(&area_bgd);
    background->draw(&area2);

    if(is_parallax)
    {
        LX_AABB area4 = area_fgd;
        area4.x += area4.w;
        LX_AABB area3 = area_mgd;
        area3.x += area3.w;

        middleground->draw(&area_mgd);
        middleground->draw(&area3);
        foreground->draw(&area_fgd);
        foreground->draw(&area4);
    }
}


void Background::update()
{
    scroll();
    draw();
}

void Background::setIncrease()
{
    inc_speed = true;
}

void Background::increaseSpeed()
{
    if((LX_Timer::getTicks() - t) > SECOND)
    {
        speed_bgd -= 1.0f/DIV3;
        speed_mgd -= 1.0f - (1.0f / DIV3);
        speed_fgd -= 1.0f;
        t = LX_Timer::getTicks();
    }
}

Background::~Background()
{
    delete foreground;
    delete middleground;
    delete background;
}
