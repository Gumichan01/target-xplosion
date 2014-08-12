#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED



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
*	@file Entity.h
*	@brief The Entity library
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.1
*	@date July 23th, 2014
*
*
*
*/


#include <cstdio>
#include <string>

#include<SDL/SDL.h>
#include<SDL/SDL_mixer.h>

#include "../engine/LX_Hitbox.h"

#include <cmath>

/**
* DOC
*
*
*/
typedef struct{

    int speed_X;
    int speed_Y;

}Speed;


/**
* DOC
*
*
*/
class Entity{

    protected :

    //unsigned long id;  /**< The entity id*/ // 0x[BASE_NAME][SUB_BASE][KIND]
    SDL_Surface *graphic;
    Mix_Chunk *sound;
    SDL_Rect position;
    bool still_alive;
    Speed speed;


    public:

/**
* DOC
*
*
*/
    Entity(SDL_Surface *image, Mix_Chunk *audio,int x, int y, int w, int h,int dX, int dY)
    {
        graphic = image;    // assign the image
        sound = audio;      // assign the sound

        // assigne all features
        position.x = x;
        position.y = y;
        position.w = w;
        position.h = h;

        still_alive = true;

        speed.speed_X = dX;
        speed.speed_Y = dY;

    }

/**
* DOC
*
*/
    Entity(SDL_Surface *image, Mix_Chunk *audio,SDL_Rect *rect,Speed *sp)
    {
        graphic = image;    // assign the image
        sound = audio;      // assign the sound

        // assigne all features
        position.x = rect->x;
        position.y = rect->y;
        position.w = rect->w;
        position.h = rect->h;

        still_alive = true;

        speed.speed_X = sp->speed_X;
        speed.speed_Y = sp->speed_Y;

    }

    virtual void move() = 0 ;

    void die();
    bool isDead();

    //Setters

    void setX(int newX);
    void setY(int newY);
    void set_Xvel(int Xvel);
    void set_Yvel(int Yvel);


    //Getters

    SDL_Surface * getSurface();
    SDL_Rect * getPos();

    int get_Xvel(){return speed.speed_X;}
    int get_Yvel(){return speed.speed_Y;}

    int getX(){return position.x;}
    int getY(){return position.y;}

    int getWidth(){return position.w;}
    int getHeight(){return position.h;}


    virtual ~Entity()
    {
        Mix_FreeChunk(sound);
        SDL_FreeSurface(graphic);
    }

};





#endif // ENTITY_H_INCLUDED











