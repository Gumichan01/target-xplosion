

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


#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED


/**
*	@file Player.hpp
*	@brief The player class library
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include "Character.hpp"
#include "Missile.hpp"
#include "Item.hpp"

#define PLAYER_SPEED 12
#define PLAYER_WIDTH 60
#define PLAYER_HEIGHT 60
#define PLAYER_RADIUS 24

#define PLAYER_BULLET_W 24
#define PLAYER_BULLET_H 24

#define QUARTER 4
#define HALF 2

#define SHIELD_TIME 10000
#define HITS_UNDER_SHIELD 16

class HUD;
class Item;
class PlayerVistor;

namespace LX_Physics
{
struct LX_Vector2D;
struct LX_Point;
};

namespace LX_FileIO
{
class LX_FileBuffer;
};

namespace LX_Mixer
{
class LX_Sound;
};

/**
*   @class Player
*   @brief The player character
*
*   This character gathers all informations about the player
*/
class Player: public Character
{
    static LX_Physics::LX_Point last_position;

    unsigned int critical_rate;
    unsigned int nb_bomb;
    unsigned int nb_rocket;
    bool bomb_activated;
    bool rocket_activated;
    bool laser_activated;

    bool has_shield;
    double shield_time;             // Time of begining of shield
    int nb_hits;                    // Maximum number of hits under shield
    unsigned int nb_died;

    const int LIMIT_WIDTH;
    const int LIMIT_HEIGHT;

    LX_Mixer::LX_Sound * basic_shoot;
    LX_Mixer::LX_Sound * rocket_shoot;
    LX_Mixer::LX_Sound * laser_shoot;

    HUD *display;

    void initData(void);
    void initHitboxRadius(void);
    void basicShot(void);
    void rocketShot(void);
    void bombShot(void);
    void laserShot(void);
    void largeShot(void);
    void doubleShot(void);
    void specialShot(MISSILE_TYPE type);

    void heal(void);
    void rocket(void);
    void laser(void);
    void bomb(void);
    void bonus(void);

public :

    Player(unsigned int hp, unsigned int att, unsigned int sh,
           unsigned int critic, SDL_Texture *image,
           LX_Mixer::LX_Chunk *audio,SDL_Rect& rect,
           LX_Physics::LX_Vector2D& sp,
           int w_limit, int h_limit);

    void setHUD(HUD *h);
    void fire(MISSILE_TYPE m_type);
    void takeBonus(POWER_UP powerUp);

    void move();
    void die();
    void reborn();
    void receiveDamages(unsigned int attacks);
    void updateHUD();

    void setShield(bool sh);
    void collision(Missile *mi);

    unsigned int getBomb();
    unsigned int getRocket();
    unsigned int nb_death();

    static void accept(PlayerVisitor *pv);

    const LX_Physics::LX_Circle * getHitbox();
    bool isLaserActivated();

    ~Player();

};

#endif // PLAYER_H_INCLUDED

