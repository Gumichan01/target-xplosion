
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

#include "Boss.hpp"
#include "../../game/engine/Hud.hpp"
#include "../../game/engine/Engine.hpp"
#include "../../game/Scoring.hpp"
#include "../Missile.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Chunk.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Log.hpp>

using namespace LX_Physics;

Boss::Boss(unsigned int hp, unsigned int att, unsigned int sh,
           LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Chunk *audio,
           int x, int y, int w, int h, float vx, float vy)
    : Enemy(hp, att, sh, image, audio, x, y, w, h, vx, vy),
      id_strat(0), dying(false), sprite_ref_time(0), hud_display(false),
      hud(new BossHUD(*this)) {}


MoveAndShootStrategy * Boss::getMVSStrat()
{
    MoveAndShootStrategy *mvs = dynamic_cast<MoveAndShootStrategy*>(strat);

    if(mvs == nullptr)
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "RTTI — Cannot cast the current strategy");
    return mvs;
}


void Boss::strategy()
{
    if(!hud_display)
    {
        Engine::getInstance()->acceptHUD(hud);
        hud_display = true;
    }

    Enemy::strategy();
}

bool Boss::mustCheckCollision()
{
    return !dying;
}

void Boss::reaction(Missile *target)
{
    if(!dying)
        Enemy::reaction(target);

    hud->update();
}

// It is time to die
void Boss::die()
{
    Engine *g = Engine::getInstance();

    if((position.x + position.w) < 0)
        Entity::die();
    else
    {
        // The boss is dying
        if(!dying)
        {
            // The boss will die
            id_strat = -1;
            dying = true;
            g->screenCancel();
            speed = LX_Vector2D(XVEL_DIE, YVEL_DIE);
            sprite_ref_time = LX_Timer::getTicks();
            boom();
        }
        else
        {
            // It is dead
            // Give points to the player
            Entity::die();
            g->getScore()->notify(static_cast<int>(max_health_point)*2);
            Engine::getInstance()->acceptHUD(hud);
        }
    }
}

// Play the sound of explosion
void Boss::boom()
{
    if(sound != nullptr)
        sound->play();
}

Boss::~Boss()
{
    delete hud;
    hud = nullptr;
}

// Boss strategy
BossStrategy::BossStrategy(Boss *newBoss)
    : Strategy(newBoss), boss(newBoss), started(false) {}

BossStrategy::~BossStrategy() {}
