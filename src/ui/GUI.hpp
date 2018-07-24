
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
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/


#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include <Lunatix/ImgRect.hpp>
#include <Lunatix/Colour.hpp>


enum GUI_State : short {UNDEF_GUI, MAIN_GUI, PLAY_GUI};
enum GUI_Button_State : short {NORMAL,
                               /* States in the main menu */
                               PLAY_BUTTON_HOVER, OPT_BUTTON_HOVER, QUIT_BUTTON_HOVER,
                               /* States in the option menu */
                               GP_BUTTON_HOVER, BACK_BUTTON_HOVER, OVD_BUTTON_HOVER,
                               OVU_BUTTON_HOVER, MUD_BUTTON_HOVER, MUU_BUTTON_HOVER,
                               FXD_BUTTON_HOVER, FXU_BUTTON_HOVER,
                               OVD_BUTTON_CLICK, OVU_BUTTON_CLICK,
                               MUD_BUTTON_CLICK, MUU_BUTTON_CLICK,
                               FXD_BUTTON_CLICK, FXU_BUTTON_CLICK,
                               FS_BUTTON_HOVER, FS_BUTTON_CLICK,
                               OV_TEXT_CLICK, MU_TEXT_CLICK, FX_TEXT_CLICK,
                               /* Gamapad */
                               GP_CMD_CHANGE, GP_SHOT_HOVER, GP_ROCKET_HOVER,
                               GP_BOMB_HOVER, GP_SMODE_HOVER,
                               GP_SHOT_CLICK, GP_ROCKET_CLICK,
                               GP_BOMB_CLICK, GP_SMODE_CLICK
                              };

namespace lx
{

namespace Win
{
class Window;
}

namespace Graphics
{
class Texture;
class Sprite;
class TextTexture;
class ShadedTextTexture;
}

namespace TrueTypeFont
{
class Font;
}

namespace Mixer
{
class Chunk;
}

}

namespace Option
{
class OptionHandler;
}


class GUI
{
    GUI( const GUI& ) = delete;
    GUI( const GUI&& ) = delete;
    GUI& operator =( const GUI& ) = delete;
    GUI& operator =( const GUI&& ) = delete;

protected:

    static unsigned int gui_bgid;

    lx::Win::Window& win;
    lx::TrueTypeFont::Font * f;
    lx::Graphics::TextTexture * title_text;
    lx::Graphics::Texture * bg;
    GUI_State state;
    GUI_Button_State bstate;

public:

    explicit GUI( lx::Win::Window& w );
    virtual void draw() noexcept = 0;
    virtual void setButtonState( GUI_Button_State st ) noexcept = 0;
    virtual void getAABBs( lx::Physics::FloatingBox * rects ) noexcept = 0;
    virtual ~GUI();
};


class MainGUI final: virtual public GUI
{
    lx::TrueTypeFont::Font * title_font;
    lx::Graphics::Sprite * button_play;
    lx::Graphics::Sprite * button_option;
    lx::Graphics::Sprite * button_quit;
    lx::Graphics::Sprite * img_control;
    lx::Graphics::TextTexture * play_text;
    lx::Graphics::TextTexture * option_text;
    lx::Graphics::TextTexture * quit_text;

    MainGUI( const MainGUI& ) = delete;
    MainGUI( const MainGUI&& ) = delete;
    MainGUI& operator =( const MainGUI& ) = delete;
    MainGUI& operator =( const MainGUI&& ) = delete;

public:

    static const int NB_BUTTONS = 3;

    explicit MainGUI( lx::Win::Window& w );

    virtual void draw() noexcept override;
    virtual void setButtonState( GUI_Button_State st ) noexcept override;
    virtual void getAABBs( lx::Physics::FloatingBox * rects ) noexcept override;
    void setState( GUI_State st ) noexcept;

    virtual ~MainGUI();
};

