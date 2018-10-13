
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

#include "GUI.hpp"
#include "../option/GamepadControl.hpp"
#include "../resources/ResourceManager.hpp"
#include "../utils/cppgettext/cppgettext.h"
#include "../option/OptionHandler.hpp"
#include "../option/GamepadControl.hpp"
#include "../asset/TX_Asset.hpp"
#include "../level/Level.hpp"
#include "../utils/misc.hpp"

#include <Lunatix/ImgRect.hpp>
#include <Lunatix/Chunk.hpp>
#include <Lunatix/Texture.hpp>
#include <Lunatix/Window.hpp>
#include <Lunatix/Random.hpp>
#include <Lunatix/TrueTypeFont.hpp>
#include <Lunatix/Hitbox.hpp>
#include <Lunatix/Text.hpp>
#include <Lunatix/Log.hpp>

#include <sstream>
#include <cstdlib>

#define _(msg) cppgettext(msg)

using namespace lx::Graphics;
using namespace lx::Random;

namespace
{
/// ID of resources
const unsigned int GUI_BUTTON_ID = 0;
const unsigned int GUI_BUTTON_HOVER_ID = 1;
const unsigned int GUI_ARROW_ID = 2;
const unsigned int GUI_ARROW_HOVER_ID = 3;
const unsigned int GUI_XBOX_ID = 6;
const unsigned int GUI_CONTROL_ID = 7;

/// Colour
const lx::Graphics::Colour GUI_BLACK_COLOUR  = {0, 0, 0, 240};
const lx::Graphics::Colour GUI_BLACKA_COLOUR = {0, 0, 0, 192};
const lx::Graphics::Colour GUI_WHITE_COLOUR  = {255, 255, 255, 240};
const lx::Graphics::Colour GUI_YELLOW_COLOUR = {0xc0, 0xc6, 0x24, 127};
const lx::Graphics::Colour GUI_GREEN_COLOUR  = {0xa1, 0xf9, 0x83, 127};

/// Strings
const std::string TITLE( "Target Xplosion" );
const std::string PLAY( "Play" );
const std::string OPTION( "Options" );
const std::string QUIT( "Quit" );
const std::string OVERALL_VOLUME( "Sound volume" );
const std::string MUSIC_VOLUME( "Music volume" );
const std::string FX_VOLUME( "SFX volume" );
const std::string FULLSCREEN( "Fullscreen" );
const std::string GAMEPAD( "Gamepad" );
const std::string BACK( "Back" );

const std::string SHOT( "Shot" );
const std::string ROCKET( "Rocket" );
const std::string BOMB( "Bomb" );
const std::string SMODE( "Slow mode" );


/// MainGUI
const unsigned int GUI_TITLE_SZ = 128;
const int GUI_TITLE_XPOS = 192;
const int GUI_TITLE_YPOS = 12;

const unsigned int GUI_SELECT_SZ = 64;
const int GUI_PLAY_XPOS = 160;
const int GUI_PLAY_YPOS = 304;

const int GUI_OPT_XPOS  = 160;
const int GUI_OPT_YPOS  = 454;
const int GUI_QUIT_XPOS = 160;
const int GUI_QUIT_YPOS = 604;

// Box of main menu buttons
ImgRect play_box = {0, 300, 527, 96};
ImgRect opt_box  = {0, 450, 677, 96};
ImgRect quit_box = {0, 600, 827, 96};

ImgRect main_play_box = {0, 300, 427, 96};
ImgRect main_opt_box  = {0, 450, 427, 96};
ImgRect main_quit_box = {0, 600, 427, 96};

ImgRect aux1_box = {100, 300, 427, 96};
ImgRect aux2_box = {250, 450, 427, 96};
ImgRect aux3_box = {250, 600, 427, 96};
ImgRect aux4_box = {400, 600, 427, 96};

ImgRect control_box = {800, 300, 435, 192};

/// OptionGUI
const unsigned int VOL_SZ = 60;
const int OPT_XPOS = 64;
const int OPT_OV_YPOS = 160;
const int OPT_MUSIC_YPOS = OPT_OV_YPOS + 70;
const int OPT_FX_YPOS = OPT_MUSIC_YPOS + 70;
const int OPT_FULLSCREEN_YPOS = OPT_FX_YPOS + 108;

const int OPT_AROW_OV_YPOS = 180;
const int OPT_AROW_MU_YPOS = OPT_AROW_OV_YPOS + 70;
const int OPT_AROW_FX_YPOS = OPT_AROW_MU_YPOS + 70;

const unsigned int OPT_SZ    = 64;
const int OPT_TEXT_XPOS      = 64;
const int OPT_TEXT_GP_YPOS   = 499;
const int OPT_TEXT_BACK_YPOS = 620;

const int OPT_YOFF = 4;

ImgRect gp_box       = {0, 498, 427, 80};
ImgRect back_box     = {0, 620, 427, 80};
ImgRect aux_gp_box   = {64, 498, 427, 80};
ImgRect aux_back_box = {224, 620, 427, 80};

ImgRect option_gp_box   = {0, 498, 448, 80};
ImgRect option_back_box = {0, 620, 600, 80};

ImgRect option_ovd_box = {586, OPT_AROW_OV_YPOS, 90, 48};
ImgRect option_ovu_box = {768, OPT_AROW_OV_YPOS, 90, 48};
ImgRect option_mud_box = {586, OPT_AROW_MU_YPOS, 90, 48};
ImgRect option_muu_box = {768, OPT_AROW_MU_YPOS, 90, 48};
ImgRect option_fxd_box = {586, OPT_AROW_FX_YPOS, 90, 48};
ImgRect option_fxu_box = {768, OPT_AROW_FX_YPOS, 90, 48};
ImgRect option_fullscreen_box = {624, OPT_FULLSCREEN_YPOS, 256, 48};

// text box
ImgRect option_oval_box;
ImgRect option_mval_box;
ImgRect option_fxval_box;

/// Gamepad
const int GP_OPT_XPOS = OPT_XPOS;
const int GP_VOPT_XPOS = GP_OPT_XPOS + 512;
const int GP_SHOT_YPOS = OPT_OV_YPOS;
const int GP_ROCK_YPOS = GP_SHOT_YPOS + 80;
const int GP_BOMB_YPOS = GP_ROCK_YPOS + 80;
const int GP_SMODE_YPOS = GP_BOMB_YPOS + 80;
const int GP_VALUE_OFFSET = 90;

const int GP_VALUE_W = 128;
// The height depends on the text, see GamepadGUI ::position()


/* OptionMenuCallback */
const int OPT_BASE = 10;
const unsigned short OPT_MAX_VOLUME = 100;
const unsigned int SOUND_EXPLOSION_ID = 3;

// Convert a string to a number
inline unsigned short toNumber( const UTF8string& u8str ) noexcept
{
    return static_cast<unsigned short>( std::strtol( u8str.utf8_str(), nullptr, OPT_BASE ) );
}

// Check if the string is a number
inline bool isNumber( const std::string& str ) noexcept
{
    if ( str.empty() || ( ( !std::isdigit( str[0] ) ) && ( str[0] != '-' ) && ( str[0] != '+' ) ) )
        return false;

    char * p;
    std::strtol( str.c_str(), &p, OPT_BASE );
    return *p == 0;
}

// Set the text volume text properly
inline UTF8string transformString( const UTF8string& u8str ) noexcept
{
    auto i = toNumber( u8str );
    return UTF8string( misc::to_string( i > OPT_MAX_VOLUME ? OPT_MAX_VOLUME : i ) );
}



class OptionMenuCallback final: public lx::Text::RedrawCallback
{
    lx::Win::Window& _w;
    lx::Graphics::TextTexture& _t;
    OptionGUI& gui;
    Option::OptionHandler& opt;
    GUI_Button_State st;
    UTF8string u8number;

