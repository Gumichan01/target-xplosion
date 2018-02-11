
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
#include "Engine.hpp"
#include "EntityHandler.hpp"
#include "../../level/Level.hpp"
#include "../../entities/Player.hpp"
#include "../../entities/boss/Boss.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../resources/WinID.hpp"
#include "../../asset/TX_Asset.hpp"

#include <LunatiX/utils/libtagspp/libtagspp.hpp>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Timer.hpp>

#include <algorithm>
#include <sstream>

using namespace LX_Win;
using namespace LX_TrueTypeFont;
using namespace LX_Graphics;

#define ICAST(x) static_cast<int>(x)

namespace
{
// Player
const int PLAYER_HUD_SIZE = 56;             // The font size of the HUD texts
const int PLAYER_HUD_OFFSET = 650;          // The reference position of the HUD

// X position of the texts
const int PLAYER_HUD_XPOS1 = PLAYER_HUD_OFFSET/4;
const int PLAYER_HUD_XPOS2 = PLAYER_HUD_OFFSET/2;
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

const int BOSS_HUD_XPOS = 702;
const int BOSS_HUD_YPOS = 1;
const int BOSS_HUD_W = 570;
const int BOSS_HUD_H = 64;

const int BOSS_HUD_DX = 6;
const int BOSS_HUD_DY = BOSS_HUD_DX;

const int BOSS_GRAD_MAX = BOSS_HUD_W - 2 * BOSS_HUD_DX;
const int BOSS_GRAD_W = 1;
const int BOSS_GRAD_H = 54;

const int ENEMY_GRAD_H = 12;

unsigned int FILL_STEP = 4;
LX_AABB bgrad = {0, BOSS_HUD_YPOS + BOSS_HUD_DY, BOSS_GRAD_W, BOSS_GRAD_H};

// BGM
const unsigned int BGM_SIZE = 28;
const unsigned int BGM_DELAY = 4500;
const LX_Colour BGM_DCOLOUR = {255, 255, 255, 255};

// Viewport
const int HUD_VPORT_H = 68;
// Fading
const int FADE_MAX_VALUE = 255;
}

uint8_t HudHandler::fade_out_counter = 0;

// HUD handler
HudHandler& HudHandler::getInstance() noexcept
{
    static HudHandler singleton;
    return singleton;
}

bool HudHandler::addHUD(HUD& hud) noexcept
{
    bool found = std::any_of(huds.begin(), huds.end(), [&hud](const HUD * h)
    {
        return h == &hud;
    });

    if(found)
        return false;

    huds.push_back(&hud);
    return true;
}

void HudHandler::setBGM(BGM& bg) noexcept
{
    bgm = &bg;
}

bool HudHandler::removeHUD(HUD& hud) noexcept
{
    auto found = std::find(huds.begin(), huds.end(), &hud);

    if(found != huds.cend())
    {
        huds.erase(found);
        return true;
    }

    return false;
}

void HudHandler::fadeOut(bool& end_of_level)
{
    LX_Window *win = LX_WindowManager::getInstance()->getWindow(WinID::getWinID());

    if(fade_out_counter < FADE_MAX_VALUE)
    {
        LX_Colour colour{0, 0, 0, fade_out_counter};
        LX_AABB box{0, 0, Engine::getMaxXlim(), Engine::getMaxYlim()};

        win->setDrawColour(colour);
        fade_out_counter++;
        win->fillRect(box);
    }
    else
    {
        fade_out_counter = 0;
        end_of_level = true;
        win->clearWindow();
    }
}

void HudHandler::displayHUD()
{
    LX_Window *win = LX_WindowManager::getInstance()->getWindow(WinID::getWinID());
    LX_AABB viewport{0, 0, Engine::getMaxXlim(), HUD_VPORT_H};
    const LX_AABB& cvport = viewport;
    LX_Colour bcolour{0,0,0,64};

    bgm->displayHUD();
    win->setViewPort(&viewport);
    win->setDrawColour(bcolour);
    win->fillRect(cvport);

    std::for_each(huds.begin(), huds.end(), [](HUD *hud)
    {
        hud->displayHUD();
    });
}

