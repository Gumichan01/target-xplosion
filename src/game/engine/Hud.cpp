
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

#include "Hud.hpp"
#include "Engine.hpp"
#include "../../level/Level.hpp"
#include "../../entities/Player.hpp"
#include "../../entities/boss/Boss.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../resources/WinID.hpp"
#include "../../asset/TX_Asset.hpp"
#include "../../utils/misc.hpp"

#include <Lunatix/utils/libtagspp/libtagspp.hpp>
#include <Lunatix/Graphics.hpp>
#include <Lunatix/Time.hpp>

#include <algorithm>
#include <sstream>
#include <utility>

using namespace lx::Win;
using namespace lx::TrueTypeFont;
using namespace lx::Graphics;

#define ICAST(x) static_cast<int>(x)

namespace
{
// Player
const int PLAYER_HUD_SIZE = 56;             // The font size of the HUD texts
const int PLAYER_HUD_OFFSET = 650;          // The reference position of the HUD

// X position of the texts
const int PLAYER_HUD_XPOS1 = PLAYER_HUD_OFFSET / 4;
const int PLAYER_HUD_XPOS2 = PLAYER_HUD_OFFSET / 2;
const lx::Graphics::Colour PLAYER_HUD_WHITE_COLOUR = { 255, 255, 255, 240 };

const unsigned int HEALTH_SPID = 0;
const unsigned int ROCKET_SPID = 1;
const unsigned int BOMB_SPID = 2;

lx::Graphics::ImgRect rhealth_sym = { { PLAYER_HUD_XPOS1, 12 }, 48, 48 };
lx::Graphics::ImgRect rrocket_sym = { { PLAYER_HUD_XPOS2, 12 }, 80, 40 };
lx::Graphics::ImgRect rbomb_sym = { { PLAYER_HUD_XPOS1 + PLAYER_HUD_XPOS2, 12 }, 48, 48 } ;

// Boss
const int BOSS_RC_GAUGE = 4;
const int BOSS_RC_GRAD = 5;

const int BOSS_HUD_XPOS = 702;
const int BOSS_HUD_YPOS = 1;
const int BOSS_HUD_W = 570;
const int BOSS_HUD_H = 64;

const int BOSS_HUD_DX = 11;
const int BOSS_HUD_DY = 3;

const int BOSS_HUD_OFFSET = BOSS_HUD_DX / 2;

const int BOSS_GRAD_MAX = BOSS_HUD_W - BOSS_HUD_DX;
const int BOSS_GRAD_W = 1;
const int BOSS_GRAD_H = 58;

const int ENEMY_GRAD_H = 12;

unsigned int FILL_STEP = 4;
lx::Graphics::ImgRect bgrad = { 0, BOSS_HUD_YPOS + BOSS_HUD_DY, BOSS_GRAD_W, BOSS_GRAD_H };

// BGM
const unsigned int BGM_SIZE = 28;
const unsigned int BGM_DELAY = 4500;
const lx::Graphics::Colour BGM_DCOLOUR = { 255, 255, 255, 240 };

// Viewport
const int HUD_VPORT_H = 68;
// Fading
const int FADE_MAX_VALUE = 255;

}

unsigned char HudHandler::fade_out_counter = 0;

// HUD handler
HudHandler& HudHandler::getInstance() noexcept
{
    static HudHandler singleton;
    return singleton;
}

bool HudHandler::addHUD( HUD& hud ) noexcept
{
    bool found = std::any_of( huds.begin(), huds.end(), [&hud]( const HUD * h )
    {
        return h == &hud;
    } );

    if ( found )
        return false;

    huds.push_back( &hud );
    return true;
}

void HudHandler::setBGM( BGM& bg ) noexcept
{
    bgm = &bg;
}

bool HudHandler::removeHUD( HUD& hud ) noexcept
{
    auto found = std::find( huds.begin(), huds.end(), &hud );

    if ( found != huds.cend() )
    {
        huds.erase( found );
        return true;
    }

    return false;
}

