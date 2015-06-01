



#include "Background.hpp"


Background::Background(std::string bg_file, Sint16 x, Sint16 y, Uint16 w, Uint16 h, int sp)
{

    background = NULL;
    speed = 0;

    background = LX_Graphics::getInstance()->loadTextureFromFile(bg_file.c_str());

    if(background == NULL)
    {
        throw Background_exception(SDL_GetError());
    }

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
