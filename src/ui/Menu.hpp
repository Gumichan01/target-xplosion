
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

#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

#include <LunatiX/LX_ImgRect.hpp>
#include <LunatiX/LX_Gamepad.hpp>


namespace LX_Win
{
class LX_Window;
}

namespace LX_Mixer
{
class LX_Music;
}

namespace Option
{
class OptionHandler;
}

namespace LX_Event
{
class LX_EventHandler;
}

class GUI;
class MainGUI;
class OptionGUI;

class Menu
{
    Menu( const Menu& ) = delete;
    Menu( const Menu&& ) = delete;
    Menu& operator =( const Menu& ) = delete;
    Menu& operator =( const Menu&& ) = delete;

    void gamepadEvent( LX_Event::LX_EventHandler& ev ) noexcept;
    void keyboardEvent( LX_Event::LX_EventHandler& ev ) noexcept;

protected:

    bool _done;
    GUI * gui;
    int cursor;
    bool validate;
    bool navigating;
    bool has_written;
    LX_Physics::LX_FloatingBox * button_rect;

    virtual void subEvent() noexcept = 0;
    virtual void hover( LX_Event::LX_EventHandler& ev ) noexcept = 0;
    virtual void mouseClick( LX_Event::LX_EventHandler& ev ) noexcept = 0;

public:

    Menu();
    virtual void event() noexcept;
    virtual ~Menu() = default;
};

class MainMenu final: virtual public Menu
{
    LX_Win::LX_Window& win;
    LX_Mixer::LX_Music * music_menu;
    LX_Device::LX_Gamepad gamepad;

    MainMenu( const MainMenu& ) = delete;
    MainMenu( const MainMenu&& ) = delete;
    MainMenu& operator =( const MainMenu& ) = delete;
    MainMenu& operator =( const MainMenu&& ) = delete;

    void play() noexcept;
    void option() noexcept;
    void loadGamepad() noexcept;

protected:

    virtual void subEvent() noexcept override;
    virtual void hover( LX_Event::LX_EventHandler& ev ) noexcept override;
    virtual void mouseClick( LX_Event::LX_EventHandler& ev ) noexcept override;

public:

    explicit MainMenu( LX_Win::LX_Window& w );
    virtual ~MainMenu();
};

class OptionMenu final: virtual public Menu
{
    LX_Win::LX_Window& win;
    OptionGUI * opt_gui;
    Option::OptionHandler * opt_handler;

    OptionMenu( const OptionMenu& ) = delete;
    OptionMenu( const OptionMenu&& ) = delete;
    OptionMenu& operator =( const OptionMenu& ) = delete;
    OptionMenu& operator =( const OptionMenu&& ) = delete;

    void call_( int cur, bool from_keyboard = false ) noexcept;
    void hover_( int cur ) noexcept;

protected:

    virtual void hover( LX_Event::LX_EventHandler& ev ) noexcept override;
    void mouseClick( LX_Event::LX_EventHandler& ev ) noexcept;
    virtual void subEvent() noexcept override;

public:

    explicit OptionMenu( LX_Win::LX_Window& w );
    void gamepad() noexcept;
    virtual ~OptionMenu();
};

class GamepadMenu final: virtual public Menu
{
    GamepadMenu( const GamepadMenu& ) = delete;
    GamepadMenu( const GamepadMenu&& ) = delete;
    GamepadMenu& operator =( const GamepadMenu& ) = delete;
    GamepadMenu& operator =( const GamepadMenu&& ) = delete;

    void hover_( int i ) noexcept;
    void ignoreInput_() noexcept;
    void restoreInput_() noexcept;
    void beforeClick_( int i ) noexcept;
    void click_( int i ) noexcept;
    void afterClick_( const LX_Event::LX_EventHandler& ev, int i ) noexcept;

protected:

    virtual void hover( LX_Event::LX_EventHandler& ev ) noexcept;
    virtual void mouseClick( LX_Event::LX_EventHandler& ev ) noexcept;
    virtual void subEvent() noexcept override;

public:

    explicit GamepadMenu( LX_Win::LX_Window& w );
    virtual ~GamepadMenu();
};

#endif // MENU_HPP_INCLUDED
