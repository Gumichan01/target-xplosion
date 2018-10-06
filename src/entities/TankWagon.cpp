
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


#include "TankWagon.hpp"
#include "../pattern/Strategy.hpp"
#include "../game/engine/AudioHandler.hpp"

#include <Lunatix/Texture.hpp>

TankWagon::TankWagon( unsigned int hp, unsigned int att, unsigned int sh,
                      lx::Graphics::Sprite * image, int x, int y, int w, int h,
                      float vx, float vy )
    : BigEnemy( hp, att, sh, image, x, y, w, h, vx, vy )
{
    mvs->addMoveStrat( new MoveStrategy( this ) );
    addStrategy( mvs );
}

void TankWagon::boom() noexcept
{
    AudioHandler::AudioHDL::getInstance()->playMediumExplosion();
}

void TankWagon::draw() noexcept
{
    if ( dying )
    {
        const int N = 9;
        lx::Graphics::ImgRect box[N] =
        {
            {48, 64, 64, 64}, {128, 20, 64, 64}, {96, 128, 64, 64},
            {128, 160, 64, 64}, {260, 182, 64, 64}, {220, 16, 64, 64},
            {182, 102, 64, 64}, {348, 56, 64, 64}, {452, 64, 64, 64}
        };

        imgbox = lx::Graphics::toImgRect( phybox );

        for ( int i = 0; i < N; i++ )
        {
            box[i].p.x += imgbox.p.x;
            box[i].p.y += imgbox.p.y;
            graphic->draw( box[i] );
        }
    }
    else
        BigEnemy::draw();
}
