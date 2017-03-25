
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

namespace
{

// Player
const int PLAYER_HUD_SIZE = 28;             // The font size of the HUD texts
const int PLAYER_HUD_OFFSET = 800;          // The reference position of the HUD

// X position of the texts
const int PLAYER_HUD_XPOS1 = PLAYER_HUD_OFFSET/4;
const int PLAYER_HUD_XPOS2 = PLAYER_HUD_OFFSET/2;
const int PLAYER_HUD_YPOS = 1;              // Y position of the HUD texts
const LX_Colour PLAYER_HUD_WHITE_COLOUR = {255,255,255,0};

const std::string HEALTH_STRING = "Health";
const std::string MISSILE_STRING = "Missile";
const std::string BOMB_STRING = "Bomb";

// Boss

const int BOSS_RC_GAUGE = 5;
const int BOSS_RC_GRAD = 6;

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
      hud_font(nullptr), hp_str_tx(nullptr), missile_str_tx(nullptr),
      bomb_str_tx(nullptr), hp_val_tx(nullptr), missile_val_tx(nullptr),
      bomb_val_tx(nullptr)
{
    LX_Window *win = LX_WindowManager::getInstance()->getWindow(WinID::getWinID());
    hud_font = new LX_Font(TX_Asset::getInstance()->getFontFile(),
                           PLAYER_HUD_WHITE_COLOUR, PLAYER_HUD_SIZE);

    // Labels
    hp_str_tx = new LX_BlendedTextTexture(HEALTH_STRING, *hud_font, *win);
    missile_str_tx = new LX_BlendedTextTexture(MISSILE_STRING, *hud_font, *win);
    bomb_str_tx = new LX_BlendedTextTexture(BOMB_STRING, *hud_font, *win);

    // Values
    hp_val_tx = new LX_BlendedTextTexture(*hud_font, *win);
    missile_val_tx = new LX_BlendedTextTexture(*hud_font, *win);
    bomb_val_tx = new LX_BlendedTextTexture(*hud_font, *win);

    hp_str_tx->setPosition(PLAYER_HUD_XPOS1, PLAYER_HUD_YPOS);
    missile_str_tx->setPosition(PLAYER_HUD_XPOS2, PLAYER_HUD_YPOS);
    bomb_str_tx->setPosition(PLAYER_HUD_XPOS1 + PLAYER_HUD_XPOS2, PLAYER_HUD_YPOS);
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
    hp_stream << player_hp << " / " << player_hp_max;
    missile_stream << player_rockets;
    bomb_stream << player_bombs;

    hp_val_tx->setText(hp_stream.str());
    missile_val_tx->setText(missile_stream.str());
    bomb_val_tx->setText(bomb_stream.str());

    int w, h;   // w is useless
    hp_str_tx->getTextDimension(w,h);
    hp_val_tx->setPosition(PLAYER_HUD_XPOS1, h+1);
    missile_str_tx->getTextDimension(w,h);
    missile_val_tx->setPosition(PLAYER_HUD_XPOS2, h+1);
    bomb_str_tx->getTextDimension(w,h);
    bomb_val_tx->setPosition(PLAYER_HUD_XPOS1 + PLAYER_HUD_XPOS2, h+1);

    // Display
    hp_str_tx->draw();
    hp_val_tx->draw();
    missile_str_tx->draw();
    bomb_str_tx->draw();
    missile_val_tx->draw();
    bomb_val_tx->draw();
}


PlayerHUD::~PlayerHUD()
{
    delete bomb_val_tx;
    delete missile_val_tx;
    delete hp_val_tx;
    delete bomb_str_tx;
    delete missile_str_tx;
    delete hp_str_tx;
    delete hud_font;
}
