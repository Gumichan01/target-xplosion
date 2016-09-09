
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
enum GUI_Button_State: short {NORMAL,PLAY_BUTTON_HOVER,OPT_BUTTON_HOVER,QUIT_BUTTON_HOVER};

namespace LX_Win
{
class LX_Window;
};

namespace LX_Graphics
{
class LX_Image;
class LX_Sprite;
class LX_TextImage;
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
    GUI_State state;
    GUI_Button_State bstate;

public:

    GUI(LX_Win::LX_Window& w);
    virtual void draw() = 0;
    ~GUI();
};


class MainGUI: virtual public GUI
{
    LX_Graphics::LX_Image * bg;
    LX_Graphics::LX_Sprite * button_play;
    LX_Graphics::LX_Sprite * button_option;
    LX_Graphics::LX_Sprite * button_quit;
    LX_Graphics::LX_TextImage * title_text;
    LX_Graphics::LX_TextImage * play_text;
    LX_Graphics::LX_TextImage * option_text;
    LX_Graphics::LX_TextImage * quit_text;

public:

    static const int NB_BUTTONS = 3;

    MainGUI(LX_Win::LX_Window& w);

    void draw();
    void setState(GUI_State st);
    void setButtonState(GUI_Button_State st);
    void getAABBs(LX_AABB * aabb);

    virtual ~MainGUI();
};

#endif // GUI_HPP_INCLUDED
