

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


#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED

#include <LunatiX/LX_Graphics.hpp>


/**
*	@file Background.hpp
*	@brief The background class. It handles the background image
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/


class Background_exception : public std::exception
{
    public :

    std::string str_err;

    Background_exception(std::string err)
    {
        str_err = err;
    }

    const char * what() const throw() {return str_err.c_str() ;}

    ~Background_exception() throw(){}
};



class Background{

    SDL_Texture * background;   // the image
    SDL_Rect pos;               // the position and dimension of the background
    int speed;                  // the scrolling speed


    public:

    Background(std::string bg_file, Sint16 x, Sint16 y, Uint16 w, Uint16 h, int sp);

    SDL_Texture * getBackground();

    SDL_Rect * getPos();

    Sint16 getX_scroll();
    Sint16 getY_scroll();

    Uint16 getW();
    Uint16 getH();

    int getSpeed();

    void scroll(void);

    ~Background();

};


#endif // BACKGROUND_H_INCLUDED


