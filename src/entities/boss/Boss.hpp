
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
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#ifndef BOSS_HPP_INCLUDED
#define BOSS_HPP_INCLUDED

#include "../Enemy.hpp"
#include "../../pattern/Strategy.hpp"

class HUD;
class EnemyHUD;


namespace lx
{

namespace Graphics
{
class Sprite;
}

namespace Mixer
{
class Sound;
}

}

// Boss is the mother class of all bosses defined in the game
// A boss is an enemy
class Boss : public Enemy
{

    Boss( const Boss& ) = delete;
    Boss( const Boss&& ) = delete;
    Boss& operator =( const Boss& ) = delete;
    Boss& operator =( const Boss&& ) = delete;

protected:

    static const unsigned int DEFAULT_XPLOSION_DELAY = 3000;
    static const unsigned int DEFAULT_NOISE_DELAY = 250;
    const float XVEL_DIE = -2.56f;
    const float YVEL_DIE = 1.28f;

    int id_strat;
    unsigned int sprite_ref_time;       // Reference time of explosion
    bool hud_display;
    HUD * hud;

    bool mustCheckCollision() noexcept;

    template <typename It>
    void generateGenericBulletCircles( lx::Graphics::ImgRect& pos, lx::Graphics::Sprite * sp,
                                       It _beg, It _end, bool is_lunatic = false ) noexcept;

public:

    explicit Boss( unsigned int hp, unsigned int att, unsigned int sh,
                   lx::Graphics::Sprite * image, int x, int y, int w, int h,
                   float vx, float vy );

    virtual void draw() noexcept override;
    virtual void boom() noexcept override;
    virtual void strategy() noexcept override;
    virtual void collision( Missile * mi ) noexcept override;
    virtual void collision( Player * play ) noexcept override;
    virtual void reaction( Missile * target ) noexcept override;
    virtual void die() noexcept override;
    virtual ~Boss();
};

// Boss
class BossStrategy: virtual public Strategy
{

    BossStrategy( const BossStrategy& ) = delete;
    BossStrategy( const BossStrategy&& ) = delete;
    BossStrategy& operator =( const BossStrategy& ) = delete;
    BossStrategy& operator =( const BossStrategy&& ) = delete;

protected:

    Boss * boss;

public:

    explicit BossStrategy( Boss * newBoss );
    void proceed() noexcept override = 0;
    ~BossStrategy() = default;
};

#include "Boss.tpp"

#endif // BOSS_HPP_INCLUDED