void HudHandler::fadeOut( bool& end_of_level )
{
    Window& win = WindowManager::getInstance().getWindow( WinID::getWinID() );

    if ( fade_out_counter < FADE_MAX_VALUE )
    {
        lx::Graphics::Colour colour = { 0, 0, 0, fade_out_counter };
        const int MAX_X = static_cast<int>( Engine::getMaxXlim() );
        const int MAX_Y = static_cast<int>( Engine::getMaxYlim() );
        lx::Graphics::ImgRect box = { 0, 0, MAX_X, MAX_Y };

        win.setDrawColour( colour );
        fade_out_counter++;
        win.fillRect( box );
    }
    else
    {
        fade_out_counter = 0;
        end_of_level = true;
        win.clearWindow();
    }
}

void HudHandler::displayHUD()
{
    Window& win = WindowManager::getInstance().getWindow( WinID::getWinID() );
    lx::Graphics::ImgRect viewport = { { 0, 0 }, win.getLogicalWidth(), HUD_VPORT_H };
    const lx::Graphics::ImgRect& cvport = viewport;
    lx::Graphics::Colour bcolour = { 0, 0, 0, 64 };

    bgm->displayHUD();
    win.setViewPort( viewport );
    win.setDrawColour( bcolour );
    win.fillRect( cvport );

    std::for_each( huds.begin(), huds.end(), []( HUD * hud )
    {
        hud->displayHUD();
    } );
    win.resetViewPort();
}

void HudHandler::clearHUDs()
{
    huds.clear();
}


// Enemy HUD
EnemyHUD::EnemyHUD( Enemy& e )
    : enemy( e ), gauge( nullptr ), grad( nullptr ), nb_graduation( e.getWidth() ),
      grad_max( e.getWidth() - 1 )
{
    const ResourceManager * const rc = ResourceManager::getInstance();
    gauge = rc->getMenuResource( BOSS_RC_GAUGE );
    grad = rc->getMenuResource( BOSS_RC_GRAD );
}


void EnemyHUD::displayGauge()
{
    lx::Graphics::ImgRect egrad =
    {
        { ICAST( enemy.getX() ), ICAST( enemy.getY() ) - ENEMY_GRAD_H },
        1, ENEMY_GRAD_H
    };

    _displayGauge( ICAST( enemy.getX() ), egrad );
}

