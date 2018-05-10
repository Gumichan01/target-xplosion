
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

#include "Enemy.hpp"

#include "BasicMissile.hpp"
#include "../asset/TX_Asset.hpp"
#include "../tx/TargetXplosion.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../game/engine/AudioHandler.hpp"
#include "../game/Scoring.hpp"
#include "../entities/Player.hpp"
#include "../pattern/Strategy.hpp"
#include "../resources/ResourceManager.hpp"
#include "../resources/WinID.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>


namespace
{

const int ENEMY_BMISSILE_ID = 9;
const unsigned int ENEMY_EXPLOSION_ID = 8;
const unsigned int ENEMY_EXPLOSION_DELAY = 250;
const unsigned int ENEMY_INVICIBILITY_DELAY = 100;
const unsigned int ENEMY_DIV10 = 10;
LX_Graphics::LX_BufferedImage *xbuff = nullptr;

}

using namespace LX_Physics;
using namespace MissileInfo;
using namespace FloatBox;

void Enemy::loadExplosionBuffer()
{
    using LX_Graphics::LX_BufferedImage;
    const TX_Asset * const A = TX_Asset::getInstance();
    xbuff = new LX_BufferedImage(A->getExplosionSpriteFile(ENEMY_EXPLOSION_ID));
}

void Enemy::destroyExplosionBuffer() noexcept
{
    delete xbuff;
    xbuff = nullptr;
}


Enemy::Enemy(unsigned int hp, unsigned int att, unsigned int sh,
             LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
             float vx, float vy)
    : Character(hp, att, sh, image, tobox(x,y,w,h), LX_Vector2D{vx, vy}),
      strat(nullptr), xtexture(nullptr), mvs(new MoveAndShootStrategy(this)),
      tick(0), ut(0), destroyable(false)
{
    // An enemy that has no graphical repreesntation cannot exist
    if(graphic == nullptr)
        LX_Log::logError(LX_Log::LX_LogType::APPLICATION, "enemy - No graphical resource");

    const TX_Asset * const ASSET = TX_Asset::getInstance();
    const TX_Anima * const ANIMA = ASSET->getExplosionAnimation(ENEMY_EXPLOSION_ID);

    LX_Win::LX_Window& WIN = LX_Win::getWindowManager().getWindow(WinID::getWinID());
    xtexture = xbuff->generateAnimatedSprite(WIN, ANIMA->v, ANIMA->delay, false);

    if(xtexture == nullptr)
        LX_Log::logError(LX_Log::LX_LogType::APPLICATION, "enemy - No explosion resource");
}

Enemy::~Enemy()
{
    if(strat == mvs)
    {
        delete mvs;
        strat = nullptr;
    }
    else
        delete strat;

    delete xtexture;
}


void Enemy::move() noexcept
{
    LX_Physics::moveBox(phybox, speed);
    LX_Physics::moveCircle(circle_box, speed);
}


void Enemy::start() noexcept
{
    ut = LX_Timer::getTicks();
}

// Use the strategy
void Enemy::strategy() noexcept
{
    if(!destroyable && (LX_Timer::getTicks() - ut) > ENEMY_INVICIBILITY_DELAY)
        destroyable = true;

    if(strat != nullptr)
        strat->proceed();
}


void Enemy::boom() noexcept
{
    AudioHandler::AudioHDL::getInstance()->playSmallExplosion();
}

void Enemy::fire() noexcept
{
    LX_Graphics::LX_ImgRect pos_mis;
    LX_Vector2D sp_mis = LX_Vector2D{-MISSILE_SPEED, FNIL};

    const ResourceManager * const RC = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = RC->getResource(RC_MISSILE, ENEMY_BMISSILE_ID);

    pos_mis.p.x = imgbox.p.x - MISSILE_WIDTH;
    pos_mis.p.y = imgbox.p.y + ((imgbox.h - MISSILE_HEIGHT)/ 2);
    pos_mis.w = MISSILE_WIDTH;
    pos_mis.h = MISSILE_HEIGHT;

    EntityHandler& hdl = EntityHandler::getInstance();
    hdl.pushEnemyMissile(*(new BasicMissile(attack_val, spr, pos_mis, sp_mis)));
}


void Enemy::collision(Missile *mi) noexcept
{
    if(!mi->isDead() && !mi->explosion() &&
            mi->getX() <= (imgbox.p.x + fbox(imgbox.w)) && !dying)
    {
        if(LX_Physics::collisionCircleBox(circle_box, mi->getHitbox()))
        {
            if(destroyable)
                reaction(mi);
            mi->die();
        }
    }
}

void Enemy::collision(Player *play) noexcept
{
    if(play->getX() <= (imgbox.p.x + fbox(imgbox.w)) && !dying)
    {
        if(LX_Physics::collisionCircle(play->getHitbox(), circle_box))
            play->die();
    }
}


// Define how the enemy react when it has collision with the following target
void Enemy::reaction(Missile *target) noexcept
{
    receiveDamages(target->hit());
    Engine::getInstance()->getScore()->notify(Scoring::DAMAGE_SCORE);
}

void Enemy::receiveDamages(unsigned int attacks) noexcept
{
    Character::receiveDamages(attacks);

    if(health_point < (max_health_point / ENEMY_DIV10))
        AudioHandler::AudioHDL::getInstance()->playEnemyHit();

    if(health_point == 0)
        Character::kill();
}

// Add a new strategy deleting the old one
void Enemy::addStrategy(Strategy *new_strat, bool delete_previous) noexcept
{
    if(delete_previous)
        delete strat;

    strat = new_strat;
}


void Enemy::die() noexcept
{
    if(!dying && imgbox.p.x >= -imgbox.w)
    {
        if(xtexture != nullptr)
        {
            xtexture->resetAnimation();
            graphic = xtexture;
        }

        boom();
        dying = true;
        speed *= 0.5f;
        addStrategy(new DeathStrategy(this, ENEMY_EXPLOSION_DELAY, ENEMY_EXPLOSION_DELAY));
    }
    else
    {
        // It is dead
        dying = false;
        Character::die();
    }
}


BigEnemy::BigEnemy(unsigned int hp, unsigned int att, unsigned int sh,
                   LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                   float vx, float vy)
    : Enemy(hp, att, sh, image, x, y, w, h, vx, vy), ehud(nullptr)
{
    if(TargetXplosion::isDebugged())
        ehud = new EnemyHUD(*this);
}


void BigEnemy::draw() noexcept
{
    Enemy::draw();

    if(TargetXplosion::isDebugged())
        ehud->displayHUD();
}

void BigEnemy::reaction(Missile *target) noexcept
{
    Enemy::reaction(target);

    if(TargetXplosion::isDebugged())
        ehud->update();
}

BigEnemy::~BigEnemy()
{
    delete ehud;
}
