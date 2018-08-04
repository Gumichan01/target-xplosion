
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
*   along with this program. If not, see <http://www.gnu.org/licenses/>.lx::Graphics::ImgRect
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "Background.hpp"
#include "../resources/WinID.hpp"
#include "../asset/TX_Asset.hpp"

#include <Lunatix/Texture.hpp>
#include <Lunatix/WindowManager.hpp>
#include <Lunatix/Time.hpp>
#include <Lunatix/Hitbox.hpp>

namespace
{
const unsigned int SECOND = 500;
const Float DIV3{3.0f};
const Float MAX_SPEED = {24.0f};
}

using namespace FloatBox;
using lx::Physics::toFloatPosition;

Background::Background( unsigned int lvl, lx::Graphics::ImgRect& rect, int sp )
    : speed_fgd( fbox<int>( sp ) ), speed_mgd( fbox<int>( sp - ( sp / DIV3 ) ) ),
      speed_bgd( fbox<int>( sp ) ),
      area_fgd( rect ), area_mgd( rect ), area_bgd( rect ),
      pos_fgd( toFloatPosition( rect.p ) ), pos_mgd( toFloatPosition( rect.p ) ),
      pos_bgd( toFloatPosition( rect.p ) ),
      foreground( nullptr ), middleground( nullptr ), background( nullptr ),
      inc_speed( false ), is_parallax( false ), t( 0 )
{
    lx::Win::Window& win = lx::Win::getWindowManager().getWindow( WinID::getWinID() );
    const TX_Asset * const a = TX_Asset::getInstance();
    const TX_ParallaxAsset * const passet = a->getLevelParallax( lvl );

    if ( passet != nullptr )
    {
        is_parallax  = true;
        foreground   = new lx::Graphics::Sprite( passet->parallax01_bg, win );
        middleground = new lx::Graphics::Sprite( passet->parallax02_bg, win );
        background   = new lx::Graphics::Sprite( passet->parallax03_bg, win );
        speed_bgd   /= fbox( 3.0f );
    }
    else
        background   = new lx::Graphics::Sprite( a->getLevelBg( lvl ), win );
}

// Move the background
void Background::scroll()
{
    if ( inc_speed && ( -speed_fgd ) < MAX_SPEED )
        increaseSpeed();

    if ( is_parallax )
    {
        if ( pos_fgd.x <= -area_fgd.w )
            pos_fgd.x = FNIL;
        else
            pos_fgd.x += speed_fgd;

        if ( pos_mgd.x <= -area_mgd.w )
            pos_mgd.x = FNIL;
        else
            pos_mgd.x += speed_mgd;

        area_fgd.p = lx::Graphics::toPixelPosition( pos_fgd );
        area_mgd.p = lx::Graphics::toPixelPosition( pos_mgd );
    }

    if ( pos_bgd.x <= fbox<int>( -area_bgd.w ) )
        pos_bgd.x = FNIL;
    else
        pos_bgd.x += speed_bgd;

    area_bgd.p = lx::Graphics::toPixelPosition( pos_bgd );
}

void Background::draw()
{
    lx::Graphics::ImgRect area2 = area_bgd;
    area2.p.x += area2.w;
    background->draw( area_bgd );
    background->draw( area2 );

    if ( is_parallax )
    {
        lx::Graphics::ImgRect area4 = area_fgd;
        area4.p.x += area4.w;
        lx::Graphics::ImgRect area3 = area_mgd;
        area3.p.x += area3.w;

        middleground->draw( area_mgd );
        middleground->draw( area3 );
        foreground->draw( area_fgd );
        foreground->draw( area4 );
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
    if ( ( lx::Time::getTicks() - t ) > SECOND )
    {
        const Float ONE = fbox( 1.0f );
        speed_bgd -= ONE / DIV3;
        speed_mgd -= ONE - ( ONE / DIV3 );
        speed_fgd -= ONE;
        t = lx::Time::getTicks();
    }
}

Background::~Background()
{
    delete foreground;
    delete middleground;
    delete background;
}
