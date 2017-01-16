
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017  Luxon Jean-Pierre
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Character.hpp"

#define PLAYER_SPEED 12
#define PLAYER_WIDTH 64
#define PLAYER_HEIGHT 48


class PlayerHUD;
class Item;
class PlayerVisitor;
enum MISSILE_TYPE: short;
enum POWER_UP: short;

namespace LX_Physics
{
struct LX_Vector2D;
struct LX_Point;
struct LX_Circle;
};

namespace LX_FileIO
{
class LX_FileBuffer;
};

namespace LX_Mixer
{
class LX_Sound;
};

namespace LX_Graphics
{
class LX_Sprite;
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
    uint32_t shield_time;                       // Time of begining of shield
    unsigned int nb_hits;                       // Maximum number of hits under shield
    unsigned int nb_died;
    uint32_t laser_begin;
    uint32_t laser_delay;

    const int LIMIT_WIDTH;
    const int LIMIT_HEIGHT;

    LX_Mixer::LX_Sound * basic_shot;
    LX_Mixer::LX_Sound * rocket_shot;
    LX_Mixer::LX_Sound * laser_shot;

    PlayerHUD *display;

    void initData();
    void initHitboxRadius();
    void rocketShot();
    void bombShot();
    void laserShot();
    void specialShot(const MISSILE_TYPE& type);

    void heal();
    void rocket();
    void laser();
    void bomb();

public :

    Player(unsigned int hp, unsigned int att, unsigned int sh,
           unsigned int critic, LX_Graphics::LX_Sprite *image,
           LX_Mixer::LX_Sound *audio, LX_AABB& rect,
           LX_Physics::LX_Vector2D& sp,
           int w_limit, int h_limit);

    void setHUD(PlayerHUD *h);
    void fire(const MISSILE_TYPE& m_type);
    void takeBonus(const POWER_UP& powerUp);

    virtual void move();
    virtual void die();
    void reborn();
    virtual void receiveDamages(unsigned int attacks);
    void updateHUD();

    void setShield(bool sh);
    virtual void collision(Missile *mi);
    virtual void collision(Item *item);

    unsigned int getBomb() const;
    unsigned int getRocket() const;
    unsigned int nb_death() const;

    static void accept(PlayerVisitor *pv);

    const LX_Physics::LX_Circle * getHitbox();
    bool isLaserActivated() const;

    ~Player();
};

#endif // PLAYER_H_INCLUDED
