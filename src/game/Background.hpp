

/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2014  Luxon Jean-Pierre
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

#include "../engine/LX_Graphics.hpp"


/**
*	@file background.h
*	@brief The background class. It handles the background image
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date September 5th, 2014
*
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

    SDL_Texture * getBackground(){return background;}

    SDL_Rect * getPos(){return &pos;}

    Sint16 getX_scroll(){return pos.x;}
    Sint16 getY_scroll(){return pos.y;}

    Uint16 getW(){return pos.w;}
    Uint16 getH(){return pos.h;}

    int getSpeed(){return speed;}

    void scroll(void);

    ~Background();

};









#endif // BACKGROUND_H_INCLUDED










