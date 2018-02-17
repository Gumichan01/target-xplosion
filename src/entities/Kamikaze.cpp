
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017 Luxon Jean-Pierre
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

#include "Kamikaze.hpp"
#include "Player.hpp"
#include "Bullet.hpp"
#include "../pattern/Strategy.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../game/engine/EntityHandler.hpp"
#include "../resources/ResourceManager.hpp"

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_Physics.hpp>


namespace
{
const unsigned int KAMIKAZE_BULLET_ID = 9;

const int KAMIKAZE_XOFF = 33;
const int KAMIKAZE_YOFF = 17;
const int KAMIKAZE_DIM = 30;
const unsigned int KAMIKAZE_SHOT_DELAY = 250;
const float KAMIKAZE_BULLET_RATIO = 1.75f;
}


Kamikaze::Kamikaze(unsigned int hp, unsigned int att, unsigned int sh,
                   LX_Graphics::LX_Sprite *image, int x, int y, int w, int h,
                   float vx, float vy)
    : Enemy(hp, att, sh, image, x, y, w, h,vx, vy), max_speed(0)
{
    ShotStrategy *shot = new ShotStrategy(this);
    shot->setShotDelay(KAMIKAZE_SHOT_DELAY);

    // mvs is member of Enemy
    mvs->addMoveStrat(new MoveStrategy(this));
    mvs->addShotStrat(shot);
    addStrategy(mvs);

    using LX_Physics::vector_norm;
    max_speed = -vector_norm(speed);
}


void Kamikaze::draw()
{
    if(graphic != nullptr)
    {
        double angle = 0.0;
        BulletPattern::calculateAngle(speed, angle);
        graphic->draw(&position, angle);
    }
}


void Kamikaze::strategy()
{
    PlayerVisitor visitor;
    Player::accept(&visitor);

    if(visitor.getLastX() < position.x)
    {
        // I don't need to create another function
        // to make the enemy go to the player
        // ShotOnPlayer() does the job
        using LX_Physics::LX_Vector2D;
        LX_Vector2D v(speed);
        BulletPattern::shotOnPlayer(hitbox.center.x, hitbox.center.y, max_speed, v);
        speed = v;
    }
    Enemy::strategy();
}


void Kamikaze::fire()
{
    LX_AABB pos = {position.x + KAMIKAZE_XOFF, position.y + KAMIKAZE_YOFF,
                   KAMIKAZE_DIM, KAMIKAZE_DIM
                  };

    using LX_Physics::LX_Vector2D;
    const ResourceManager *rc = ResourceManager::getInstance();
    LX_Graphics::LX_Sprite *spr = rc->getResource(RC_MISSILE, KAMIKAZE_BULLET_ID);

    LX_Vector2D v(speed * KAMIKAZE_BULLET_RATIO);
    EntityHandler::getInstance().pushEnemyMissile(*(new Bullet(attack_val, spr, pos, v)));
}
