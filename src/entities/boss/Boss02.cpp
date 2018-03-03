
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

#include "Boss02.hpp"
#include "../Item.hpp"
#include "../Rocket.hpp"
#include "../Player.hpp"
#include "../TreeMissile.hpp"
#include "../../game/engine/Hud.hpp"
#include "../../game/engine/Engine.hpp"
#include "../../game/engine/EntityHandler.hpp"
#include "../../game/engine/AudioHandler.hpp"
#include "../../resources/ResourceManager.hpp"

#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Random.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Log.hpp>


using namespace AudioHandler;
using namespace LX_Physics;

namespace
{
const int BOSS02_GLOBAL_XOFFSET = 48;
const int BOSS02_GLOBAL_YOFFSET = 8;
const int BOSS02_GLOBAL_BOXWIDTH = 448;
const int BOSS02_GLOBAL_BOXHEIGHT = 256;   // or 248

const int BOSS02_SHIELD_XOFFSET = 12;
const int BOSS02_SHIELD_YOFFSET = 8;
const int BOSS02_SHIELD_WIDTH = 48;
const int BOSS02_SHIELD_HEIGHT = 254;

const int BOSS02_SPRITE_SHID = 7;
const int BOSS02_SPRITE_DID = 5;
const float BOSS02_MSTRAT1_XVEL = -6.0f;
const float BOSS02_MSTRAT1_YVEL = 1.5f;
const int BOSS02_MSTRAT1_BULLET_ID = 6;

const LX_Graphics::LX_ImgCoord BOSS02_MSTRAT1_BULLET_POS[] =
{
    LX_Graphics::LX_ImgCoord{376, 137},
    LX_Graphics::LX_ImgCoord{342, 183},
    LX_Graphics::LX_ImgCoord{332, 105},
    LX_Graphics::LX_ImgCoord{294, 146}
};

int bindex = -1;
const unsigned int BOSS02_NB_SELECT = 4U;

const int BOSS02_MSTRAT1_BULLET_W = 16;
const int BOSS02_MSTRAT1_BULLET_H = 16;
const unsigned int BOSS02_MSTRAT1_BULLET_DELAY = 1000;

const unsigned int BOSS02_MSTRAT1_STOP_DELAY = 2000;
const float BOSS02_MSTRAT1_SPEED = 2.0f;

const int BOSS02_MSTRAT2_YUP = 100;
const int BOSS02_MSTRAT2_YDOWN = 500;

const int BOSS02_MSTRAT3_BULLET_ID = 1;
const unsigned int BOSS02_MSTRAT3_BULLET_DELAY = 500;
const int BOSS02_MSTRAT3_ROCKET_XOFF = 80;
const int BOSS02_MSTRAT3_ROCKET_YOFF = 186;
const int BOSS02_MSTRAT3_ROCKET_WIDTH = 48;
const int BOSS02_MSTRAT3_ROCKET_HEIGHT = 12;
const float BOSS02_MSTRAT3_SPEED = -3.5f;

const int BOSS02_MSTRAT4_BULLET_ID = 7;
const unsigned int BOSS02_MSTRAT4_BULLET_DELAY = 500;
const int BOSS02_MSTRAT4_BULLET_WIDTH = 28;
const int BOSS02_MSTRAT4_BULLET_HEIGHT = 28;
const int BOSS02_MSTRAT4_BULLET_XOFF = 174 - BOSS02_MSTRAT4_BULLET_WIDTH;
const int BOSS02_MSTRAT4_BULLET_YOFF = 19;
const float BOSS02_MSTRAT4_SPEED = -8.0f;
const float BOSS02_MSTRAT44_SPEED = 6.0f;

const unsigned int BOSS02_MSTRAT5_BULLET_DELAY = 100;
const float BOSS02_MSTRAT5_XVEL = -6.0f;
const float BOSS02_MSTRAT5_YVEL = 0.5f;

const unsigned int BOSS02_MAX_REFLECT_VALUE = 10000;
const unsigned int BOSS02_DELAY_NOISE = 500;

const std::vector<LX_FloatPosition> HPOINTS
{
    LX_FloatPosition{7,147}, LX_FloatPosition{243,67},
    LX_FloatPosition{174,47}, LX_FloatPosition{174,19},
    LX_FloatPosition{300,8}, LX_FloatPosition{380,8},
    LX_FloatPosition{405,64}, LX_FloatPosition{464,88},
    LX_FloatPosition{494,160}, LX_FloatPosition{464,218},
    LX_FloatPosition{432,248}, LX_FloatPosition{370,246},
    LX_FloatPosition{360,260}, LX_FloatPosition{282,260},
    LX_FloatPosition{248,220}, LX_FloatPosition{108,220},
    LX_FloatPosition{108,184}, LX_FloatPosition{238,184},
    LX_FloatPosition{216,162}
};

}


