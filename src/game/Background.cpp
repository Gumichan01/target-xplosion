
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
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.LX_Graphics::LX_ImgRect
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "Background.hpp"
#include "../resources/WinID.hpp"
#include "../asset/TX_Asset.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Hitbox.hpp>

namespace
{
const unsigned int SECOND = 500;
const Float DIV3{3.0f};
const Float MAX_SPEED = {24.0f};
}

using namespace FloatBox;
using LX_Physics::toFloatPosition;

Background::Background(unsigned int lvl, LX_Graphics::LX_ImgRect& rect, int sp)
    : speed_fgd(fbox(sp)), speed_mgd(fbox(sp - (sp / DIV3))),
      speed_bgd(fbox(sp)),
      area_fgd(rect), area_mgd(rect), area_bgd(rect),
      pos_fgd(toFloatPosition(rect.p)), pos_mgd(toFloatPosition(rect.p)),
      pos_bgd(toFloatPosition(rect.p)),
      foreground(nullptr), middleground(nullptr), background(nullptr),
      inc_speed(false), is_parallax(false), t(0)
{
    LX_Win::LX_Window& win = LX_Win::getWindowManager().getWindow(WinID::getWinID());
    const TX_Asset * const a = TX_Asset::getInstance();
    const TX_ParallaxAsset * const passet = a->getLevelParallax(lvl);

    if(passet != nullptr)
    {
        is_parallax  = true;
        foreground   = new LX_Graphics::LX_Sprite(passet->parallax01_bg, win);
        middleground = new LX_Graphics::LX_Sprite(passet->parallax02_bg, win);
        background   = new LX_Graphics::LX_Sprite(passet->parallax03_bg, win);
        speed_bgd   /= fbox(3.0f);
    }
    else
        background   = new LX_Graphics::LX_Sprite(a->getLevelBg(lvl), win);
}

// Move the background
void Background::scroll()
{
    const Float ZERO{0.0f};

    if(inc_speed && (-speed_fgd) < MAX_SPEED)
        increaseSpeed();

    if(is_parallax)
    {
        if(pos_fgd.x <= -area_fgd.w)
            pos_fgd.x = ZERO;
        else
            pos_fgd.x += speed_fgd;

        if(pos_mgd.x <= -area_mgd.w)
            pos_mgd.x = ZERO;
        else
            pos_mgd.x += speed_mgd;

        area_fgd.p = LX_Graphics::toPixelPosition(pos_fgd);
        area_mgd.p = LX_Graphics::toPixelPosition(pos_mgd);
        //pos_fgd.toPixelUnit(area_fgd);
        //pos_mgd.toPixelUnit(area_mgd);
    }

    if(pos_bgd.x <= fbox(-area_bgd.w) )
        pos_bgd.x = ZERO;
    else
        pos_bgd.x += speed_bgd;

    area_bgd.p = LX_Graphics::toPixelPosition(pos_bgd);
    //pos_bgd.toPixelUnit(area_bgd);
}

void Background::draw()
{
    LX_Graphics::LX_ImgRect area2 = area_bgd;
    area2.p.x += area2.w;
    background->draw(area_bgd);
    background->draw(area2);

    if(is_parallax)
    {
        LX_Graphics::LX_ImgRect area4 = area_fgd;
        area4.p.x += area4.w;
        LX_Graphics::LX_ImgRect area3 = area_mgd;
        area3.p.x += area3.w;

        middleground->draw(area_mgd);
        middleground->draw(area3);
        foreground->draw(area_fgd);
        foreground->draw(area4);
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
        const Float ONE{1.0f};
        speed_bgd -= ONE / DIV3;
        speed_mgd -= ONE - (ONE / DIV3);
        speed_fgd -= ONE;
        t = LX_Timer::getTicks();
    }
}

Background::~Background()
{
    delete foreground;
    delete middleground;
    delete background;
}
