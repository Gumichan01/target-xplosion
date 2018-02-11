
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

#include "EntityHandler.hpp"
#include "AudioHandler.hpp"

#include "../Background.hpp"
#include "../../entities/Item.hpp"
#include "../../entities/Enemy.hpp"
#include "../../entities/Missile.hpp"
#include "../../entities/Rocket.hpp"
#include "../../resources/EnemyInfo.hpp"
#include "../../resources/EnemyInfo.hpp"
#include "../../level/Level.hpp"

#include <LunatiX/LX_Timer.hpp>


// Entity handler
EntityHandler& EntityHandler::getInstance() noexcept
{
    static EntityHandler singleton;
    return singleton;
}

void EntityHandler::setGameEnv(GameEnv& env) noexcept
{
    genv.level = env.level;
    genv.background = env.background;
}

bool EntityHandler::generateEnemy() noexcept
{
    using AudioHandler::AudioHDL;
    AudioHDL * audiohdl = AudioHDL::getInstance();

    EnemyInfo data;

    if(genv.level == nullptr)
        return false;

    if(genv.level->statEnemyInfo(data))
    {
        if((LX_Timer::getTicks() - start_point) > data.t)
        {
            genv.level->popData();

            if(data._alarm)
            {
                genv.background->setIncrease();
                audiohdl->playAlarm();
                audiohdl->playVoiceBoss();
            }
            else if(data.boss)
                audiohdl->playBossMusic();

            if(data.e != nullptr)
            {
                enemies.push_back(data.e);
                data.e->start();
            }

            return true;
        }
    }
    return false;
}