Boss02::Boss02(unsigned int hp, unsigned int att, unsigned int sh,
               LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
               float vx, float vy)
    : Boss(hp, att, sh, image, x, y, w, h, vx, vy),

      global_hitbox{LX_Physics::LX_FloatPosition{x + BOSS02_GLOBAL_XOFFSET, y + BOSS02_GLOBAL_YOFFSET},
                    BOSS02_GLOBAL_BOXWIDTH, BOSS02_GLOBAL_BOXHEIGHT},

      shield_hitbox{LX_Physics::LX_FloatPosition{x + BOSS02_SHIELD_XOFFSET, y + BOSS02_SHIELD_YOFFSET},
                    BOSS02_SHIELD_WIDTH, BOSS02_SHIELD_HEIGHT},

      shape(HPOINTS, LX_FloatPosition{x,y}), sh_sprite(nullptr), has_shield(false),
      shield_destroyed(false), b1time(0), rshield_life(BOSS02_MAX_REFLECT_VALUE)
{
    addStrategy(new MoveStrategy(this));
    bindex = LX_Random::xrand(0U, BOSS02_NB_SELECT);

    sprite = graphic;
    sh_sprite = ResourceManager::getInstance()->getResource(RC_ENEMY, BOSS02_SPRITE_SHID);
}

/// private functions

void Boss02::changeShotStrat(const unsigned int d) noexcept
{
    ShotStrategy *shot = new ShotStrategy(this);
    shot->setShotDelay(d);
    mvs->addShotStrat(shot);
}


// boss position in strategy #0
void Boss02::prepareTheAttack() noexcept
{
    const Float& XLIM = Engine::getMaxXlim();

    if(phybox.fpoint.x <= (XLIM - toFloat(phybox.w) ))
    {
        id_strat = 1;
        speed *= 0.0f;

        ShotStrategy *shot = new ShotStrategy(this);
        MoveStrategy *mv = new MoveStrategy(this);

        shot->setShotDelay(BOSS02_MSTRAT1_BULLET_DELAY);
        mvs->addShotStrat(shot);
        mvs->addMoveStrat(mv);
        addStrategy(mvs);
        b1time = LX_Timer::getTicks();
    }
}

// boss position in strategy #1
void Boss02::engage() noexcept
{
    if((LX_Timer::getTicks() - b1time) > BOSS02_MSTRAT1_STOP_DELAY)
    {
        mvs->addMoveStrat(new UpDownMoveStrategy(this, BOSS02_MSTRAT2_YUP,
                          BOSS02_MSTRAT2_YDOWN, BOSS02_MSTRAT1_SPEED));

        speed *= 0.0f;
        speed.vy = toFloat(BOSS02_MSTRAT1_SPEED);
        b1time = LX_Timer::getTicks();
        id_strat = 2;
    }
}

// boss position in strategy #2
void Boss02::meshAttack() noexcept
{
    const unsigned int HP_83PERCENT = static_cast<float>(max_health_point) * 0.83f;
    const unsigned int HP_34PERCENT = static_cast<float>(max_health_point) * 0.34f;

    if(health_point < HP_34PERCENT || (!has_shield && health_point < HP_83PERCENT))
    {
        id_strat = 3;
        changeShotStrat(BOSS02_MSTRAT3_BULLET_DELAY);
        EntityHandler::getInstance().bulletCancel();
    }

}

