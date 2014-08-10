#ifndef TX_GRAPHICS_H_INCLUDED
#define TX_GRAPHICS_H_INCLUDED



/*
*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*
*/

/**
*	@file TX_graphics.h
*	@brief The TX_graphics library, it will be linked with the SDL library
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 1.0
*	@date July 13th, 2014
*
*
*/
#include<iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "TX_error.h"


/**
*   @class TX_graphics
*   @brief The Target_Xplosion graphic engine.
*
*   This class describes the TX_graphics engine, a graphic engine used in Target Xplosion.
*/
class TX_graphics{

    SDL_Surface *screen;    /**< The main surface (for the window creation)*/
    int TX_width, TX_height, TX_bpp;

    public:

/**
*
*   @fn TX_graphics(unsigned int width, unsigned int height, unsigned int bpp)
*
*   This constructor initializes The SDL subsystems and create the window of the game
*
*   @param width : The width of the window
*   @param height : the height of the window
*   @param bpp : bits per pixels
*
*/
    TX_graphics(unsigned int width, unsigned int height, unsigned int bpp)
    {
        TX_width = width;
        TX_height = height;
        TX_bpp = bpp;

        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER) == -1)
        {
            fprintf(stderr,"\nException occured in TX_graphics constructor, SDL_Init : %s \n", SDL_GetError());
            throw TX_SDL_INIT_ERROR;
        }


        int flag= IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF ;

        if( IMG_Init(flag) != flag)
        {
            fprintf(stderr,"\nException occured in TX_graphics constructor, IMG_Init : %s \n", IMG_GetError());
            SDL_Quit();
            throw TX_IMG_INIT_ERROR;
        }

        //screen=SDL_SetVideoMode(width,height,bpp,SDL_HWSURFACE|SDL_DOUBLEBUF/*|SDL_FULLSCREEN*/);
        screen=SDL_SetVideoMode(width,height,bpp,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);

        if(screen == NULL)
        {
            fprintf(stderr,"\nException occured in TX_graphics constructor, SDL_SetVideoMode : %s \n", SDL_GetError());
            IMG_Quit();
            SDL_Quit();
            throw TX_SCREEN_ERROR;
        }

    SDL_WM_SetCaption("Target Xplosion prototype (v 0.1)", NULL);

    }

    // The static functions

    static SDL_Surface * load_BMP(std::string filename);
    static SDL_Surface * load_image(std::string filename);

    static bool put_transparency(SDL_Surface *image,unsigned int red, unsigned int green, unsigned int blue);

    // put the image on the screen
    bool put_image(SDL_Surface *image, SDL_Rect *area, SDL_Rect *pos);

    // the update and clearing functions
    void update();

    void clear();

    //the getter, for the TX_TTF class
    SDL_Surface * getScreen();

    int getWidth(){return TX_width;}
    int getHeight(){return TX_height;}
    int getBPP(){return TX_bpp;}

    ~TX_graphics(){

        IMG_Quit();
        SDL_Quit();
    }

};









#endif // TX_GRAPHICS_H_INCLUDED












