
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


#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include <LunatiX/LX_AABB.hpp>
#include <LunatiX/LX_Colour.hpp>


enum GUI_State: short {UNDEF_GUI, MAIN_GUI, PLAY_GUI};
enum GUI_Button_State: short {NORMAL,
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
                             };

namespace LX_Win
{
class LX_Window;
};

namespace LX_Graphics
{
class LX_Texture;
class LX_Sprite;
class LX_TextTexture;
class LX_ShadedTextTexture;
};

namespace LX_TrueTypeFont
{
class LX_Font;
};

namespace Option
{
class OptionHandler;
}


class GUI
{
protected:

    LX_Win::LX_Window& win;
    LX_TrueTypeFont::LX_Font * f;
    LX_Graphics::LX_TextTexture * title_text;
    LX_Graphics::LX_Texture * bg;
    GUI_State state;
    GUI_Button_State bstate;


public:

    explicit GUI(LX_Win::LX_Window& w);
    virtual void draw() = 0;
    virtual void setButtonState(GUI_Button_State st) = 0;
    virtual void getAABBs(LX_AABB * aabb) = 0;
    virtual ~GUI();
};


class MainGUI: virtual public GUI
{
    LX_TrueTypeFont::LX_Font * title_font;
    LX_Graphics::LX_Sprite * button_play;
    LX_Graphics::LX_Sprite * button_option;
    LX_Graphics::LX_Sprite * button_quit;
    LX_Graphics::LX_TextTexture * play_text;
    LX_Graphics::LX_TextTexture * option_text;
    LX_Graphics::LX_TextTexture * quit_text;

public:

    static const int NB_BUTTONS = 3;

    explicit MainGUI(LX_Win::LX_Window& w);

    virtual void draw();
    virtual void setButtonState(GUI_Button_State st);
    virtual void getAABBs(LX_AABB * aabb);
    void setState(GUI_State st);

    virtual ~MainGUI();
};

class OptionGUI: virtual public GUI
{
    LX_TrueTypeFont::LX_Font * text_font;
    LX_Graphics::LX_TextTexture * ov_volume_text;
    LX_Graphics::LX_TextTexture * ov_volume_vtext;
    LX_Graphics::LX_Sprite * button_ov_down;
    LX_Graphics::LX_Sprite * button_ov_up;

    LX_Graphics::LX_TextTexture * music_volume_text;
    LX_Graphics::LX_TextTexture * music_volume_vtext;
    LX_Graphics::LX_Sprite * button_music_down;
    LX_Graphics::LX_Sprite * button_music_up;

    LX_Graphics::LX_TextTexture * fx_volume_text;
    LX_Graphics::LX_TextTexture * fx_volume_vtext;
    LX_Graphics::LX_Sprite * button_fx_down;
    LX_Graphics::LX_Sprite * button_fx_up;

    LX_Graphics::LX_TextTexture * fullscreen_text;
    LX_Graphics::LX_ShadedTextTexture * fullscreen_vtext;

    LX_Graphics::LX_TextTexture * gp_text;
    LX_Graphics::LX_Sprite * button_gp;
    LX_Graphics::LX_TextTexture * return_text;
    LX_Graphics::LX_Sprite * button_back;

    LX_Graphics::LX_TextTexture * esc_text;

    void position();
    unsigned short incVolume(unsigned short vol);
    unsigned short decVolume(unsigned short vol);

public:

    static const int NB_BUTTONS = 12;

    explicit OptionGUI(LX_Win::LX_Window& w, const Option::OptionHandler& opt);

    virtual void draw();
    virtual void setButtonState(GUI_Button_State st);
    virtual void getAABBs(LX_AABB * aabb);

    void updateTextVolume(GUI_Button_State st, Option::OptionHandler& opt);
    void updateVolume(GUI_Button_State st, Option::OptionHandler& opt);
    void updateFullscreen(GUI_Button_State st, Option::OptionHandler& opt);

    virtual ~OptionGUI();
};

class GamepadGUI: virtual public GUI
{
    LX_TrueTypeFont::LX_Font * text_font;
    LX_Graphics::LX_TextTexture * gp_text;
    LX_Graphics::LX_TextTexture * back_text;
    LX_Graphics::LX_Sprite * button_back;
    LX_Graphics::LX_Sprite * xbox;
    LX_Colour c;

    void position();

public:

    static const int NB_BUTTONS = 6;

    explicit GamepadGUI(LX_Win::LX_Window& w);

    virtual void draw();
    virtual void setButtonState(GUI_Button_State st);
    virtual void getAABBs(LX_AABB * aabb);

    ~GamepadGUI();
};

#endif // GUI_HPP_INCLUDED