    OptionMenuCallback( const OptionMenuCallback& ) = delete;
    OptionMenuCallback( const OptionMenuCallback&& ) = delete;
    OptionMenuCallback& operator =( const OptionMenuCallback& ) = delete;
    OptionMenuCallback& operator =( const OptionMenuCallback&& ) = delete;

public:

    OptionMenuCallback( lx::Win::Window& win, lx::Graphics::TextTexture& texture,
                        OptionGUI& o, Option::OptionHandler& hdl,
                        GUI_Button_State s )
        : _w( win ), _t( texture ), gui( o ), opt( hdl ), st( s ), u8number() {}

    void operator ()( UTF8string& u8str, UTF8string&, const bool update,
                      size_t cursor, size_t prev_cur ) noexcept override
    {
        if ( update )
        {
            if ( cursor == prev_cur - 1 )
            {
                if ( !u8number.utf8_empty() )
                    u8number.utf8_pop();
            }
            else if ( !u8str.utf8_empty() && cursor == u8str.utf8_length() )
            {
                std::string s = u8str.utf8_at( u8str.utf8_length() - 1 );

                if ( isNumber( s ) )
                    u8number += s;
            }

            if ( !u8number.utf8_empty() )
            {
                if ( st == OV_TEXT_CLICK )
                    opt.setOverallVolume( toNumber( u8number ) );

                else if ( st == MU_TEXT_CLICK )
                    opt.setMusicVolume( toNumber( u8number ) );

                else if ( st == FX_TEXT_CLICK )
                {
                    opt.setFXVolume( toNumber( u8number ) );
                    gui.playSound();
                }
            }

            _t.setText( transformString( u8number ) );
            gui.draw();
        }
    }

