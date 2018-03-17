
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


#include "Menu.hpp"
#include "GUI.hpp"
#include "../asset/TX_Asset.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/Result.hpp"
#include "../game/Balance.hpp"
#include "../option/OptionHandler.hpp"

#include <LunatiX/LX_Music.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Event.hpp>
#include <LunatiX/LX_Device.hpp>

using namespace LX_Event;
using namespace FloatBox;

namespace
{
const uint32_t MENU_SLEEP = 33;
const UTF8string MENU_GP_A_BUTTON("a");
const UTF8string MENU_GP_B_BUTTON("b");
const short MENU_GP_MAX_DOWN = 32000;
const short MENU_GP_MAX_UP   = -32000;
}


/** Menu */

Menu::Menu() : _done(false), gui(nullptr), cursor(0), validate(false),
    has_written(false), button_rect(nullptr) {}

void Menu::gamepadEvent(LX_EventHandler& ev) noexcept
{
    if(ev.getEventType() == LX_EventType::CONTROLLERAXISMOTION)
    {
        const LX_GAxis ax = ev.getAxis();

        if(ax.value > MENU_GP_MAX_DOWN)
        {
            if(cursor < OptionGUI::NB_BUTTONS)
                cursor++;
        }
        else if(ax.value < MENU_GP_MAX_UP)
        {
            if(cursor > 0)
                cursor--;
        }
    }
    else if(ev.getEventType() == LX_EventType::CONTROLLERBUTTONUP)
    {
        // Button
        const LX_GButton bu = ev.getButton();

        if(stringOfButton(bu.value) == MENU_GP_A_BUTTON)
            validate = true;
        else if(stringOfButton(bu.value) == MENU_GP_B_BUTTON)
            _done = true;
    }

    subEvent();
}

void Menu::keyboardEvent(LX_EventHandler& ev) noexcept
{
    if(ev.getKeyCode() == SDLK_UP || ev.getKeyCode() == SDLK_LEFT)
    {
        if(cursor > 0)
            cursor--;
    }
    else if(ev.getKeyCode() == SDLK_DOWN || ev.getKeyCode() == SDLK_RIGHT)
    {
        if(cursor < OptionGUI::NB_BUTTONS)
            cursor++;
    }
    else if(ev.getKeyCode() == SDLK_RETURN)
        validate = true;

    else if(ev.getKeyCode() == SDLK_ESCAPE && !has_written)
    {
        if(has_written)
            has_written = false;
        else
            _done = true;
    }

    subEvent();
}

void Menu::event() noexcept
{
    LX_EventHandler ev;

    while(!_done)
    {
        while(ev.pollEvent())
        {
            switch(ev.getEventType())
            {
            case LX_EventType::QUIT:
                _done = true;
                break;

            case LX_EventType::MOUSEBUTTONUP:
                mouseClick(ev);
                break;

            case LX_EventType::MOUSEMOTION:
                hover(ev);
                break;

            case LX_EventType::CONTROLLERBUTTONUP:
            case LX_EventType::CONTROLLERAXISMOTION:
                gamepadEvent(ev);
                break;

            case LX_EventType::KEYUP:
                keyboardEvent(ev);
                break;

            default:
                break;
            }
        }

        gui->draw();
        LX_Timer::delay(MENU_SLEEP);
    }
}

Menu::~Menu()
{
    delete [] button_rect;
}


/** Main menu */

MainMenu::MainMenu(LX_Win::LX_Window& w) : win(w), music_menu(nullptr),
    gamepad()
{
    gui = new MainGUI(w);
    music_menu = new LX_Mixer::LX_Music(TX_Asset::getInstance()->getLevelMusic(0));
    button_rect = new LX_Physics::LX_FloatingBox[MainGUI::NB_BUTTONS];

    gui->getAABBs(button_rect);
    music_menu->play(true);
    win.setDrawBlendMode(LX_Win::LX_BlendMode::LX_BLENDMODE_BLEND);

    Option::OptionHandler op;
    loadGamepad();

    if(op.getFullscreenFlag() == static_cast<uint8_t>(1))
        win.toggleFullscreen(LX_Win::LX_WinMode::FULLSCREEN);

    // Set the butteon state
    gui->setButtonState(PLAY_BUTTON_HOVER);
}


MainMenu::~MainMenu()
{
    gamepad.close();
    delete music_menu;
    delete gui;
}


