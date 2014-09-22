#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED



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
*	@file Item.h
*	@brief The Item class
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.2
*	@date September 21st, 2014
*
*
*/

#include "Entity.h"
#include "../game/random.h"


class LX_graphics;

typedef enum POWER_UP {NO_POWER_UP=0,HEALTH_QUARTER=10,HEALTH_HALF=200,SHIELD=30,ROCKET_BONUS=40,BOMB_BONUS=50,LASER_BONUS=60,MEGA_MISSILE_BONUS=70} POWER_UP;

#define POWER_UP_NUMBER 7

// Item position
#define XPOS 1600   // X absolute position
#define YPOS 600    // Y relative position

// Item dimension
#define ITEM_W 47
#define ITEM_H 47

// X velocity
#define XVEL -1


class Item: public Entity
{

    POWER_UP bonus;
    LX_AABB aabb;

    public :

    Item();

    POWER_UP getPowerUp();
    void move();
    LX_AABB * box();

    ~Item(){}

};











#endif // ITEM_H_INCLUDED










