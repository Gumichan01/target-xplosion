
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
#include "Item.hpp"

class PlayerHUD;
class Item;
class PlayerVisitor;
enum class MissileType;

namespace LX_Physics
{
struct LX_Vector2D;
struct LX_Point;
struct LX_Circle;
}

namespace LX_FileIO
{
class LX_FileBuffer;
}

namespace LX_Graphics
{
class LX_Sprite;
class LX_AnimatedSprite;
}


/**
*   @class Player
*   @brief The player character
*
*   This character gathers all informations about the player
*/
class Player: public Character
{
    static LX_Physics::LX_Point last_position;

    // Game limits (width/height)
    const int GAME_WLIM;
    const int GAME_HLIM;

    // Skills
    unsigned int critical_rate;
    unsigned int nb_bomb;
    unsigned int nb_rocket;

    // Shield
    bool has_shield;
    uint32_t shield_t;                          // Time of the shield (beginning)
    unsigned int hit_count;                     // Hit counter under shield
    unsigned int deaths;

    // Laser weapon
    bool laser_activated;
    uint32_t laser_begin;
    uint32_t laser_delay;
    uint32_t invincibility_t;

    // Slow mode
    bool slow_mode;

    PlayerHUD *display;
    LX_Graphics::LX_Sprite *sprite_hitbox;
    LX_Graphics::LX_AnimatedSprite *sprite_explosion;

    void initHitboxRadius();
    void laserShot();

    void heal();
    void rocket();
    void laser();
    void bomb();

    void checkLaserShot();
    void reborn();

public:

    static constexpr int PLAYER_WIDTH = 64;
    static constexpr int PLAYER_HEIGHT = 48;
    static constexpr float PLAYER_SPEED = 12.0f;
    static constexpr float PLAYER_SPEED_RATIO = 1.80f;

    Player(unsigned int hp, unsigned int att, unsigned int sh,
           unsigned int critic, LX_Graphics::LX_Sprite *image, LX_AABB& rect,
           LX_Physics::LX_Vector2D& sp, int w_limit, int h_limit);

    virtual void receiveDamages(unsigned int attacks);

    void normalShot();
    void rocketShot();
    void bombShot();
    void takeBonus(ItemType powerUp);

    virtual void boom();
    virtual void move();
    virtual void draw();
    virtual void die();

    void status() noexcept;

    void setShield(bool sh);
    void notifySlow(bool slow);
    virtual void collision(Missile *mi);
    virtual void collision(Item *item);

    unsigned int getBomb() const;
    unsigned int getRocket() const;
    unsigned int nb_death() const;

    static void accept(PlayerVisitor *pv);
    bool isLaserActivated() const;

    ~Player();
};

#endif // PLAYER_H_INCLUDED
