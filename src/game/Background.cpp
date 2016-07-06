

/*
*	Target_Xplosion - A classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Luxon Jean-Pierre (Gumichan01)
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <LunatiX/LX_Graphics.hpp>

#include "Background.hpp"


Background::Background(std::string bg_file, int x, int y,
                       int w, int h, int sp)
    : speed(sp),pos({x,y,w,h}),
background(LX_Graphics::loadTextureFromFile(bg_file.c_str()))
{
    // Empty
}

SDL_Texture * Background::getBackground() const
{
    return background;
}

// X speed of the scrolling
int Background::getX_scroll() const
{
    return pos.x;
}

// Y speed of the scrolling
int Background::getY_scroll() const
{
    return pos.y;
}

int Background::getW() const
{
    return pos.w;
}

int Background::getH() const
{
    return pos.h;
}

int Background::getSpeed() const
{
    return speed;
}

SDL_Rect * Background::getPos()
{
    return &pos;
}

// Move the background
void Background::scroll(void)
{
    if(pos.x <= -pos.w)
        pos.x =0;
    else
        pos.x += speed;
}

Background::~Background()
{
    SDL_DestroyTexture(background);
}