class OptionGUI final: virtual public GUI
{
    lx::TrueTypeFont::Font * text_font;
    lx::Graphics::TextTexture * ov_volume_text;
    lx::Graphics::TextTexture * ov_volume_vtext;
    lx::Graphics::Sprite * button_ov_down;
    lx::Graphics::Sprite * button_ov_up;

    lx::Graphics::TextTexture * music_volume_text;
    lx::Graphics::TextTexture * music_volume_vtext;
    lx::Graphics::Sprite * button_music_down;
    lx::Graphics::Sprite * button_music_up;

    lx::Graphics::TextTexture * fx_volume_text;
    lx::Graphics::TextTexture * fx_volume_vtext;
    lx::Graphics::Sprite * button_fx_down;
    lx::Graphics::Sprite * button_fx_up;

    lx::Graphics::TextTexture * fullscreen_text;
    lx::Graphics::ShadedTextTexture * fullscreen_vtext;

    lx::Graphics::TextTexture * gp_text;
    lx::Graphics::Sprite * button_gp;
    lx::Graphics::TextTexture * return_text;
    lx::Graphics::Sprite * button_back;

    lx::Graphics::TextTexture * esc_text;
    lx::Mixer::Chunk * vsound;

    OptionGUI( const OptionGUI& ) = delete;
    OptionGUI( const OptionGUI&& ) = delete;
    OptionGUI& operator =( const OptionGUI& ) = delete;
    OptionGUI& operator =( const OptionGUI&& ) = delete;

    void position() noexcept;
    unsigned short incVolume( unsigned short vol ) noexcept;
    unsigned short decVolume( unsigned short vol ) noexcept;

public:

    static const int NB_BUTTONS = 12;

    explicit OptionGUI( lx::Win::Window& w, const Option::OptionHandler& opt );

    virtual void draw() noexcept override;
    virtual void setButtonState( GUI_Button_State st ) noexcept override;
    virtual void getAABBs( lx::Physics::FloatingBox * rects ) noexcept override;

    void playSound() noexcept;

    void updateTextVolume( GUI_Button_State st, Option::OptionHandler& opt ) noexcept;
    void updateVolume( GUI_Button_State st, Option::OptionHandler& opt ) noexcept;
    void updateFullscreen( GUI_Button_State st, Option::OptionHandler& opt ) noexcept;

    virtual ~OptionGUI();
};

class GamepadGUI final: virtual public GUI
{
    lx::TrueTypeFont::Font * text_font;
    lx::Graphics::TextTexture * gp_text;
    lx::Graphics::TextTexture * back_text;

    lx::Graphics::TextTexture * shot_text;
    lx::Graphics::ShadedTextTexture * shot_vtext;
    lx::Graphics::TextTexture * rocket_text;
    lx::Graphics::ShadedTextTexture * rocket_vtext;
    lx::Graphics::TextTexture * bomb_text;
    lx::Graphics::ShadedTextTexture * bomb_vtext;
    lx::Graphics::TextTexture * smode_text;
    lx::Graphics::ShadedTextTexture * smode_vtext;

    lx::Graphics::Sprite * button_back;
    lx::Graphics::Sprite * xbox;
    lx::Graphics::Colour colour;
    const lx::Graphics::Colour BHOVER_COLOUR;
    const lx::Graphics::Colour BCLICK_COLOUR;

    GamepadGUI( const GamepadGUI& ) = delete;
    GamepadGUI( const GamepadGUI&& ) = delete;
    GamepadGUI& operator =( const GamepadGUI& ) = delete;
    GamepadGUI& operator =( const GamepadGUI&& ) = delete;

    void updateGamepadGUI() noexcept;
    void position() noexcept;

public:

    static const int NB_BUTTONS = 5;

    explicit GamepadGUI( lx::Win::Window& w );

    virtual void draw() noexcept override;
    virtual void setButtonState( GUI_Button_State st ) noexcept override;
    virtual void getAABBs( lx::Physics::FloatingBox * rects ) noexcept override;

    ~GamepadGUI();
};

#endif // GUI_HPP_INCLUDED
