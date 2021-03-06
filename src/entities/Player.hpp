
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
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

namespace lx
{

namespace Physics
{
struct Vector2D;
struct Point;
struct Circle;
}

namespace FileIO
{
class FileBuffer;
}

namespace Graphics
{
class Sprite;
class AnimatedSprite;
}

}

namespace GPconfig
{
class GamepadHandler;
}


/**
*   @class Player final
*   @brief The player character
*
*   This character gathers all informations about the player
*/
class Player final: public Character
{
    static lx::Physics::FloatPosition last_position;

    // Game limits (width/height)
    const Float GAME_WLIM;
    const Float GAME_HLIM;

    // Skills
    unsigned int critical_rate;
    unsigned int nb_bomb;
    unsigned int nb_rocket;

    bool has_shield;
    bool laser_activated;
    unsigned int hit_count;     // Hit counter under shield
    unsigned int deaths;
    bool slow_mode;

    lx::Time::Timer ptimer;
    lx::Time::Timer shtimer;
    lx::Time::Timer latimer;
    lx::Time::Timer invtimer;
    lx::Time::Timer extimer;
    lx::Time::Timer dhtimer;

    PlayerHUD * display;
    lx::Graphics::Sprite * sprite_hitbox;
    lx::Graphics::AnimatedSprite * sprite_explosion;
    GPconfig::GamepadHandler& gamepadhdl;

    void initHitboxRadius() noexcept;
    void laserShot() noexcept;

    void heal() noexcept;
    void rocket() noexcept;
    void laser() noexcept;
    void bomb() noexcept;

    void checkLaserShot() noexcept;
    void updateStatus( unsigned int prev_health ) noexcept;
    void feedback( unsigned int prev_health ) noexcept;
    void vibrate( float strength, uint32_t length ) noexcept;
    void reborn() noexcept;

    Player( const Player& ) = delete;
    Player( const Player&& ) = delete;
    Player& operator =( const Player& ) = delete;
    Player& operator =( const Player&& ) = delete;

public:

    static constexpr int PLAYER_WIDTH = 64;
    static constexpr int PLAYER_HEIGHT = 48;
    static constexpr float PLAYER_SPEED = 12.0f;
    static constexpr float PLAYER_SPEED_RATIO = 1.80f;

    Player( unsigned int hp, unsigned int att, unsigned int sh,
            unsigned int critic, lx::Graphics::Sprite * image, lx::Graphics::ImgRect& rect,
            lx::Physics::Vector2D& sp, GPconfig::GamepadHandler& ghdl );

    virtual void receiveDamages( unsigned int attacks ) noexcept override;

    void normalShot() noexcept;
    void rocketShot() noexcept;
    void bombShot() noexcept;
    void takeBonus( ItemType powerUp ) noexcept;

    void boom() noexcept;
    virtual void move() noexcept override;
    virtual void draw() noexcept override;
    virtual void die() noexcept override;

    void status() noexcept;

    void setShield( bool sh ) noexcept;
    void notifySlow( bool slow ) noexcept;
    using Character::collision;
    virtual void collision( Missile * mi ) noexcept override;
    void collision( Item * item ) noexcept;

    unsigned int getBomb() const noexcept;
    unsigned int getRocket() const noexcept;
    unsigned int nb_death() const noexcept;

    static void accept( PlayerVisitor& pv ) noexcept;
    bool isLaserActivated() const noexcept;

    ~Player();
};

#endif // PLAYER_H_INCLUDED
