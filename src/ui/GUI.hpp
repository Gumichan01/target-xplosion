
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
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
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/


#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

enum GUI_State: short {UNDEF_GUI,MAIN_GUI,PLAY_GUI};
enum GUI_Button_State: short {NORMAL,
                              /* States in the main menu */
                              PLAY_BUTTON_HOVER,OPT_BUTTON_HOVER,QUIT_BUTTON_HOVER,
                              /* States in the option menu */
                              GP_BUTTON_HOVER,BACK_BUTTON_HOVER,OVD_BUTTON_HOVER,
                              OVU_BUTTON_HOVER,MUD_BUTTON_HOVER,MUU_BUTTON_HOVER,
                              FXD_BUTTON_HOVER,FXU_BUTTON_HOVER,
                             };

namespace LX_Win
{
class LX_Window;
};

namespace LX_Graphics
{
class LX_Image;
class LX_Sprite;
class LX_TextImage;
class LX_ShadedTextImage;
};

namespace LX_TrueTypeFont
{
class LX_Font;
};

struct LX_AABB;


class GUI
{
protected:
    LX_Win::LX_Window& win;
    LX_TrueTypeFont::LX_Font * f;
    LX_Graphics::LX_TextImage * title_text;
    LX_Graphics::LX_Image * bg;
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
    LX_Graphics::LX_Sprite * button_play;
    LX_Graphics::LX_Sprite * button_option;
    LX_Graphics::LX_Sprite * button_quit;
    LX_Graphics::LX_TextImage * play_text;
    LX_Graphics::LX_TextImage * option_text;
    LX_Graphics::LX_TextImage * quit_text;

public:

    static const int NB_BUTTONS = 3;

    explicit MainGUI(LX_Win::LX_Window& w);

    void draw();
    void setState(GUI_State st);
    virtual void setButtonState(GUI_Button_State st);
    void getAABBs(LX_AABB * aabb);

    virtual ~MainGUI();
};

class OptionGUI: virtual public GUI
{
    LX_Graphics::LX_TextImage * ov_volume_text;
    LX_Graphics::LX_ShadedTextImage * ov_volume_vtext;
    LX_Graphics::LX_Sprite * button_ov_down;
    LX_Graphics::LX_Sprite * button_ov_up;

    LX_Graphics::LX_TextImage * music_volume_text;
    LX_Graphics::LX_ShadedTextImage * music_volume_vtext;
    LX_Graphics::LX_Sprite * button_music_down;
    LX_Graphics::LX_Sprite * button_music_up;

    LX_Graphics::LX_TextImage * fx_volume_text;
    LX_Graphics::LX_ShadedTextImage * fx_volume_vtext;
    LX_Graphics::LX_Sprite * button_fx_down;
    LX_Graphics::LX_Sprite * button_fx_up;

    LX_Graphics::LX_TextImage * gp_text;
    LX_Graphics::LX_Sprite * button_gp;
    LX_Graphics::LX_TextImage * return_text;
    LX_Graphics::LX_Sprite * button_back;

public:

    static const int NB_BUTTONS = 8;

    explicit OptionGUI(LX_Win::LX_Window& w);

    void draw();
    virtual void setButtonState(GUI_Button_State st);
    void getAABBs(LX_AABB * aabb);

    virtual ~OptionGUI();
};


#endif // GUI_HPP_INCLUDED
