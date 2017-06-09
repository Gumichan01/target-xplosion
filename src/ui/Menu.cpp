
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


#include "Menu.hpp"
#include "GUI.hpp"
#include "../asset/TX_Asset.hpp"
#include "../game/engine/Engine.hpp"
#include "../game/Result.hpp"
#include "../option/OptionHandler.hpp"

#include <LunatiX/LX_Music.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Event.hpp>
#include <LunatiX/LX_Device.hpp>
#include <typeinfo>

using namespace LX_Event;

namespace
{
const uint32_t SLEEP = 33;
}

const UTF8string GP_A_BUTTON("a");
const UTF8string GP_B_BUTTON("b");
const short GP_MAX_DOWN = 32000;
const short GP_MAX_UP   = -32000;


/** Menu */

Menu::Menu() : _done(false), gui(nullptr), cursor(0), validate(false),
    button_rect(nullptr) {}

void Menu::gamepadEvent(LX_EventHandler& ev)
{
    if(ev.getEventType() == LX_CONTROLLERAXISMOTION)
    {
        const LX_GAxis ax = ev.getAxis();

        if(ax.value > GP_MAX_DOWN)
        {
            if(cursor < OptionGUI::NB_BUTTONS)
                cursor++;
        }
        else if(ax.value < GP_MAX_UP)
        {
            if(cursor > 0)
                cursor--;
        }
    }
    else if(ev.getEventType() == LX_CONTROLLERBUTTONUP)
    {
        // Button
        const LX_GButton bu = ev.getButton();

        if(stringOfButton(bu.value) == GP_A_BUTTON)
            validate = true;
        else if(stringOfButton(bu.value) == GP_B_BUTTON)
            _done = true;
    }

    subEvent();
}

void Menu::keyboardEvent(LX_EventHandler& ev)
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

    else if(ev.getKeyCode() == SDLK_BACKSPACE)
        _done = true;

    subEvent();
}

void Menu::event()
{
    LX_EventHandler ev;

    while(!_done)
    {
        while(ev.pollEvent())
        {
            switch(ev.getEventType())
            {
            case LX_QUIT:
                _done = true;
                break;

            case LX_MOUSEBUTTONUP:
                mouseClick(ev, _done);
                break;

            case LX_MOUSEMOTION:
                hover(ev);
                break;

            case LX_CONTROLLERBUTTONUP:
            case LX_CONTROLLERAXISMOTION:
                gamepadEvent(ev);
                break;

            case LX_KEYUP:
                keyboardEvent(ev);
                break;

            default:
                break;
            }
        }

        gui->draw();
        LX_Timer::delay(SLEEP);
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
    button_rect = new LX_AABB[MainGUI::NB_BUTTONS];

    gui->getAABBs(button_rect);
    music_menu->play(LX_Mixer::LX_MIXER_LOOP);
    win.setDrawBlendMode(LX_Win::LX_BLENDMODE_BLEND);

    Option::OptionHandler op;
    loadGamepad();

    if(op.getFullscreenFlag() == static_cast<uint8_t>(1))
        win.toggleFullscreen(LX_Win::LX_WINDOW_FULLSCREEN);
}


MainMenu::~MainMenu()
{
    gamepad.close();
    delete music_menu;
    delete gui;
}


void MainMenu::loadGamepad()
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


void MainMenu::subEvent()
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

        gui->setButtonState(NORMAL);
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


void MainMenu::hover(LX_EventHandler& ev)
{
    const LX_Physics::LX_Point p(ev.getMouseMotion().x, ev.getMouseMotion().y);

    if(LX_Physics::collisionPointRect(p, button_rect[0]))
        gui->setButtonState(PLAY_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[1]))
        gui->setButtonState(OPT_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[2]))
        gui->setButtonState(QUIT_BUTTON_HOVER);

    else
        gui->setButtonState(NORMAL);
}


