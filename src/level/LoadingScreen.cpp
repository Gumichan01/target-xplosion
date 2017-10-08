
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
    : w(LX_WindowManager::getInstance()->getWindow(WinID::getWinID())),
      font(TX_Asset::getInstance()->getFontFile(), WHITE_COLOUR, FONT_SZ) {}

void LoadingScreen::operator()(int nb, int total)
{
    std::ostringstream osint;
    osint << nb * 100 / total;
    UTF8string u8str(osint.str());
    LX_Graphics::LX_SolidTextTexture tvalue(u8str, font, *w);
    tvalue.setPosition(w->getWidth() - tvalue.getTextWidth(), w->getHeight() - tvalue.getTextHeight());

    {
        LX_Event::LX_EventHandler ev;
        ev.pollEvent();
    }

    w->clearWindow();
    tvalue.draw();
    w->update();
}

LoadingScreen::~LoadingScreen() {}
