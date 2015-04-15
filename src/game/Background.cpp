



#include "Background.hpp"


Background::Background(std::string bg_file, Sint16 x, Sint16 y, Uint16 w, Uint16 h, int sp)
{

    background = NULL;
    speed = 0;

    background = LX_Graphics::getInstance()->loadTextureFromFile(bg_file.c_str());

    if(background == NULL)
    {
        //throw Background_exception("image loading failed");
    }

    pos = {x,y,w,h};

    speed = sp;
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