    ~OptionMenuCallback() = default;
};

}


/** GUI */

unsigned int GUI::gui_bgid = 0;

GUI::GUI( lx::Win::Window& w )
    : win( w ), f( nullptr ), title_text( nullptr ), bg( nullptr ),
      state( UNDEF_GUI ), bstate( NORMAL ) {}

GUI::~GUI()
{
    delete title_text;
    delete f;
}


/** Main GUI */

MainGUI::MainGUI( lx::Win::Window& w )
    : GUI( w ), title_font( nullptr ), button_play( nullptr ), button_option( nullptr ),
      button_quit( nullptr ), img_control( nullptr ), play_text( nullptr ),
      option_text( nullptr ), quit_text( nullptr )
{
    state = MAIN_GUI;
    const TX_Asset * const a = TX_Asset::getInstance();
    const ResourceManager * rc = ResourceManager::getInstance();
    gui_bgid = lx::Random::xrand( 1U, Level::MAX_LEVEL + 1U );

    bg = new Sprite( a->getLevelBg( gui_bgid ), w );
    f = new lx::TrueTypeFont::Font( a->getFontFile(), GUI_BLACK_COLOUR, GUI_SELECT_SZ );
    title_font = new lx::TrueTypeFont::Font( a->getFontFile(), GUI_WHITE_COLOUR, GUI_TITLE_SZ );

    // Sprites
    img_control   = rc->getMenuResource( GUI_CONTROL_ID );
    Sprite * s  = rc->getMenuResource( GUI_BUTTON_ID );
    button_play   = s;
    button_option = s;
    button_quit   = s;

    // Background
    title_text = new lx::Graphics::BlendedTextTexture( TITLE, *title_font, win );
    title_text->setPosition( GUI_TITLE_XPOS, GUI_TITLE_YPOS );

    // Text
    play_text = new lx::Graphics::BlendedTextTexture( _(PLAY), *f, win );
    option_text = new lx::Graphics::BlendedTextTexture( _(OPTION), *f, win );
    quit_text = new lx::Graphics::BlendedTextTexture( _(QUIT), *f, win );

    play_text->setPosition( GUI_PLAY_XPOS, GUI_PLAY_YPOS );
    option_text->setPosition( GUI_OPT_XPOS, GUI_OPT_YPOS );
    quit_text->setPosition( GUI_QUIT_XPOS, GUI_QUIT_YPOS );
}

MainGUI::~MainGUI()
{
    delete quit_text;
    delete option_text;
    delete play_text;
    delete title_font;
    delete bg;
}


void MainGUI::draw() noexcept
{
    win.clearWindow();

    bg->draw();
    title_text->draw();
    img_control->draw( control_box );

    // Button
    button_play->draw( main_play_box );
    button_play->draw( aux1_box );
    button_option->draw( main_opt_box );
    button_option->draw( aux2_box );
    button_quit->draw( main_quit_box );
    button_quit->draw( aux3_box );
    button_quit->draw( aux4_box );

    // Text
    play_text->draw();
    option_text->draw();
    quit_text->draw();

    win.update();
}


void MainGUI::setState( GUI_State st ) noexcept
{
    state = st;
}

void MainGUI::setButtonState( GUI_Button_State st ) noexcept
{
    bstate = st;
    ResourceManager * rc = ResourceManager::getInstance();
    Sprite * b = rc->getMenuResource( GUI_BUTTON_ID );
    Sprite * bhover = rc->getMenuResource( GUI_BUTTON_HOVER_ID );

    switch ( bstate )
    {
    case PLAY_BUTTON_HOVER:
        button_play = bhover;
        button_option = b;
        button_quit = b;
        break;

    case OPT_BUTTON_HOVER:
        button_play = b;
        button_option = bhover;
        button_quit = b;
        break;

    case QUIT_BUTTON_HOVER:
        button_play = b;
        button_option = b;
        button_quit = bhover;
        break;

    default:
        button_play = b;
        button_option = b;
        button_quit = b;
        break;
    }
}

void MainGUI::getAABBs( lx::Physics::FloatingBox * rects ) noexcept
{
    using lx::Physics::toFloatingBox;
    if ( rects != nullptr )
    {
        rects[0] = toFloatingBox( play_box );
        rects[1] = toFloatingBox( opt_box );
        rects[2] = toFloatingBox( quit_box );
    }
}


/** OptionGUI */

