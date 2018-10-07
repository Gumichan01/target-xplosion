
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

class TargetXplosion final
{
    static bool debug_mode;
    bool gui_mode;

    TargetXplosion( const TargetXplosion& ) = delete;
    TargetXplosion& operator=( const TargetXplosion& ) = delete;
    TargetXplosion( const TargetXplosion&& ) = delete;
    TargetXplosion& operator=( const TargetXplosion&& ) = delete;

    void sdlConfig() noexcept;
    void xmlConfig();
    void i18n() noexcept;
    void debug();
    void release();

public:

    static bool isDebugged() noexcept;

    explicit TargetXplosion( bool gui = true, bool todebug = false );
    void run();
    ~TargetXplosion();
};