void MainMenu::loadGamepad() noexcept
{
    using namespace LX_Device;

    if(numberOfDevices() > 0)
    {
        gamepad.open(0);

        if(gamepad.isConnected())
        {
            LX_GamepadInfo gpi;
            gamepad.stat(gpi);
            LX_Log::log("\n%s", gamepadToString(gpi).utf8_str());
        }
    }
}


void MainMenu::subEvent() noexcept
{
    cursor %= MainGUI::NB_BUTTONS;

    if(validate)
    {
        // Button selected
        switch(cursor)
        {
        case 0:
            play();
            break;
        case 1:
            option();
            break;
        case 2:
            _done = true;
            music_menu->stop();
            break;
        default:
            break;
        }
    }
    else
    {
        // Navigation
        switch(cursor)
        {
        case 0:
            gui->setButtonState(PLAY_BUTTON_HOVER);
            break;
        case 1:
            gui->setButtonState(OPT_BUTTON_HOVER);
            break;
        case 2:
            gui->setButtonState(QUIT_BUTTON_HOVER);
            break;
        default:
            break;
        }
    }

    validate = false;
}


void MainMenu::hover(LX_EventHandler& ev) noexcept
{
    const LX_Physics::LX_FloatPosition P = {fbox(ev.getMouseMotion().x),
                                            fbox(ev.getMouseMotion().y)
                                           };

    if(LX_Physics::collisionPointBox(P, button_rect[0]))
        gui->setButtonState(PLAY_BUTTON_HOVER);

    else if(LX_Physics::collisionPointBox(P, button_rect[1]))
        gui->setButtonState(OPT_BUTTON_HOVER);

    else if(LX_Physics::collisionPointBox(P, button_rect[2]))
        gui->setButtonState(QUIT_BUTTON_HOVER);
}


void MainMenu::mouseClick(LX_EventHandler& ev) noexcept
{
    const LX_Physics::LX_FloatPosition P = {fbox(ev.getMouseButton().x),
                                            fbox(ev.getMouseButton().y)
                                           };

    if(LX_Physics::collisionPointBox(P, button_rect[0]))
        play();

    else if(LX_Physics::collisionPointBox(P, button_rect[1]))
        option();

    else if(LX_Physics::collisionPointBox(P, button_rect[2]))
    {
        _done = true;
        music_menu->stop();
    }
}


void MainMenu::play() noexcept
{
    const int FIRST_LEVEL = 1;
    const int LAST_LEVEL = 3;

    music_menu->stop();
    Engine *target_xplosion = Engine::getInstance();
    ResultInfo info = {0,0,0,0,0,0,0};
    DynamicGameBalance::reset();

    for(int i = FIRST_LEVEL; i <= LAST_LEVEL; i++)
    {
        EngineStatusV gs = target_xplosion->play(info, i);

        if(gs == EngineStatusV::GAME_QUIT)
            break;

        if(gs == EngineStatusV::GAME_FINISH)
            Result::displayResult(info);
        else
            LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,"Unknown game state");

        info.nb_killed_enemies = 0;
        info.max_nb_enemies = 0;
        info.nb_death = 0;
        info.score = 0;
    }

    Engine::destroy();
    music_menu->play(true);
}


void MainMenu::option() noexcept
{
    OptionMenu opt(win);
    opt.event();
}


/** Option menu */

OptionMenu::OptionMenu(LX_Win::LX_Window& w) : win(w), opt_gui(nullptr),
    opt_handler(nullptr)
{
    opt_handler = new Option::OptionHandler();
    opt_gui = new OptionGUI(w,*opt_handler);
    gui = opt_gui;
    button_rect = new LX_Physics::LX_FloatingBox[OptionGUI::NB_BUTTONS];
    gui->getAABBs(button_rect);
    gui->setButtonState(OVD_BUTTON_HOVER);
}


OptionMenu::~OptionMenu()
{
    delete opt_handler;
    delete gui;
}


void OptionMenu::subEvent() noexcept
{
    cursor %= OptionGUI::NB_BUTTONS -3;

    if(validate)
        call_(cursor, true);
    else
        hover_(cursor);

    validate = false;
}