OptionGUI::OptionGUI( lx::Win::Window& w, const Option::OptionHandler& opt )
    : GUI( w ), text_font( nullptr ), ov_volume_text( nullptr ),
      ov_volume_vtext( nullptr ), button_ov_down( nullptr ), button_ov_up( nullptr ),
      music_volume_text( nullptr ), music_volume_vtext( nullptr ),
      button_music_down( nullptr ), button_music_up( nullptr ),
      fx_volume_text( nullptr ), fx_volume_vtext( nullptr ),
      button_fx_down( nullptr ), button_fx_up( nullptr ),
      fullscreen_text( nullptr ), fullscreen_vtext( nullptr ),
      gp_text( nullptr ), button_gp( nullptr ), return_text( nullptr ),
      button_back( nullptr ), esc_text( nullptr ), vsound( nullptr )
{
    state = MAIN_GUI;

    const ResourceManager * rc = ResourceManager::getInstance();
    Sprite * s = rc->getMenuResource( GUI_BUTTON_ID );
    Sprite * ars = rc->getMenuResource( GUI_ARROW_ID );
    const std::string& ffile = TX_Asset::getInstance()->getFontFile();

    // Sound for volume
    vsound = new lx::Mixer::Chunk( TX_Asset::getInstance()->getSound( SOUND_EXPLOSION_ID ) );
    bg = new Sprite( TX_Asset::getInstance()->getLevelBg( gui_bgid ), w );
    f = new lx::TrueTypeFont::Font( ffile, GUI_WHITE_COLOUR, VOL_SZ );
    text_font = new lx::TrueTypeFont::Font( ffile, GUI_BLACK_COLOUR, OPT_SZ );

    /// Labels
    title_text = new lx::Graphics::BlendedTextTexture( _(OPTION), GUI_TITLE_SZ, *f, win );
    ov_volume_text = new lx::Graphics::BlendedTextTexture( _(OVERALL_VOLUME), *f, win );
    music_volume_text = new lx::Graphics::BlendedTextTexture( _(MUSIC_VOLUME), *f, win );
    fx_volume_text = new lx::Graphics::BlendedTextTexture( _(FX_VOLUME), *f, win );
    fullscreen_text = new lx::Graphics::BlendedTextTexture( _(FULLSCREEN), *f, win );
    gp_text = new lx::Graphics::BlendedTextTexture( _(GAMEPAD), *text_font, win );
    return_text = new lx::Graphics::BlendedTextTexture( _(BACK), *text_font, win );

    /// Values
    ov_volume_vtext = new lx::Graphics::ShadedTextTexture( opt.stringOfOverallVolume(), *f, GUI_BLACKA_COLOUR, win );
    music_volume_vtext = new lx::Graphics::ShadedTextTexture( opt.stringOfMusicVolume(), *f, GUI_BLACKA_COLOUR, win );
    fx_volume_vtext = new lx::Graphics::ShadedTextTexture( opt.stringOfFXVolume(), *f, GUI_BLACKA_COLOUR, win );
    fullscreen_vtext = new lx::Graphics::ShadedTextTexture( opt.stringOfFullscreenFlag(), *f, GUI_BLACKA_COLOUR, win );

    // Set the position of the textures and set the buttons
    position();
    button_gp = s;
    button_back = s;
    button_ov_down = ars;
    button_ov_up = ars;
    button_music_down = ars;
    button_music_up = ars;
    button_fx_down = ars;
    button_fx_up = ars;

    // Set the boxes of the values
    int h = ov_volume_vtext->getTextHeight();
    option_oval_box  = {option_ovd_box.p.x + option_ovd_box.w, option_ovd_box.p.y - OPT_YOFF,
                        option_ovu_box.p.x - option_ovd_box.p.x, h
                       };
    option_mval_box  = {option_mud_box.p.x + option_mud_box.w, option_mud_box.p.y - OPT_YOFF,
                        option_muu_box.p.x - option_mud_box.p.x, h
                       };
    option_fxval_box = {option_fxd_box.p.x + option_fxd_box.w, option_fxd_box.p.y - OPT_YOFF,
                        option_fxu_box.p.x - option_fxd_box.p.x, h
                       };
}

void OptionGUI::position() noexcept
{
    title_text->setPosition( GUI_TITLE_XPOS, GUI_TITLE_YPOS );
    ov_volume_text->setPosition( OPT_XPOS, OPT_OV_YPOS );
    music_volume_text->setPosition( OPT_XPOS, OPT_MUSIC_YPOS );
    fx_volume_text->setPosition( OPT_XPOS, OPT_FX_YPOS );
    fullscreen_text->setPosition( OPT_XPOS, OPT_FULLSCREEN_YPOS );
    gp_text->setPosition( OPT_TEXT_XPOS, OPT_TEXT_GP_YPOS );
    return_text->setPosition( OPT_TEXT_XPOS, OPT_TEXT_BACK_YPOS );

    ov_volume_vtext->setPosition( option_ovd_box.p.x + option_ovd_box.w,
                                  option_ovd_box.p.y - OPT_YOFF );

    music_volume_vtext->setPosition( option_mud_box.p.x + option_mud_box.w,
                                     option_mud_box.p.y - OPT_YOFF );

    fx_volume_vtext->setPosition( option_fxd_box.p.x + option_fxd_box.w,
                                  option_fxd_box.p.y - OPT_YOFF );

    fullscreen_vtext->setPosition( option_fullscreen_box.p.x,
                                   option_fullscreen_box.p.y - OPT_YOFF );
}

