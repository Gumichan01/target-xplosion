
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

#include "Hud.hpp"
#include "../../level/Level.hpp"
#include "../../entities/Player.hpp"
#include "../../entities/boss/Boss.hpp"
#include "../../asset/TX_Asset.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../resources/WinID.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <sstream>

using namespace LX_Win;
using namespace LX_TrueTypeFont;
using namespace LX_Graphics;

/// @todo (#1#) v0.4.7: Display BGM

namespace
{

// Player
const int PLAYER_HUD_SIZE = 56;             // The font size of the HUD texts
const int PLAYER_HUD_OFFSET = 650;          // The reference position of the HUD

// X position of the texts
const int PLAYER_HUD_XPOS1 = PLAYER_HUD_OFFSET/4;
const int PLAYER_HUD_XPOS2 = PLAYER_HUD_OFFSET/2;
const int PLAYER_HUD_YPOS = 1;              // Y position of the HUD texts
const LX_Colour PLAYER_HUD_WHITE_COLOUR = {255,255,255,0};

const unsigned int HEALTH_SPID = 0;
const unsigned int ROCKET_SPID = 1;
const unsigned int BOMB_SPID = 2;

LX_AABB rhealth_sym = {PLAYER_HUD_XPOS1, 12, 48, 48};
LX_AABB rrocket_sym = {PLAYER_HUD_XPOS2, 12, 80, 40};
LX_AABB rbomb_sym = {PLAYER_HUD_XPOS1 + PLAYER_HUD_XPOS2, 12, 48, 48};

// Boss
const int BOSS_RC_GAUGE = 4;
const int BOSS_RC_GRAD = 5;

const int BOSS_HUD_XPOS = 700;
const int BOSS_HUD_YPOS = 1;
const int BOSS_HUD_W = 570;
const int BOSS_HUD_H = 64;

const int BOSS_HUD_DX = 6;
const int BOSS_HUD_DY = BOSS_HUD_DX;

const int BOSS_GRAD_MAX = BOSS_HUD_W - 2 * BOSS_HUD_DX;
const int BOSS_GRAD_W = 1;
const int BOSS_GRAD_H = 54;

unsigned int FILL_STEP = 4;
LX_AABB bgrad = {0, BOSS_HUD_YPOS + BOSS_HUD_DY, BOSS_GRAD_W, BOSS_GRAD_H};
};

// HUD (Head-Up Display)
HUD::HUD() {}
HUD::~HUD() {}


// HUD of any boss/semi-boss
BossHUD::BossHUD(Boss& b)
    : boss(b), gauge(nullptr), grad(nullptr), nb_graduation(BOSS_GRAD_MAX),
      filled(false), fill_level(1)
{
    const ResourceManager *rc = ResourceManager::getInstance();
    gauge = rc->getMenuResource(BOSS_RC_GAUGE);
    grad = rc->getMenuResource(BOSS_RC_GRAD);
}

void BossHUD::fillGauge()
{
    for(unsigned int j = 1; j < fill_level; j++)
    {
        bgrad.x = BOSS_HUD_XPOS + BOSS_HUD_DX + (j + 1) * BOSS_GRAD_W;
        grad->draw(&bgrad);
    }

    if(fill_level >= nb_graduation)
        filled = true;
    else
        fill_level += FILL_STEP;
}

void BossHUD::displayGauge()
{
    for(unsigned int i = 1; i <= nb_graduation; i++)
    {
        bgrad.x = BOSS_HUD_XPOS + BOSS_HUD_DX + (i + 1) * BOSS_GRAD_W;
        grad->draw(&bgrad);
    }
}

void BossHUD::update()
{
    const unsigned int hp = boss.getHP();
    const unsigned int mhp = boss.getMaxHP();
    nb_graduation = hp * BOSS_GRAD_MAX / mhp;
}

void BossHUD::displayHUD()
{
    LX_AABB bgauge = {BOSS_HUD_XPOS, BOSS_HUD_YPOS, BOSS_HUD_W, BOSS_HUD_H};
    gauge->draw(&bgauge);

    if(!filled)
        fillGauge();
    else
        displayGauge();
}


// HUD of the player
PlayerHUD::PlayerHUD(Player& sub)
    : subject(sub), player_hp(sub.getHP()), player_hp_max(sub.getHP()),
      player_rockets(sub.getRocket()), player_bombs(sub.getBomb()),
      hud_font(nullptr), health_symbol(nullptr), missile_symbol(nullptr),
      bomb_symbol(nullptr), hp_val_tx(nullptr), missile_val_tx(nullptr),
      bomb_val_tx(nullptr)
{
    const TX_Asset *asset = TX_Asset::getInstance();
    LX_Window *win = LX_WindowManager::getInstance()->getWindow(WinID::getWinID());
    hud_font = new LX_Font(TX_Asset::getInstance()->getFontFile(),
                           PLAYER_HUD_WHITE_COLOUR, PLAYER_HUD_SIZE);

    // Labels
    health_symbol = new LX_Graphics::LX_Sprite(asset->getItemFile(HEALTH_SPID), *win);
    missile_symbol = ResourceManager::getInstance()->getResource(RC_MISSILE, ROCKET_SPID);
    bomb_symbol = ResourceManager::getInstance()->getResource(RC_MISSILE, BOMB_SPID);

    // Values
    hp_val_tx = new LX_BlendedTextTexture(*hud_font, *win);
    missile_val_tx = new LX_BlendedTextTexture(*hud_font, *win);
    bomb_val_tx = new LX_BlendedTextTexture(*hud_font, *win);
}

// Update information
void PlayerHUD::update()
{
    player_hp = subject.getHP();
    player_bombs = subject.getBomb();
    player_rockets = subject.getRocket();
}

// Display information
void PlayerHUD::displayHUD()
{
    std::ostringstream hp_stream, missile_stream, bomb_stream;

    // Get the values
    hp_stream << player_hp;
    missile_stream << player_rockets;
    bomb_stream << player_bombs;

    hp_val_tx->setText(hp_stream.str());
    missile_val_tx->setText(missile_stream.str());
    bomb_val_tx->setText(bomb_stream.str());

    // Position + Display
    drawHealth();
    drawMissile();
    if(Level::getLevelNum() >= Level::BOMB_LEVEL_MIN)
        drawBomb();
}

void PlayerHUD::drawHealth()
{
    health_symbol->draw(&rhealth_sym);
    hp_val_tx->setPosition(rhealth_sym.x + rhealth_sym.w, 1);
    hp_val_tx->draw();
}

void PlayerHUD::drawMissile()
{
    missile_symbol->draw(&rrocket_sym);
    missile_val_tx->setPosition(rrocket_sym.x + rrocket_sym.w, 1);
    missile_val_tx->draw();
}

void PlayerHUD::drawBomb()
{
    bomb_symbol->draw(&rbomb_sym);
    bomb_val_tx->setPosition(rbomb_sym.x + rbomb_sym.w, 1);
    bomb_val_tx->draw();
}

PlayerHUD::~PlayerHUD()
{
    delete bomb_val_tx;
    delete missile_val_tx;
    delete hp_val_tx;
    delete health_symbol;
    delete hud_font;
}
