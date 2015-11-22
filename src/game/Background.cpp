

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2015  Luxon Jean-Pierre
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
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <LunatiX/LX_Graphics.hpp>

#include "Background.hpp"


Background::Background(std::string bg_file, Sint16 x, Sint16 y, Uint16 w, Uint16 h, int sp)
{
    background = LX_Graphics::loadTextureFromFile(bg_file.c_str());
    pos = {x,y,w,h};
    speed = sp;
}


SDL_Texture * Background::getBackground()
{
    return background;
}


Sint16 Background::getX_scroll()
{
    return pos.x;
}


Sint16 Background::getY_scroll()
{
    return pos.y;
}


Uint16 Background::getW()
{
    return pos.w;
}


Uint16 Background::getH()
{
    return pos.h;
}


int Background::getSpeed()
{
    return speed;
}


SDL_Rect * Background::getPos()
{
    return &pos;
}


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