void OptionGUI::draw() noexcept
{
    win.clearWindow();

    bg->draw();
    title_text->draw();
    ov_volume_text->draw();
    fx_volume_text->draw();
    music_volume_text->draw();
    fullscreen_text->draw();
    fullscreen_vtext->draw();

    button_ov_down->draw( option_ovd_box, 0.0, lx::Graphics::MirrorEffect::HORIZONTAL );
    ov_volume_vtext->draw();
    button_ov_up->draw( option_ovu_box );

    button_music_down->draw( option_mud_box, 0.0, lx::Graphics::MirrorEffect::HORIZONTAL );
    music_volume_vtext->draw();
    button_music_up->draw( option_muu_box );

    button_fx_down->draw( option_fxd_box, 0.0, lx::Graphics::MirrorEffect::HORIZONTAL );
    fx_volume_vtext->draw();
    button_fx_up->draw( option_fxu_box );

    button_gp->draw( gp_box );
    button_gp->draw( aux_gp_box );
    button_back->draw( back_box );
    button_back->draw( aux_back_box );

    gp_text->draw();
    return_text->draw();

    if ( esc_text != nullptr )
        esc_text->draw();

    win.update();
}

void OptionGUI::playSound() noexcept
{
    vsound->play();
}

void OptionGUI::setButtonState( GUI_Button_State st ) noexcept
{
    bstate = st;
    ResourceManager * rc = ResourceManager::getInstance();
    Sprite * opt = rc->getMenuResource( GUI_BUTTON_ID );
    Sprite * a = rc->getMenuResource( GUI_ARROW_ID );
    Sprite * a_hover = rc->getMenuResource( GUI_ARROW_HOVER_ID );
    Sprite * opt_hover = rc->getMenuResource( GUI_BUTTON_HOVER_ID );

    button_gp = opt;
    button_back = opt;
    button_ov_down = a;
    button_ov_up = a;
    button_music_down = a;
    button_music_up = a;
    button_fx_down = a;
    button_fx_up = a;
    fullscreen_vtext->setTextColour( GUI_WHITE_COLOUR );
    fullscreen_vtext->setBgColour( GUI_BLACKA_COLOUR );

    switch ( bstate )
    {
    case GP_BUTTON_HOVER:
        button_gp = opt_hover;
        break;

    case BACK_BUTTON_HOVER:
        button_back = opt_hover;
        break;

    case OVD_BUTTON_HOVER:
        button_ov_down = a_hover;
        break;

    case OVU_BUTTON_HOVER:
        button_ov_up = a_hover;
        break;

    case MUD_BUTTON_HOVER:
        button_music_down = a_hover;
        break;

    case MUU_BUTTON_HOVER:
        button_music_up = a_hover;
        break;

    case FXD_BUTTON_HOVER:
        button_fx_down = a_hover;
        break;

    case FXU_BUTTON_HOVER:
        button_fx_up = a_hover;
        break;

    case FS_BUTTON_HOVER:
        fullscreen_vtext->setTextColour( GUI_BLACKA_COLOUR );
        fullscreen_vtext->setBgColour( GUI_WHITE_COLOUR );
        break;

    default:
        break;
    }

    fullscreen_vtext->setText( fullscreen_vtext->getText() );
}


unsigned short OptionGUI::incVolume( unsigned short vol ) noexcept
{
    return ( vol < Option::OPT_MAX_VOLUME ) ? vol + 1 : vol;
}

unsigned short OptionGUI::decVolume( unsigned short vol ) noexcept
{
    return ( vol > 0 ) ? vol - 1 : vol;
}

