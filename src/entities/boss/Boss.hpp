
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

#ifndef BOSS_HPP_INCLUDED
#define BOSS_HPP_INCLUDED

#include "../Enemy.hpp"
#include "../../pattern/Strategy.hpp"

class HUD;

namespace LX_Graphics
{
class LX_Sprite;
}

namespace LX_Mixer
{
class LX_Sound;
}


// Boss is the mother class of all bosses defined in the game
// A boss is an enemy
class Boss : public Enemy
{
protected:

    static const uint32_t DEFAULT_XPLOSION_DELAY = 3000;
    static const uint32_t DEFAULT_NOISE_DELAY = 250;
    const float XVEL_DIE = -1.0f;
    const float YVEL_DIE = 1.0f;

    int id_strat;
    uint32_t sprite_ref_time;       // Reference time of explosion
    bool hud_display;

    void die();

protected:

    HUD *hud;
    bool mustCheckCollision();

public:

    explicit Boss(unsigned int hp, unsigned int att, unsigned int sh,
                  LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                  float vx, float vy);

    virtual void boom();
    virtual void strategy();
    virtual void reaction(Missile *target);
    virtual ~Boss();
};

// Boss
class BossStrategy: virtual public Strategy
{
protected:

    Boss * boss;
    bool started;

public:

    explicit BossStrategy(Boss *newBoss);
    void proceed() = 0;
    ~BossStrategy();
};

const uint32_t BOSS_FIGHT_DELAY = 45000;

#endif // BOSS_HPP_INCLUDED