// boss position in strategy #3
void Boss02::targetAttack() noexcept
{
    const unsigned int HP_66PERCENT = static_cast<float>(max_health_point) * 0.66f;
    const unsigned int HP_16PERCENT = static_cast<float>(max_health_point) * 0.16f;

    if(health_point < HP_16PERCENT || (!has_shield && health_point < HP_66PERCENT))
    {
        id_strat = 4;
        changeShotStrat(BOSS02_MSTRAT4_BULLET_DELAY);
        EntityHandler::getInstance().bulletCancel();
    }

}

void Boss02::bulletAttack() noexcept
{
    const unsigned int HP_50PERCENT = static_cast<float>(max_health_point) * 0.50f;
    const unsigned int HP_10PERCENT = static_cast<float>(max_health_point) * 0.10f;

    if(health_point == 0)
    {
        die();
    }
    else if(health_point < HP_10PERCENT || (!has_shield && health_point < HP_50PERCENT))
    {
        if(health_point < HP_10PERCENT)
        {
            id_strat = 5;
            speed /= 2.0f;
            changeShotStrat(BOSS02_MSTRAT5_BULLET_DELAY);

        }
        else if(!has_shield && health_point < HP_50PERCENT)
        {
            id_strat = 2;
            has_shield = true;
            graphic = sh_sprite;
            changeShotStrat(BOSS02_MSTRAT1_BULLET_DELAY);
        }

        EntityHandler::getInstance().bulletCancel();
    }
}

/// Shot

void Boss02::mesh() noexcept
{
    float vx = (has_shield ? BOSS02_MSTRAT5_XVEL : BOSS02_MSTRAT1_XVEL);
    float vy = (has_shield ? BOSS02_MSTRAT5_YVEL : BOSS02_MSTRAT1_YVEL);
    LX_Vector2D v[] = {LX_Vector2D{vx, vy}, LX_Vector2D{vx, -vy}};

    LX_Graphics::LX_ImgRect b = {{position.p.x + BOSS02_MSTRAT1_BULLET_POS[bindex].x,
                                 position.p.y + BOSS02_MSTRAT1_BULLET_POS[bindex].y},
                                 BOSS02_MSTRAT1_BULLET_W, BOSS02_MSTRAT1_BULLET_H
                                };

    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *s = rc->getResource(RC_MISSILE, BOSS02_MSTRAT1_BULLET_ID);
    EntityHandler& hdl = EntityHandler::getInstance();

    hdl.pushEnemyMissile(*(new TreeMissile(attack_val, s, b, v[0])));
    hdl.pushEnemyMissile(*(new TreeMissile(attack_val, s, b, v[1])));
}

void Boss02::target() noexcept
{
    LX_Vector2D v{BOSS02_MSTRAT3_SPEED, 0.0f};
    LX_Graphics::LX_ImgRect b = {position.p.x + BOSS02_MSTRAT3_ROCKET_XOFF,
                                 position.p.y + BOSS02_MSTRAT3_ROCKET_YOFF,
                                 BOSS02_MSTRAT3_ROCKET_WIDTH, BOSS02_MSTRAT3_ROCKET_HEIGHT
                                };

    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *s = rc->getResource(RC_MISSILE, BOSS02_MSTRAT3_BULLET_ID);

    EntityHandler& hdl = EntityHandler::getInstance();
    hdl.pushEnemyMissile(*(new EnemyRocket(attack_val, s, b, v)));
}