void OptionGUI::updateTextVolume( GUI_Button_State st, Option::OptionHandler& opt ) noexcept
{
    const std::string ESC( "ESC to cancel" );
    esc_text = new lx::Graphics::BlendedTextTexture( ESC, *f, win );
    lx::Graphics::TextTexture * t = nullptr;

    if ( st == OV_TEXT_CLICK )
    {
        t = ov_volume_vtext;
        esc_text->setPosition( option_ovu_box.p.x + option_ovu_box.w + 1, option_ovu_box.p.y );
    }
    else if ( st == MU_TEXT_CLICK )
    {
        t = music_volume_vtext;
        esc_text->setPosition( option_muu_box.p.x + option_muu_box.w + 1, option_muu_box.p.y );
    }
    else if ( st == FX_TEXT_CLICK )
    {
        t = fx_volume_vtext;
        esc_text->setPosition( option_fxu_box.p.x + option_fxu_box.w + 1, option_fxu_box.p.y );
    }

    // No update if the texture pointer is null
    if ( t == nullptr )
    {
        delete esc_text;
        esc_text = nullptr;
        return;
    }

    // Draw + Text input
    draw();
    OptionMenuCallback clk( win, *t, *this, opt, st );
    lx::Text::TextInput().eventLoop( clk );

    delete esc_text;
    esc_text = nullptr;
}

void OptionGUI::updateVolume( GUI_Button_State st, Option::OptionHandler& opt ) noexcept
{
    bstate = st;

    switch ( bstate )
    {
    case OVD_BUTTON_CLICK:
        opt.setOverallVolume( decVolume( opt.getOverallVolume() ) );
        ov_volume_vtext->setText( opt.stringOfOverallVolume() );
        break;

    case OVU_BUTTON_CLICK:
        opt.setOverallVolume( incVolume( opt.getOverallVolume() ) );
        ov_volume_vtext->setText( opt.stringOfOverallVolume() );
        break;

    case MUD_BUTTON_CLICK:
        opt.setMusicVolume( decVolume( opt.getMusicVolume() ) );
        music_volume_vtext->setText( opt.stringOfMusicVolume() );
        break;

    case MUU_BUTTON_CLICK:
        opt.setMusicVolume( incVolume( opt.getMusicVolume() ) );
        music_volume_vtext->setText( opt.stringOfMusicVolume() );
        break;

    case FXD_BUTTON_CLICK:
        opt.setFXVolume( decVolume( opt.getFXVolume() ) );
        fx_volume_vtext->setText( opt.stringOfFXVolume() );
        vsound->play();
        break;

    case FXU_BUTTON_CLICK:
        opt.setFXVolume( incVolume( opt.getFXVolume() ) );
        fx_volume_vtext->setText( opt.stringOfFXVolume() );
        vsound->play();
        break;

    default:
        break;
    }
}

void OptionGUI::updateFullscreen( GUI_Button_State st, Option::OptionHandler& opt ) noexcept
{
    bstate = st;

    switch ( bstate )
    {
    case FS_BUTTON_CLICK:
        if ( opt.getFullscreenFlag() == static_cast<uint8_t>( 1 ) )
        {
            lx::Log::log( "NORMAL" );
            win.toggleFullscreen( lx::Win::ScreenMode::NO_FULLSCREEN );
            opt.setFullscreenFlag( 0 );
        }
        else
        {
            lx::Log::log( "FULLSCREEN" );
            win.toggleFullscreen( lx::Win::ScreenMode::FULLSCREEN );
            opt.setFullscreenFlag( 1 );
        }

        fullscreen_vtext->setBgColour( GUI_BLACK_COLOUR );
        fullscreen_vtext->setText( opt.stringOfFullscreenFlag() );
        break;

    default:
        break;
    }

    lx::Log::log( "Window: %d x %d", win.getWidth(), win.getHeight() );
    lx::Log::log( "Window logic: %d x %d", win.getLogicalWidth(), win.getLogicalHeight() );
}

void OptionGUI::getAABBs( lx::Physics::FloatingBox * rects ) noexcept
{
    using lx::Physics::toFloatingBox;
    rects[0]  = toFloatingBox( option_ovd_box );
    rects[1]  = toFloatingBox( option_ovu_box );
    rects[2]  = toFloatingBox( option_mud_box );
    rects[3]  = toFloatingBox( option_muu_box );
    rects[4]  = toFloatingBox( option_fxd_box );
    rects[5]  = toFloatingBox( option_fxu_box );
    rects[6]  = toFloatingBox( option_fullscreen_box );
    rects[7]  = toFloatingBox( option_oval_box );
    rects[8]  = toFloatingBox( option_mval_box );
    rects[9]  = toFloatingBox( option_fxval_box );
    rects[10] = toFloatingBox( option_gp_box );
    rects[11] = toFloatingBox( option_back_box );
}


OptionGUI::~OptionGUI()
{
    delete vsound;
    delete return_text;
    delete gp_text;
    delete fullscreen_vtext;
    delete fullscreen_text;
    delete fx_volume_vtext;
    delete fx_volume_text;
    delete music_volume_vtext;
    delete music_volume_text;
    delete ov_volume_vtext;
    delete ov_volume_text;
    delete text_font;
    delete bg;
}


