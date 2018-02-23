
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

#ifndef LOADINGSCREEN_HPP_INCLUDED
#define LOADINGSCREEN_HPP_INCLUDED

#include <LunatiX/LX_Texture.hpp>
#include <LunatiX/LX_TrueTypeFont.hpp>

namespace LX_Win
{
class LX_Window;
}

class LoadingScreen
{
    unsigned long previous;
    LX_Win::LX_Window *w;
    LX_TrueTypeFont::LX_Font font;
    LX_Graphics::LX_SolidTextTexture tvalue;

public:

    LoadingScreen();
    // pre-condtion : total ≠ 0
    void operator ()(const unsigned long nb, const unsigned long total) noexcept;
    ~LoadingScreen() = default;

};

#endif // LOADINGSCREEN_HPP_INCLUDED