void HudHandler::clearHUDs()
{
    huds.clear();
}


// HUD (Head-Up Display)
HUD::HUD() {}
HUD::~HUD() {}


// Enemy HUD
EnemyHUD::EnemyHUD(Enemy& e)
    : enemy(e), gauge(nullptr), grad(nullptr), nb_graduation(e.getWidth()),
      grad_max(e.getWidth() - 1)
{
    const ResourceManager *rc = ResourceManager::getInstance();
    gauge = rc->getMenuResource(BOSS_RC_GAUGE);
    grad = rc->getMenuResource(BOSS_RC_GRAD);
}


void EnemyHUD::displayGauge()
{
    LX_AABB egrad = {enemy.getX(), enemy.getY() - ENEMY_GRAD_H, 1, ENEMY_GRAD_H};
    _displayGauge(enemy.getX(), egrad);
}

void EnemyHUD::_displayGauge(int x, LX_AABB& rect)
{
    for(unsigned int i = 1; i <= nb_graduation; i++)
    {
        rect.x = x + (ICAST(i) + 1) * 1;
        grad->draw(&rect);
    }
}


void EnemyHUD::update()
{
    const unsigned int hp  = enemy.getHP();
    const unsigned int mhp = enemy.getMaxHP();
    nb_graduation = hp * grad_max / mhp;
}


void EnemyHUD::displayHUD()
{
    LX_AABB bgauge = {enemy.getX(), enemy.getY() - ENEMY_GRAD_H, enemy.getWidth(), ENEMY_GRAD_H};
    gauge->draw(&bgauge);
    displayGauge();
}



// HUD of any boss/semi-boss
BossHUD::BossHUD(Boss& b)
    : EnemyHUD(b), boss(b), filled(false), fill_level(1)
{
    nb_graduation = BOSS_GRAD_MAX;
    grad_max = BOSS_GRAD_MAX;
}

void BossHUD::fillGauge()
{
    for(unsigned int j = 1; j < fill_level; j++)
    {
        bgrad.x = BOSS_HUD_XPOS + (ICAST(j) + 1) * BOSS_GRAD_W;
        grad->draw(&bgrad);
    }

    if(fill_level >= nb_graduation)
        filled = true;
    else
        fill_level += FILL_STEP;
}


void BossHUD::displayGauge()
{
    _displayGauge(BOSS_HUD_XPOS, bgrad);
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
    hud_font = new LX_Font(asset->getFontFile(), PLAYER_HUD_WHITE_COLOUR, PLAYER_HUD_SIZE);

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


BGM::BGM(unsigned int lvl) : t(0), tag(nullptr), bgm_font(nullptr), bgm_tx(nullptr)
{
    const std::string mstring = TX_Asset::getInstance()->getLevelMusic(lvl);
    const TX_Asset *a = TX_Asset::getInstance();
    LX_Window *w = LX_WindowManager::getInstance()->getWindow(WinID::getWinID());
    LX_Colour bgm_colour  = BGM_DCOLOUR;

    bgm_font = new LX_Font(a->getFontFile(), bgm_colour, BGM_SIZE);
    bgm_tx = new LX_BlendedTextTexture(*bgm_font, *w);
    tag = new libtagpp::Tag();
    tag->readTag(mstring);
    update();
}


void BGM::update()
{
    int w, h;
    const int W = Engine::getInstance()->getMaxXlim();
    const int H = Engine::getInstance()->getMaxYlim();
    std::string _artist = tag->artist();
    std::string _title  = tag->title();
    UTF8string bgm_text("BGM. " + _artist + " - " + _title + " ");

    bgm_tx->setText(bgm_text);
    bgm_tx->getTextDimension(w, h);
    bgm_tx->setPosition(W - w, H - h);
    t = LX_Timer::getTicks();
}

void BGM::displayHUD()
{
    if((LX_Timer::getTicks() - t) <= BGM_DELAY)
        bgm_tx->draw();
}

BGM::~BGM()
{
    delete tag;
    delete bgm_tx;
    delete bgm_font;
}