/** Gamepad GUI */

GamepadGUI::GamepadGUI( lx::Win::Window& w ): GUI( w ), text_font( nullptr ),
    gp_text( nullptr ), back_text( nullptr ),
    shot_text( nullptr ), shot_vtext( nullptr ),
    rocket_text( nullptr ), rocket_vtext( nullptr ),
    bomb_text( nullptr ), bomb_vtext( nullptr ),
    smode_text( nullptr ), smode_vtext( nullptr ),
    button_back( nullptr ), xbox( nullptr ), colour( GUI_BLACK_COLOUR ),
    BHOVER_COLOUR( GUI_YELLOW_COLOUR ), BCLICK_COLOUR( GUI_GREEN_COLOUR )
{
    const lx::Graphics::Colour WCOLOUR = {255, 255, 255, 128};
    const lx::Graphics::Colour BCOLOUR = {0, 0, 0, 255};
    const ResourceManager * const rc = ResourceManager::getInstance();
    const std::string& fname = TX_Asset::getInstance()->getFontFile();

    button_back = rc->getMenuResource( GUI_BUTTON_ID );
    xbox = rc->getMenuResource( GUI_XBOX_ID );

    bg = new Sprite( TX_Asset::getInstance()->getLevelBg( gui_bgid ), w );
    text_font = new lx::TrueTypeFont::Font( fname, GUI_WHITE_COLOUR, OPT_SZ );
    gp_text = new lx::Graphics::BlendedTextTexture( GAMEPAD, GUI_TITLE_SZ, *text_font, win );
    back_text = new lx::Graphics::BlendedTextTexture( BACK, *text_font, win );

    GPconfig::GamepadControl gpcontrol;
    const UTF8string SHOT_U8STR  = gpcontrol.getControl( GPconfig::ActionControl::SHOT );
    const UTF8string ROCK_U8STR  = gpcontrol.getControl( GPconfig::ActionControl::ROCKET );
    const UTF8string BOMB_U8STR  = gpcontrol.getControl( GPconfig::ActionControl::BOMB );
    const UTF8string SMODE_U8STR = gpcontrol.getControl( GPconfig::ActionControl::SLOW );

    shot_text    = new lx::Graphics::BlendedTextTexture( SHOT, *text_font, win );
    shot_vtext   = new lx::Graphics::ShadedTextTexture( SHOT_U8STR, *text_font, GUI_BLACKA_COLOUR, win );
    rocket_text  = new lx::Graphics::BlendedTextTexture( ROCKET, *text_font, win );
    rocket_vtext = new lx::Graphics::ShadedTextTexture( ROCK_U8STR, *text_font, GUI_BLACKA_COLOUR, win );
    bomb_text    = new lx::Graphics::BlendedTextTexture( BOMB, *text_font, win );
    bomb_vtext   = new lx::Graphics::ShadedTextTexture( BOMB_U8STR, *text_font, GUI_BLACKA_COLOUR, win );
    smode_text   = new lx::Graphics::BlendedTextTexture( SMODE, *text_font, win );
    smode_vtext  = new lx::Graphics::ShadedTextTexture( SMODE_U8STR, *text_font, GUI_BLACKA_COLOUR, win );

    back_text->setTextColour( BCOLOUR );
    win.getDrawColour( colour );
    win.setDrawColour( WCOLOUR );
    position();
}

void GamepadGUI::updateGamepadGUI() noexcept
{
    GPconfig::GamepadControl gpcontrol;
    const UTF8string SHOT_U8STR  = gpcontrol.getControl( GPconfig::ActionControl::SHOT );
    const UTF8string ROCK_U8STR  = gpcontrol.getControl( GPconfig::ActionControl::ROCKET );
    const UTF8string BOMB_U8STR  = gpcontrol.getControl( GPconfig::ActionControl::BOMB );
    const UTF8string SMODE_U8STR = gpcontrol.getControl( GPconfig::ActionControl::SLOW );

    shot_vtext->setText( SHOT_U8STR );
    rocket_vtext->setText( ROCK_U8STR );
    bomb_vtext->setText( BOMB_U8STR );
    smode_vtext->setText( SMODE_U8STR );

    shot_vtext->setBgColour( GUI_BLACKA_COLOUR );
    rocket_vtext->setBgColour( GUI_BLACKA_COLOUR );
    bomb_vtext->setBgColour( GUI_BLACKA_COLOUR );
    smode_vtext->setBgColour( GUI_BLACKA_COLOUR );
}