void OptionMenu::call_(int cur, bool from_keyboard) noexcept
{
    switch(cur)
    {
    case 0:
        opt_gui->updateVolume(OVD_BUTTON_CLICK, *opt_handler);
        break;

    case 1:
        opt_gui->updateVolume(OVU_BUTTON_CLICK, *opt_handler);
        break;

    case 2:
        opt_gui->updateVolume(MUD_BUTTON_CLICK, *opt_handler);
        break;

    case 3:
        opt_gui->updateVolume(MUU_BUTTON_CLICK, *opt_handler);
        break;

    case 4:
        opt_gui->updateVolume(FXD_BUTTON_CLICK, *opt_handler);
        break;

    case 5:
        opt_gui->updateVolume(FXU_BUTTON_CLICK, *opt_handler);
        break;

    case 6:
        opt_gui->updateFullscreen(FS_BUTTON_CLICK, *opt_handler);
        break;

    case 7:
        if(from_keyboard)
            gamepad();
        else
        {
            opt_gui->updateTextVolume(OV_TEXT_CLICK, *opt_handler);
            has_written = true;
        }
        break;
    case 8:
        if(from_keyboard)
        {
            gui->setButtonState(NORMAL);
            _done = true;
        }
        else
        {
            opt_gui->updateTextVolume(MU_TEXT_CLICK, *opt_handler);
            has_written = true;
        }
        break;

    // from mouse
    case 9:
        opt_gui->updateTextVolume(FX_TEXT_CLICK, *opt_handler);
        has_written = true;
        break;

    case 10:
        gamepad();
        break;

    case 11:
        _done = true;
        break;
    }
}

void OptionMenu::hover_(int cur) noexcept
{
    switch(cur)
    {
    case 0:
        gui->setButtonState(OVD_BUTTON_HOVER);
        break;

    case 1:
        gui->setButtonState(OVU_BUTTON_HOVER);
        break;

    case 2:
        gui->setButtonState(MUD_BUTTON_HOVER);
        break;

    case 3:
        gui->setButtonState(MUU_BUTTON_HOVER);
        break;

    case 4:
        gui->setButtonState(FXD_BUTTON_HOVER);
        break;

    case 5:
        gui->setButtonState(FXU_BUTTON_HOVER);
        break;

    case 6:
        gui->setButtonState(FS_BUTTON_HOVER);
        break;

    case 7:
    case 10:
        gui->setButtonState(GP_BUTTON_HOVER);
        break;

    case 8:
    case 11:
        gui->setButtonState(BACK_BUTTON_HOVER);
        break;

    default:
        gui->setButtonState(NORMAL);
        break;
    }
}


void OptionMenu::hover(LX_EventHandler& ev) noexcept
{
    const LX_Physics::LX_FloatPosition P = {fbox(ev.getMouseMotion().x),
                                            fbox(ev.getMouseMotion().y)
                                           };

    int i = 0;
    while(i < OptionGUI::NB_BUTTONS)
    {
        // hitboxes from 7 to 9 are related to the text boxes.
        // I don't need to check them
        if(i < 7 || i > 9)
        {
            if(LX_Physics::collisionPointBox(P, button_rect[i]))
            {
                hover_(i);
                break;
            }
        }

        if(i == 7)
            i = 10;
        else
            i += 1;
    }

    if(i == OptionGUI::NB_BUTTONS)
        gui->setButtonState(NORMAL);
}


void OptionMenu::mouseClick(LX_EventHandler& ev) noexcept
{
    const LX_Physics::LX_FloatPosition P = {fbox(ev.getMouseButton().x),
                                            fbox(ev.getMouseButton().y)
                                           };

    int i = -1;
    while((++i) < OptionGUI::NB_BUTTONS)
    {
        if(LX_Physics::collisionPointBox(P, button_rect[i]))
        {
            call_(i, false);
            break;
        }
    }
}

void OptionMenu::gamepad() noexcept
{
    GamepadMenu gp(win);
    gp.event();
}

/** Gamepad menu */

GamepadMenu::GamepadMenu(LX_Win::LX_Window& w)
{
    gui = new GamepadGUI(w);
    button_rect = new LX_Physics::LX_FloatingBox[GamepadGUI::NB_BUTTONS];
    gui->getAABBs(button_rect);
}


void GamepadMenu::hover(LX_Event::LX_EventHandler& ev) noexcept
{
    const LX_Physics::LX_FloatPosition P = {fbox(ev.getMouseMotion().x),
                                            fbox(ev.getMouseMotion().y)
                                           };

    if(LX_Physics::collisionPointBox(P, button_rect[0]))
        gui->setButtonState(BACK_BUTTON_HOVER);
}

void GamepadMenu::mouseClick(LX_Event::LX_EventHandler& ev) noexcept
{
    const LX_Physics::LX_FloatPosition P = {fbox(ev.getMouseButton().x),
                                            fbox(ev.getMouseButton().y)
                                           };

    if(LX_Physics::collisionPointBox(P, button_rect[0]))
    {
        gui->setButtonState(NORMAL);
        _done = true;
    }
}
