
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
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

#ifndef HUD2_H_INCLUDED
#define HUD2_H_INCLUDED

class Player;

namespace LX_TrueTypeFont
{
class LX_Font;
};

/// @todo (#3#) v0.4.7: HUD - adapt it for the player and the boss
/// @todo (#2#) v0.4.6: HUD - modify it so that it can handle a specific font

class HUD
{
    Player& subject;
    unsigned int player_hp;
    unsigned int player_hp_max;
    unsigned int player_rockets;
    unsigned int player_bombs;
    LX_TrueTypeFont::LX_Font *hud_font;

    HUD(const HUD& hud);

public :

    explicit HUD(Player& sub);

    void update();
    void displayHUD();

    ~HUD();
};

#endif // HUD2_H_INCLUDED

