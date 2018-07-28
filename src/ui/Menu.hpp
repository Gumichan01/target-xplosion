
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

#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

#include <Lunatix/ImgRect.hpp>
#include <Lunatix/Gamepad.hpp>


namespace lx
{

namespace Win
{
class Window;
}

namespace Mixer
{
class Music;
}

namespace Event
{
class EventHandler;
}

}

namespace Option
{
class OptionHandler;
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

    void gamepadEvent( lx::Event::EventHandler& ev ) noexcept;
    void keyboardEvent( lx::Event::EventHandler& ev ) noexcept;

protected:

    bool _done;
    GUI * gui;
    int cursor;
    bool validate;
    bool navigating;
    bool has_written;
    lx::Physics::FloatingBox * button_rect;

    virtual void subEvent() noexcept = 0;
    virtual void hover( lx::Event::EventHandler& ev ) noexcept = 0;
    virtual void mouseClick( lx::Event::EventHandler& ev ) noexcept = 0;

public:

    Menu();
    virtual void event() noexcept;
    virtual ~Menu() = default;
};

class MainMenu final: virtual public Menu
{
    lx::Win::Window& win;
    lx::Mixer::Music * music_menu;
    lx::Device::Gamepad gamepad;

    MainMenu( const MainMenu& ) = delete;
    MainMenu( const MainMenu&& ) = delete;
    MainMenu& operator =( const MainMenu& ) = delete;
    MainMenu& operator =( const MainMenu&& ) = delete;

    void play() noexcept;
    void option() noexcept;
    void loadGamepad() noexcept;

protected:

    virtual void subEvent() noexcept override;
    virtual void hover( lx::Event::EventHandler& ev ) noexcept override;
    virtual void mouseClick( lx::Event::EventHandler& ev ) noexcept override;

public:

    explicit MainMenu( lx::Win::Window& w );
    virtual ~MainMenu();
};

class OptionMenu final: virtual public Menu
{
    lx::Win::Window& win;
    OptionGUI * opt_gui;
    Option::OptionHandler * opt_handler;

    OptionMenu( const OptionMenu& ) = delete;
    OptionMenu( const OptionMenu&& ) = delete;
    OptionMenu& operator =( const OptionMenu& ) = delete;
    OptionMenu& operator =( const OptionMenu&& ) = delete;

    void call_( int cur, bool from_keyboard = false ) noexcept;
    void hover_( int cur ) noexcept;

protected:

    virtual void hover( lx::Event::EventHandler& ev ) noexcept override;
    void mouseClick( lx::Event::EventHandler& ev ) noexcept;
    virtual void subEvent() noexcept override;

public:

    explicit OptionMenu( lx::Win::Window& w );
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
    void afterClick_( const lx::Event::EventHandler& ev, int i ) noexcept;

protected:

    virtual void hover( lx::Event::EventHandler& ev ) noexcept;
    virtual void mouseClick( lx::Event::EventHandler& ev ) noexcept;
    virtual void subEvent() noexcept override;

public:

    explicit GamepadMenu( lx::Win::Window& w );
    virtual ~GamepadMenu();
};

#endif // MENU_HPP_INCLUDED
