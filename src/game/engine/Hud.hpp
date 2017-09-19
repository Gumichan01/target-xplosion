
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
class Boss;
class Enemy;

namespace LX_TrueTypeFont
{
class LX_Font;
}

namespace LX_Graphics
{
class LX_Sprite;
class LX_TextTexture;
}

namespace libtagpp
{
class Tag;
}

class HUD
{

public:

    HUD();
    virtual void update() = 0;
    virtual void displayHUD() = 0;
    virtual ~HUD();
};


class EnemyHUD : public HUD
{
    Enemy& enemy;
    LX_Graphics::LX_Sprite *gauge;
    LX_Graphics::LX_Sprite *grad;
    unsigned int nb_graduation;
    unsigned int grad_max;

    EnemyHUD(const EnemyHUD&);
    EnemyHUD& operator =(const EnemyHUD&);

protected:

    void displayGauge();

public:

    explicit EnemyHUD(Enemy& e);
    virtual void update();
    virtual void displayHUD();
    virtual ~EnemyHUD() = default;
};


class BossHUD: public HUD
{
    Boss& boss;
    LX_Graphics::LX_Sprite *gauge;
    LX_Graphics::LX_Sprite *grad;
    unsigned int nb_graduation;
    bool filled;
    unsigned int fill_level;

    BossHUD(const BossHUD&);
    BossHUD& operator =(const BossHUD&);
    void fillGauge();
    void displayGauge();

public:

    explicit BossHUD(Boss& b);
    virtual void update();
    virtual void displayHUD();
    virtual ~BossHUD() = default;
};

class PlayerHUD: public HUD
{
    Player& subject;
    unsigned int player_hp;
    unsigned int player_hp_max;
    unsigned int player_rockets;
    unsigned int player_bombs;
    LX_TrueTypeFont::LX_Font *hud_font;
    LX_Graphics::LX_Sprite *health_symbol;
    LX_Graphics::LX_Sprite *missile_symbol;
    LX_Graphics::LX_Sprite *bomb_symbol;
    LX_Graphics::LX_TextTexture *hp_val_tx;
    LX_Graphics::LX_TextTexture *missile_val_tx;
    LX_Graphics::LX_TextTexture *bomb_val_tx;

    PlayerHUD(const PlayerHUD& hud);
    PlayerHUD& operator =(const PlayerHUD& hud);

    void drawHealth();
    void drawMissile();
    void drawBomb();

public:

    explicit PlayerHUD(Player& sub);
    virtual void update();
    virtual void displayHUD();
    virtual ~PlayerHUD();
};

// Background music
class BGM : public HUD
{
    unsigned int t;
    libtagpp::Tag *tag;
    LX_TrueTypeFont::LX_Font *bgm_font;
    LX_Graphics::LX_TextTexture *bgm_tx;

    BGM(const BGM&);
    BGM& operator =(const BGM&);

public:

    explicit BGM(unsigned int lvl);
    virtual void update();
    virtual void displayHUD();
    virtual ~BGM();
};

#endif // PlayerHUD_H_INCLUDED