void MainMenu::mouseClick(LX_EventHandler& ev, bool& done)
{
    const LX_Physics::LX_Point p(ev.getMouseButton().x, ev.getMouseButton().y);

    if(LX_Physics::collisionPointRect(p, button_rect[0]))
    {
        play();
        gui->setButtonState(NORMAL);
    }
    else if(LX_Physics::collisionPointRect(p, button_rect[1]))
    {
        option();
        gui->setButtonState(NORMAL);
    }
    else if(LX_Physics::collisionPointRect(p, button_rect[2]))
    {
        done = true;
        music_menu->stop();
    }
}


void MainMenu::play()
{
    const int FIRST_LEVEL = 1;
    const int LAST_LEVEL = 2;

    Engine::init();
    music_menu->stop();
    Engine *target_xplosion = Engine::getInstance();
    ResultInfo info = {0,0,0,0,0,0,0};

    for(int i = FIRST_LEVEL; i <= LAST_LEVEL; i++)
    {
        EngineStatusV gs = target_xplosion->play(info, i);

        if(gs == EngineStatusV::GAME_QUIT)
            break;
        else if(gs == EngineStatusV::GAME_FINISH)
            Result::displayResult(info);
        else
            LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"Unknown game state");

        info.nb_killed_enemies = 0;
        info.max_nb_enemies = 0;
        info.nb_death = 0;
        info.score = 0;
    }
    Engine::destroy();
    music_menu->play();
}


void MainMenu::option()
{
    OptionMenu opt(win);
    opt.event();
}


/** Option menu */

OptionMenu::OptionMenu(LX_Win::LX_Window& w) : win(w), opt_handler(nullptr)
{
    opt_handler = new Option::OptionHandler();
    gui = new OptionGUI(w,*opt_handler);
    button_rect = new LX_AABB[OptionGUI::NB_BUTTONS];
    gui->getAABBs(button_rect);
}


OptionMenu::~OptionMenu()
{
    delete opt_handler;
    delete gui;
}

OptionGUI * OptionMenu::getGUI()
{
    OptionGUI *opt_gui = dynamic_cast<OptionGUI*>(gui);

    if(opt_gui == nullptr)
        throw std::bad_cast();

    return opt_gui;
}

void OptionMenu::subEvent()
{
    OptionGUI *opt_gui = getGUI();
    cursor %= OptionGUI::NB_BUTTONS -2;

    if(validate)
    {
        if(cursor == 0)
            opt_gui->updateVolume(OVD_BUTTON_CLICK,*opt_handler);

        else if(cursor == 1)
            opt_gui->updateVolume(OVU_BUTTON_CLICK,*opt_handler);

        else if(cursor == 2)
            opt_gui->updateVolume(MUD_BUTTON_CLICK,*opt_handler);

        else if(cursor == 3)
            opt_gui->updateVolume(MUU_BUTTON_CLICK,*opt_handler);

        else if(cursor == 4)
            opt_gui->updateVolume(FXD_BUTTON_CLICK,*opt_handler);

        else if(cursor == 5)
            opt_gui->updateVolume(FXU_BUTTON_CLICK,*opt_handler);

        else if(cursor == 6)
            opt_gui->updateFullscreen(FS_BUTTON_CLICK,*opt_handler);

        else if(cursor == 7)
            gamepad();

        else if(cursor == 8)
        {
            gui->setButtonState(NORMAL);
            _done = true;
        }
    }
    else
    {
        if(cursor == 0)
            gui->setButtonState(OVD_BUTTON_HOVER);

        else if(cursor == 1)
            gui->setButtonState(OVU_BUTTON_HOVER);

        else if(cursor == 2)
            gui->setButtonState(MUD_BUTTON_HOVER);

        else if(cursor == 3)
            gui->setButtonState(MUU_BUTTON_HOVER);

        else if(cursor == 4)
            gui->setButtonState(FXD_BUTTON_HOVER);

        else if(cursor == 5)
            gui->setButtonState(FXU_BUTTON_HOVER);

        else if(cursor == 6)
            gui->setButtonState(FS_BUTTON_HOVER);

        else if(cursor == 7)
            gui->setButtonState(GP_BUTTON_HOVER);

        else if(cursor == 8)
            gui->setButtonState(BACK_BUTTON_HOVER);

        else
            gui->setButtonState(NORMAL);
    }

    validate = false;
}


