
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

#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

class Player;

namespace LX_TrueTypeFont
{
class LX_Font;
};

namespace LX_Graphics
{
class LX_TextTexture;
};

/// @todo (#2#) v0.5.0: HUD - adapt it for the player and the boss

class PlayerHUD
{
    Player& subject;
    unsigned int player_hp;
    unsigned int player_hp_max;
    unsigned int player_rockets;
    unsigned int player_bombs;
    LX_TrueTypeFont::LX_Font *hud_font;
    LX_Graphics::LX_TextTexture *hp_str_tx;
    LX_Graphics::LX_TextTexture *missile_str_tx;
    LX_Graphics::LX_TextTexture *bomb_str_tx;
    LX_Graphics::LX_TextTexture *hp_val_tx;
    LX_Graphics::LX_TextTexture *missile_val_tx;
    LX_Graphics::LX_TextTexture *bomb_val_tx;

    PlayerHUD(const PlayerHUD& hud);

public :

    explicit PlayerHUD(Player& sub);

    void update();
    void displayHUD();

    ~PlayerHUD();
};

#endif // PlayerHUD_H_INCLUDED