void GamepadGUI::position() noexcept
{
    gp_text->setPosition( GUI_TITLE_XPOS, GUI_TITLE_YPOS );

    shot_text->setPosition( GP_OPT_XPOS, GP_SHOT_YPOS );
    rocket_text->setPosition( GP_OPT_XPOS, GP_ROCK_YPOS );
    bomb_text->setPosition( GP_OPT_XPOS, GP_BOMB_YPOS );
    smode_text->setPosition( GP_OPT_XPOS, GP_SMODE_YPOS );

    shot_vtext->setPosition( GP_VOPT_XPOS, GP_SHOT_YPOS );
    rocket_vtext->setPosition( GP_VOPT_XPOS, GP_ROCK_YPOS );
    bomb_vtext->setPosition( GP_VOPT_XPOS, GP_BOMB_YPOS );
    smode_vtext->setPosition( GP_VOPT_XPOS, GP_SMODE_YPOS );

    back_text->setPosition( OPT_TEXT_XPOS, OPT_TEXT_BACK_YPOS );
}

void GamepadGUI::draw() noexcept
{
    win.clearWindow();
    bg->draw();

    shot_text->draw();
    rocket_text->draw();
    bomb_text->draw();
    smode_text->draw();
    shot_vtext->draw();
    rocket_vtext->draw();
    bomb_vtext->draw();
    smode_vtext->draw();

    button_back->draw( back_box );
    button_back->draw( aux_back_box );
    gp_text->draw();
    back_text->draw();

    win.update();
}

void GamepadGUI::setButtonState( GUI_Button_State st ) noexcept
{
    const ResourceManager * const rc = ResourceManager::getInstance();
    Sprite * gp_button = rc->getMenuResource( GUI_BUTTON_ID );
    Sprite * gp_hover  = rc->getMenuResource( GUI_BUTTON_HOVER_ID );

    button_back = gp_button;

    switch ( st )
    {
    case BACK_BUTTON_HOVER:
        this->button_back = gp_hover;

    case GP_CMD_CHANGE:
        updateGamepadGUI();
        break;

    case GP_SHOT_HOVER:
        shot_vtext->setBgColour( BHOVER_COLOUR );
        break;

    case GP_ROCKET_HOVER:
        rocket_vtext->setBgColour( BHOVER_COLOUR );
        break;

    case GP_BOMB_HOVER:
        bomb_vtext->setBgColour( BHOVER_COLOUR );
        break;

    case GP_SMODE_HOVER:
        smode_vtext->setBgColour( BHOVER_COLOUR );
        break;

    case GP_SHOT_CLICK:
        shot_vtext->setBgColour( BCLICK_COLOUR );
        break;

    case GP_ROCKET_CLICK:
        rocket_vtext->setBgColour( BCLICK_COLOUR );
        break;

    case GP_BOMB_CLICK:
        bomb_vtext->setBgColour( BCLICK_COLOUR );
        break;

    case GP_SMODE_CLICK:
        smode_vtext->setBgColour( BCLICK_COLOUR );
        break;

    default:
        shot_vtext->setBgColour( GUI_BLACKA_COLOUR );
        rocket_vtext->setBgColour( GUI_BLACKA_COLOUR );
        bomb_vtext->setBgColour( GUI_BLACKA_COLOUR );
        smode_vtext->setBgColour( GUI_BLACKA_COLOUR );
        break;
    }
}

void GamepadGUI::getAABBs( lx::Physics::FloatingBox * rects ) noexcept
{
    using lx::Physics::toFloatingBox;
    using lx::Graphics::ImgRect;

    rects[0] = toFloatingBox( option_back_box );
    rects[1] = toFloatingBox( ImgRect{GP_VOPT_XPOS, GP_SHOT_YPOS,
                                      shot_vtext->getTextWidth(),
                                      shot_vtext->getTextHeight()} );

    rects[2] = toFloatingBox( ImgRect{GP_VOPT_XPOS, GP_ROCK_YPOS,
                                      rocket_vtext->getTextWidth(),
                                      rocket_vtext->getTextHeight()} );

    rects[3] = toFloatingBox( ImgRect{GP_VOPT_XPOS, GP_BOMB_YPOS,
                                      bomb_vtext->getTextWidth(),
                                      bomb_vtext->getTextHeight()} );

    rects[4] = toFloatingBox( ImgRect{GP_VOPT_XPOS, GP_SMODE_YPOS,
                                      smode_vtext->getTextWidth(),
                                      smode_vtext->getTextHeight()} );
}

GamepadGUI::~GamepadGUI()
{
    win.setDrawColour( colour );

    delete smode_vtext;
    delete smode_text;
    delete bomb_vtext;
    delete bomb_text;
    delete rocket_vtext;
    delete rocket_text;
    delete shot_vtext;
    delete shot_text;
    delete text_font;
    delete back_text;
    delete gp_text;
    delete bg;
}