void Boss02::danmaku() noexcept
{
    static int id = 0;
    const ResourceManager * const rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *s = rc->getResource(RC_MISSILE, BOSS02_MSTRAT4_BULLET_ID);

    LX_Vector2D v{BOSS02_MSTRAT4_SPEED, speed.vy / 2.0f};
    LX_Graphics::LX_ImgRect b[2] =
    {
        {
            position.p.x + BOSS02_MSTRAT4_BULLET_XOFF,
            position.p.y + BOSS02_MSTRAT4_BULLET_YOFF,
            BOSS02_MSTRAT4_BULLET_WIDTH, BOSS02_MSTRAT4_BULLET_HEIGHT
        }, {
            position.p.x + BOSS02_MSTRAT3_ROCKET_XOFF,
            position.p.y + BOSS02_MSTRAT3_ROCKET_YOFF,
            BOSS02_MSTRAT4_BULLET_WIDTH, BOSS02_MSTRAT4_BULLET_HEIGHT
        }
    };

    EntityHandler& hdl = EntityHandler::getInstance();
    hdl.pushEnemyMissile(*(new MegaBullet(attack_val, s, b[id], v,
                                          BOSS02_MSTRAT44_SPEED)));
    id = 1 - id;
}

void Boss02::visit(Missile&) {}

void Boss02::visit(PlayerRocket& rocket)
{
    const unsigned int damages = rocket.hit() / 2;

    if(!shield_destroyed)
    {
        if(damages > rshield_life)
            rshield_life = 0;
        else
            rshield_life -= damages;

        shield_destroyed = (rshield_life == 0);
        receiveDamages(damages);

        if(rshield_life == 0)
            graphic = sprite;
    }
}

void Boss02::absorb(Missile *m) noexcept
{
    m->accept(*this);
    m->die();
    hud->update();
}

void Boss02::fire() noexcept
{
    switch(id_strat)
    {
    case 1:
    case 2:
        mesh();
        break;

    case 3:
        target();
        break;

    case 4:
        danmaku();
        break;

    case 5:
        mesh();
        break;
    default:
        break;
    }
}

/// public functions

void Boss02::strategy() noexcept

{
    switch(id_strat)
    {
    case 0:
        prepareTheAttack();
        break;

    case 1:
        engage();
        break;

    case 2:
        meshAttack();
        break;

    case 3:
        targetAttack();
        break;

    case 4:
        bulletAttack();
        break;

    default:
        break;
    }

    Boss::strategy();
}


void Boss02::move() noexcept
{
    moveBox(global_hitbox, speed);
    moveBox(shield_hitbox, speed);
    movePoly(shape.getPoly(), speed);
    Boss::move();
}


void Boss02::collision(Missile *mi) noexcept
{
    const LX_Physics::LX_FloatingBox& HBOX = mi->getHitbox();

    if(!mi->isDead() && !mi->explosion() && mustCheckCollision())
    {
        if(has_shield && !shield_destroyed)
        {
            if(collisionBox(HBOX, shield_hitbox))
            {
                if(destroyable)
                    absorb(mi);
                return;
            }
        }

        if(collisionBox(HBOX, global_hitbox))
        {
            if(collisionBoxPoly(HBOX, shape.getPoly()))
            {
                reaction(mi);
                mi->die();
            }
        }
    }
}

void Boss02::collision(Player *play) noexcept
{
    if(!mustCheckCollision()) return;

    const LX_Physics::LX_Circle& hbox = play->getHitbox();

    if(has_shield && !shield_destroyed)
    {
        if(collisionCircleBox(hbox, shield_hitbox))
        {
            play->die();
            return;
        }
    }

    if(collisionCircleBox(hbox, global_hitbox))
    {
        if(collisionCirclePoly(hbox, shape.getPoly()))
            play->die();
    }

    return;
}


void Boss02::die() noexcept
{
    if(!dying)
    {
        const ResourceManager *rc = ResourceManager::getInstance();
        graphic = rc->getResource(RC_XPLOSION, BOSS02_SPRITE_DID);
        AudioHDL::getInstance()->stopBossMusic();
        AudioHDL::getInstance()->playVoiceMother();
        addStrategy(new BossDeathStrategy(this, DEFAULT_XPLOSION_DELAY,
                                          BOSS02_DELAY_NOISE));
    }

    Boss::die();
}
