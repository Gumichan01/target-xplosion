
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

#include "LoadingScreen.hpp"
#include "../asset/TX_Asset.hpp"
#include "../resources/WinID.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/utils/utf8_string.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Event.hpp>

#include <sstream>

using namespace LX_Graphics;
using namespace LX_Win;

/// @todo Loading screen

namespace
{
const LX_Colour WHITE_COLOUR = {255,255,255,0};
const int FONT_SZ = 72;
}

LoadingScreen::LoadingScreen()
    : previous(-1), w(LX_WindowManager::getInstance()->getWindow(WinID::getWinID())),
      font(TX_Asset::getInstance()->getFontFile(), WHITE_COLOUR, FONT_SZ),
      tvalue(font,*w) {}

void LoadingScreen::operator()(const int nb, const int total)
{
    const int percentage = nb * 100 / total;

    if(percentage != previous)
    {
        previous = percentage;
        std::ostringstream osint;
        osint << percentage;
        tvalue.setText(UTF8string(osint.str()));
        tvalue.setPosition(w->getWidth() - tvalue.getTextWidth(), w->getHeight() - tvalue.getTextHeight());

        {
            LX_Event::LX_EventHandler().pollEvent();
        }

        w->clearWindow();
        tvalue.draw();
        w->update();
    }
}

LoadingScreen::~LoadingScreen() {}