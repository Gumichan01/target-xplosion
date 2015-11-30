
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
*	@file EnemyData.cpp
*	@brief The definition of constructors
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "EnemyData.hpp"

EnemyData::EnemyData() {}

EnemyData::EnemyData(const EnemyData& data)
    : type(data.type), hp(data.hp), att(data.att), sh(data.sh), time(data.time),
      y(data.y), w(data.w),h(data.h)
{
    // Empty
}

EnemyData& EnemyData::operator =(const EnemyData data)
{
    type = data.type;
    hp = data.hp;
    att = data.att;
    sh = data.sh;
    time = data.time;
    y = data.y;
    w = data.w;
    h = data.h;

    return *this;
}

