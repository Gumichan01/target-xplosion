
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

#include "LoadingScreen.hpp"
#include "../asset/TX_Asset.hpp"
#include "../resources/WinID.hpp"
#include "../utils/misc.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/utils/utf8_string.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Event.hpp>

#include <sstream>

using namespace LX_Graphics;
using namespace LX_Win;

/// @todo (#8#) Loading screen (image)

namespace
{
const LX_Colour WHITE_COLOUR = {255, 255, 255, 240};
const int FONT_SZ = 72;
}

LoadingScreen::LoadingScreen()
    : previous(0UL), w(LX_WindowManager::getInstance().getWindow(WinID::getWinID())),
      font(TX_Asset::getInstance()->getFontFile(), WHITE_COLOUR, FONT_SZ),
      tvalue(font, w) {}

void LoadingScreen::operator()(const unsigned long nb, const unsigned long total) noexcept
{
    const unsigned long percentage = nb * 100UL / total;

    if(percentage != previous)
    {
        previous = percentage;
        tvalue.setText(UTF8string(misc::to_string(percentage)));
        tvalue.setPosition(w.getLogicalWidth()  - tvalue.getTextWidth(),
                           w.getLogicalHeight() - tvalue.getTextHeight());

        // I just need to get an on-the-fly instance of LX_EventHandler.
        // On Windows, if I don't put this line, the window freezes
        // There is no problem on Linux systems
        LX_Event::LX_EventHandler().pollEvent();

        w.clearWindow();
        tvalue.draw();
        w.update();
    }
}