void EnemyHUD::_displayGauge( int x, lx::Graphics::ImgRect& rect )
{
    for ( unsigned int i = 1; i <= nb_graduation; i++ )
    {
        rect.p.x = x + ( ICAST( i ) + 1 ) * 1;
        grad->draw( rect );
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
    lx::Graphics::ImgRect bgauge = { ICAST( enemy.getX() ),
                                     ICAST( enemy.getY() ) - ENEMY_GRAD_H,
                                     enemy.getWidth(), ENEMY_GRAD_H
                                   };
    gauge->draw( bgauge );
    displayGauge();
}



// HUD of any boss/semi-boss
BossHUD::BossHUD( Boss& b )
    : EnemyHUD( b ), boss( b ), filled( false ), fill_level( 1 )
{
    nb_graduation = BOSS_GRAD_MAX;
    grad_max = BOSS_GRAD_MAX;
}

void BossHUD::fillGauge()
{
    for ( unsigned int j = 1; j < fill_level; j++ )
    {
        bgrad.p.x = BOSS_HUD_XPOS + BOSS_HUD_OFFSET + ICAST( j ) * BOSS_GRAD_W;
        grad->draw( bgrad );
    }

    if ( fill_level >= nb_graduation )
        filled = true;
    else
        fill_level += FILL_STEP;
}


void BossHUD::displayGauge()
{
    _displayGauge( BOSS_HUD_XPOS + BOSS_HUD_OFFSET, bgrad );
}

void BossHUD::displayHUD()
{
    lx::Graphics::ImgRect bgauge = { BOSS_HUD_XPOS, BOSS_HUD_YPOS,
                                     BOSS_HUD_W, BOSS_HUD_H
                                   };

    gauge->draw( bgauge );

    if ( !filled )
        fillGauge();
    else
        displayGauge();
}


// HUD of the player
PlayerHUD::PlayerHUD( Player& sub )
    : subject( sub ), player_hp( sub.getHP() ), player_hp_max( sub.getHP() ),
      player_rockets( sub.getRocket() ), player_bombs( sub.getBomb() ),
      hud_font( nullptr ), health_symbol( nullptr ), missile_symbol( nullptr ),
      bomb_symbol( nullptr ), hp_val_tx( nullptr ), missile_val_tx( nullptr ),
      bomb_val_tx( nullptr )
{
    const TX_Asset * const asset = TX_Asset::getInstance();
    Window& win = WindowManager::getInstance().getWindow( WinID::getWinID() );
    hud_font = new lx::TrueTypeFont::Font( asset->getFontFile(), PLAYER_HUD_WHITE_COLOUR, PLAYER_HUD_SIZE );

    // Labels
    health_symbol = new lx::Graphics::Sprite( asset->getItemFile( HEALTH_SPID ), win );
    missile_symbol = ResourceManager::getInstance()->getResource( RC_MISSILE, ROCKET_SPID );
    bomb_symbol = ResourceManager::getInstance()->getResource( RC_MISSILE, BOMB_SPID );

    // Values
    hp_val_tx = new lx::Graphics::BlendedTextTexture( *hud_font, win );
    missile_val_tx = new lx::Graphics::BlendedTextTexture( *hud_font, win );
    bomb_val_tx = new lx::Graphics::BlendedTextTexture( *hud_font, win );
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
    hp_val_tx->setText( misc::to_string( player_hp ) );
    missile_val_tx->setText( misc::to_string( player_rockets ) );
    bomb_val_tx->setText( misc::to_string( player_bombs ) );

    // Position + Display
    drawHealth();
    drawMissile();
    if ( Level::getLevelNum() >= Level::BOMB_LEVEL_MIN )
        drawBomb();
}

void PlayerHUD::drawHealth()
{
    health_symbol->draw( rhealth_sym );
    hp_val_tx->setPosition( rhealth_sym.p.x + rhealth_sym.w, 1 );
    hp_val_tx->draw();
}

void PlayerHUD::drawMissile()
{
    missile_symbol->draw( rrocket_sym );
    missile_val_tx->setPosition( rrocket_sym.p.x + rrocket_sym.w, 1 );
    missile_val_tx->draw();
}

void PlayerHUD::drawBomb()
{
    bomb_symbol->draw( rbomb_sym );
    bomb_val_tx->setPosition( rbomb_sym.p.x + rbomb_sym.w, 1 );
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


BGM::BGM( unsigned int lvl ) : timer(), tag( nullptr ), bgm_font( nullptr ), bgm_tx( nullptr )
{
    const TX_Asset * const a  = TX_Asset::getInstance();

    Window& w = WindowManager::getInstance().getWindow( WinID::getWinID() );
    bgm_font  = new lx::TrueTypeFont::Font( a->getFontFile(), BGM_DCOLOUR, BGM_SIZE );
    bgm_tx    = new lx::Graphics::BlendedTextTexture( *bgm_font, w );
    tag       = new libtagpp::Tag();
    tag->readTag( a->getLevelMusic( lvl ) );
    BGM::update();
}


void BGM::update()
{
    const int W = Engine::getInstance()->getMaxXlim();
    const int H = Engine::getInstance()->getMaxYlim();
    UTF8string _artist( tag->artist() );
    UTF8string _title( tag->title() );

    UTF8string bgm_text( "BGM. " + _artist + " - " + _title + " " );
    bgm_tx->setText( bgm_text );

    int w, h;
    std::tie( w, h ) = bgm_tx->getTextDimension();
    bgm_tx->setPosition( ImgCoord{ W - w, H - h } );
    timer.lap();
}

void BGM::displayHUD()
{
    if ( timer.getTicks() < BGM_DELAY )
        bgm_tx->draw();
}

BGM::~BGM()
{
    delete tag;
    delete bgm_tx;
    delete bgm_font;
}
