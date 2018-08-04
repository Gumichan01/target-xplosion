
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

#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

#include <Lunatix/ImgRect.hpp>

#include <vector>

class Player;
class Boss;
class Enemy;

namespace lx
{

namespace TrueTypeFont
{
class Font;
}

namespace Graphics
{
class Sprite;
class TextTexture;
}

}

namespace libtagpp
{
class Tag;
}

class HUD;
class BGM;

class HudHandler
{
    BGM * bgm = nullptr;
    static unsigned char fade_out_counter;
    std::vector<HUD *> huds = {};

    HudHandler() = default;
    HudHandler( const HudHandler& ) = delete;
    HudHandler( const HudHandler&& ) = delete;
    HudHandler& operator =( const HudHandler& ) = delete;
    HudHandler& operator =( const HudHandler&& ) = delete;
    ~HudHandler() = default;

public:

    static HudHandler& getInstance() noexcept;

    bool addHUD( HUD& hud ) noexcept;
    void setBGM( BGM& bg ) noexcept;
    bool removeHUD( HUD& hud ) noexcept;

    void fadeOut( bool& end_of_level );
    void displayHUD();
    void clearHUDs();
};


class HUD
{

public:

    HUD() = default;
    virtual void update() = 0;
    virtual void displayHUD() = 0;
    virtual ~HUD() = default;
};


class EnemyHUD: public HUD
{
    Enemy& enemy;

    EnemyHUD( const EnemyHUD& );
    EnemyHUD& operator =( const EnemyHUD& );

protected:

    lx::Graphics::Sprite * gauge;
    lx::Graphics::Sprite * grad;
    unsigned int nb_graduation;
    unsigned int grad_max;
    void displayGauge();
    void _displayGauge( int x, lx::Graphics::ImgRect& rect );

public:

    explicit EnemyHUD( Enemy& e );
    virtual void update() override;
    virtual void displayHUD() override;
    virtual ~EnemyHUD() = default;
};


class BossHUD final: public EnemyHUD
{
    Boss& boss;
    bool filled;
    unsigned int fill_level;

    BossHUD( const BossHUD& );
    BossHUD& operator =( const BossHUD& );
    void fillGauge();
    void displayGauge();

public:

    explicit BossHUD( Boss& b );
    virtual void displayHUD() override;
    virtual ~BossHUD() = default;
};

class PlayerHUD final: public HUD
{
    Player& subject;
    unsigned int player_hp;
    unsigned int player_hp_max;
    unsigned int player_rockets;
    unsigned int player_bombs;
    lx::TrueTypeFont::Font * hud_font;
    lx::Graphics::Sprite * health_symbol;
    lx::Graphics::Sprite * missile_symbol;
    lx::Graphics::Sprite * bomb_symbol;
    lx::Graphics::TextTexture * hp_val_tx;
    lx::Graphics::TextTexture * missile_val_tx;
    lx::Graphics::TextTexture * bomb_val_tx;

    PlayerHUD( const PlayerHUD& hud );
    PlayerHUD& operator =( const PlayerHUD& hud );

    void drawHealth();
    void drawMissile();
    void drawBomb();

public:

    explicit PlayerHUD( Player& sub );
    virtual void update() override;
    virtual void displayHUD() override;
    virtual ~PlayerHUD();
};

// Background music
class BGM final: public HUD
{
    unsigned int t;
    libtagpp::Tag * tag;
    lx::TrueTypeFont::Font * bgm_font;
    lx::Graphics::TextTexture * bgm_tx;

    BGM( const BGM& );
    BGM& operator =( const BGM& );

public:

    explicit BGM( unsigned int lvl );
    virtual void update() override;
    virtual void displayHUD() override;
    virtual ~BGM();
};

#endif // PlayerHUD_H_INCLUDED
