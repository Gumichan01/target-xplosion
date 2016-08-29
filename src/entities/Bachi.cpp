
/*
*   Target_Xplosion - A classic shoot'em up video game
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

#include "Bachi.hpp"
#include "Bullet.hpp"
#include "Item.hpp"

#include "../game/Rank.hpp"
#include "../game/Game.hpp"

#include "../entities/Player.hpp"
#include "../pattern/BulletPattern.hpp"
#include "../pattern/Strategy.hpp"
#include "../resources/ResourceManager.hpp"

using namespace LX_Physics;

const int BACHI_BULLET_OFFSET_X = 8;
const int BACHI_BULLET_OFFSET_Y = 16;
const int BACHI_BULLET_SIZE = 16;

const float BACHI_BULLET_VELOCITY = -8.0f;
const Uint32 BACHI_SHOT_DELAY = 1000;


Bachi::Bachi(unsigned int hp, unsigned int att, unsigned int sh,
             LX_Graphics::LX_Sprite *image, LX_Mixer::LX_Sound *audio,
             Sint16 x, Sint16 y, Uint16 w, Uint16 h,float vx, float vy)
    : Enemy(hp,att,sh,image,audio,x,y,w,h,vx,vy)
{
    initBachi();
}


Bachi::~Bachi()
{
    // Empty
}


void Bachi::initBachi()
{
    unsigned int r = Rank::getRank();
    MoveAndShootStrategy *mvs = nullptr;
    ShotStrategy *st = nullptr;

    mvs = new MoveAndShootStrategy(this);
    st = new ShotStrategy(this);
    strat = mvs;

    if(r == S_RANK)
        st->setShotDelay(BACHI_SHOT_DELAY/4);
    else if(r == A_RANK)
        st->setShotDelay(BACHI_SHOT_DELAY/2);
    else
        st->setShotDelay(BACHI_SHOT_DELAY);
    mvs->addMoveStrat(new PseudoSinusMoveStrategy(this));
    mvs->addShotStrat(st);
}


void Bachi::fire(void)
{
    unsigned int r = Rank::getRank();
    LX_Vector2D bullet_speed[3];

    SDL_Rect shot_area = {position.x + BACHI_BULLET_OFFSET_X,
                          position.y + BACHI_BULLET_OFFSET_Y,
                          BACHI_BULLET_SIZE, BACHI_BULLET_SIZE
                         };

    Player::accept(this);

    if(last_player_x < (position.x - (position.w*2)))
    {
        BulletPattern::shotOnTarget(position.x,position.y +(position.h/2),
                                    last_player_x, last_player_y,
                                    static_cast<int>(BACHI_BULLET_VELOCITY),
                                    bullet_speed[0]);

        // Change the y speed to get a spread shot
        bullet_speed[1] = bullet_speed[0];
        bullet_speed[2] = bullet_speed[0];
        bullet_speed[1].vx -= 1.0f;
        bullet_speed[2].vx -= 1.0f;
        bullet_speed[1].vy += 1.0f;
        bullet_speed[2].vy -= 1.0f;

        // Normalize the two vectors
        normalize(bullet_speed[1]);
        normalize(bullet_speed[2]);
        multiply(bullet_speed[1],-BACHI_BULLET_VELOCITY);
        multiply(bullet_speed[2],-BACHI_BULLET_VELOCITY);

        // The bullet has the same y speed, change their value
        if(static_cast<int>(bullet_speed[1].vy) ==
                static_cast<int>(bullet_speed[0].vy))
            bullet_speed[1].vy += 1.0f;

        if(static_cast<int>(bullet_speed[2].vy) ==
                static_cast<int>(bullet_speed[0].vy))
            bullet_speed[2].vy -= 1.0f;

        int n = 0;
        Game *g = Game::getInstance();
        ResourceManager *rc = ResourceManager::getInstance();

        // Set the number of bullets
        if(r == NO_RANK || r == C_RANK)
            n = 1;
        else if(r == B_RANK)
            n = 2;
        else
            n = 3;

        for(int i = 0; i < n; i++)
        {
            g->acceptEnemyMissile(new Bullet(attack_val,
                                             rc->getResource(RC_MISSILE,PLAYER_MISSILES+4),
                                             nullptr,shot_area,bullet_speed[i]));
        }
    }
}

void Bachi::reaction(Missile *target)
{
    Enemy::reaction(target);

    if(was_killed)
        Game::getInstance()->acceptItem(new Item(position.x,position.y));
}