void OptionMenu::hover(LX_EventHandler& ev)
{
    const LX_Physics::LX_Point p(ev.getMouseMotion().x, ev.getMouseMotion().y);

    if(LX_Physics::collisionPointRect(p, button_rect[0]))
        gui->setButtonState(OVD_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[1]))
        gui->setButtonState(OVU_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[2]))
        gui->setButtonState(MUD_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[3]))
        gui->setButtonState(MUU_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[4]))
        gui->setButtonState(FXD_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[5]))
        gui->setButtonState(FXU_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[6]))
        gui->setButtonState(FS_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[10]))
        gui->setButtonState(GP_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[11]))
        gui->setButtonState(BACK_BUTTON_HOVER);

    else
        gui->setButtonState(NORMAL);
}


void OptionMenu::mouseClick(LX_EventHandler& ev, bool& done)
{
    const LX_Physics::LX_Point p(ev.getMouseButton(). x, ev.getMouseButton().y);
    OptionGUI *opt_gui = getGUI();

    if(LX_Physics::collisionPointRect(p, button_rect[10]))
        gamepad();
    else if(LX_Physics::collisionPointRect(p, button_rect[11]))
    {
        gui->setButtonState(NORMAL);
        done = true;
    }
    else
    {
        if(opt_gui != nullptr)
        {
            if(LX_Physics::collisionPointRect(p, button_rect[0]))
                opt_gui->updateVolume(OVD_BUTTON_CLICK,*opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[1]))
                opt_gui->updateVolume(OVU_BUTTON_CLICK,*opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[2]))
                opt_gui->updateVolume(MUD_BUTTON_CLICK,*opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[3]))
                opt_gui->updateVolume(MUU_BUTTON_CLICK,*opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[4]))
                opt_gui->updateVolume(FXD_BUTTON_CLICK,*opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[5]))
                opt_gui->updateVolume(FXU_BUTTON_CLICK,*opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[6]))
                opt_gui->updateFullscreen(FS_BUTTON_CLICK,*opt_handler);
            else if(LX_Physics::collisionPointRect(p, button_rect[7]))
                opt_gui->updateTextVolume(OV_TEXT_CLICK, *opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[8]))
                opt_gui->updateTextVolume(MU_TEXT_CLICK, *opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[9]))
                opt_gui->updateTextVolume(FX_TEXT_CLICK, *opt_handler);
        }
    }
}

void OptionMenu::gamepad()
{
    GamepadMenu gp(win);
    gp.event();
}

/** Gamepad menu */

GamepadMenu::GamepadMenu(LX_Win::LX_Window& w)
{
    gui = new GamepadGUI(w);
    button_rect = new LX_AABB[GamepadGUI::NB_BUTTONS];
    gui->getAABBs(button_rect);
}


void GamepadMenu::hover(LX_Event::LX_EventHandler& ev)
{
    const LX_Physics::LX_Point p(ev.getMouseButton(). x, ev.getMouseButton().y);

    if(LX_Physics::collisionPointRect(p, button_rect[0]))
        gui->setButtonState(BACK_BUTTON_HOVER);
    else
        gui->setButtonState(NORMAL);
}

void GamepadMenu::mouseClick(LX_Event::LX_EventHandler& ev, bool& done)
{
    const LX_Physics::LX_Point p(ev.getMouseButton(). x, ev.getMouseButton().y);

    if(LX_Physics::collisionPointRect(p, button_rect[0]))
    {
        gui->setButtonState(NORMAL);
        done = true;
    }
}

void GamepadMenu::subEvent() {}
