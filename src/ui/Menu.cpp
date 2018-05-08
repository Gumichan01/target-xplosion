
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
#include "../game/engine/AudioHandler.hpp"
#include "../game/Result.hpp"
#include "../option/OptionHandler.hpp"
#include "../option/GamepadControl.hpp"

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
    navigating(false), has_written(false), button_rect(nullptr) {}

void Menu::gamepadEvent(LX_EventHandler& ev) noexcept
{
    if(ev.getEventType() == LX_EventType::CONTROLLERAXISMOTION)
    {
        const LX_GAxis ax = ev.getAxis();

        if(ax.value > MENU_GP_MAX_DOWN)
        {
            //if(cursor < OptionGUI::NB_BUTTONS)
            {
                cursor++;
                navigating = true;
            }
        }
        else if(ax.value < MENU_GP_MAX_UP)
        {
            //if(cursor > 0)
            {
                cursor--;
                navigating = true;
            }
        }
    }
    else if(ev.getEventType() == LX_EventType::CONTROLLERBUTTONUP)
    {
        // Button
        const LX_GButton bu = ev.getButton();

        if(stringOfButton(bu.value) == MENU_GP_A_BUTTON)
        {
            validate = true;
        }
        else if(stringOfButton(bu.value) == MENU_GP_B_BUTTON)
        {
            _done = true;
        }
    }

    subEvent();
}

void Menu::keyboardEvent(LX_EventHandler& ev) noexcept
{
    if(ev.getKeyCode() == SDLK_UP || ev.getKeyCode() == SDLK_LEFT)
    {
        //if(cursor > 0)
        {
            cursor--;
            navigating = true;
        }
    }
    else if(ev.getKeyCode() == SDLK_DOWN || ev.getKeyCode() == SDLK_RIGHT)
    {
        //if(cursor < OptionGUI::NB_BUTTONS)
        {
            cursor++;
            navigating = true;
        }
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
    delete [] button_rect;
    delete music_menu;
    delete gui;
}


void MainMenu::loadGamepad() noexcept
{
    using LX_Device::gamepadToString;

    if(LX_Device::numberOfDevices() > 0)
    {
        gamepad.open(0);

        if(gamepad.isConnected())
        {
            LX_Device::LX_GamepadInfo gpi;
            gamepad.stat(gpi);
            LX_Log::log("\n%s", gamepadToString(gpi).utf8_str());
        }
    }
}


void MainMenu::subEvent() noexcept
{
    if(cursor < 0)
        cursor = MainGUI::NB_BUTTONS -1;
    else
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
            AudioHandler::AudioHDL::getInstance()->playMenuBack();
            break;
        case 2:
            _done = true;
            music_menu->stop();
            break;
        default:
            break;
        }
    }
    else if(navigating)
    {
        // Navigation
        switch(cursor)
        {
        case 0:
            gui->setButtonState(PLAY_BUTTON_HOVER);
            AudioHandler::AudioHDL::getInstance()->playMenuSelect();
            break;
        case 1:
            gui->setButtonState(OPT_BUTTON_HOVER);
            AudioHandler::AudioHDL::getInstance()->playMenuSelect();
            break;
        case 2:
            gui->setButtonState(QUIT_BUTTON_HOVER);
            AudioHandler::AudioHDL::getInstance()->playMenuSelect();
            break;
        default:
            break;
        }
    }

    validate = false;
    navigating = false;
}


void MainMenu::hover(LX_EventHandler& ev) noexcept
{
    const LX_Physics::LX_FloatPosition P = {fbox<int>(ev.getMouseMotion().x),
                                            fbox<int>(ev.getMouseMotion().y)
                                           };
    static LX_Physics::LX_FloatPosition old_p = {FNIL, FNIL};

    if(LX_Physics::collisionPointBox(P, button_rect[0]) && !LX_Physics::collisionPointBox(old_p, button_rect[0]))
    {
        gui->setButtonState(PLAY_BUTTON_HOVER);
        AudioHandler::AudioHDL::getInstance()->playMenuSelect();
    }
    else if(LX_Physics::collisionPointBox(P, button_rect[1]) && !LX_Physics::collisionPointBox(old_p, button_rect[1]))
    {
        gui->setButtonState(OPT_BUTTON_HOVER);
        AudioHandler::AudioHDL::getInstance()->playMenuSelect();
    }
    else if(LX_Physics::collisionPointBox(P, button_rect[2]) && !LX_Physics::collisionPointBox(old_p, button_rect[2]))
    {
        gui->setButtonState(QUIT_BUTTON_HOVER);
        AudioHandler::AudioHDL::getInstance()->playMenuSelect();
    }

    old_p = P;
}


void MainMenu::mouseClick(LX_EventHandler& ev) noexcept
{
    const LX_Physics::LX_FloatPosition P = {fbox<int>(ev.getMouseButton().x),
                                            fbox<int>(ev.getMouseButton().y)
                                           };

    if(LX_Physics::collisionPointBox(P, button_rect[0]))
    {
        play();
        AudioHandler::AudioHDL::getInstance()->playMenuBack();
    }
    else if(LX_Physics::collisionPointBox(P, button_rect[1]))
    {
        option();
        AudioHandler::AudioHDL::getInstance()->playMenuBack();
    }
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
    ResultInfo info;

    for(int i = FIRST_LEVEL; i <= LAST_LEVEL; i++)
    {
        EngineStatusV gs = target_xplosion->play(info, i);

        if(gs == EngineStatusV::GAME_QUIT)
            break;

        if(gs == EngineStatusV::GAME_FINISH)
            Result::displayResult(info);
        else
        {
            // This line should not be reached
            LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION, "Unknown game state");
            std::abort();
        }

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
    OptionMenu(win).event();
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
    delete [] button_rect;
    delete opt_handler;
    delete gui;
}


void OptionMenu::subEvent() noexcept
{
    const int BTN_OFFSET = 3;

    if(cursor < 0)
        cursor = OptionGUI::NB_BUTTONS - BTN_OFFSET - 1;
    else
        cursor %= OptionGUI::NB_BUTTONS - BTN_OFFSET;


    if(validate)
    {
        call_(cursor, true);
    }
    else if(navigating)
    {
        hover_(cursor);
        AudioHandler::AudioHDL::getInstance()->playMenuSelect();
    }

    validate = false;
    navigating = false;
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
        {
            gamepad();
            AudioHandler::AudioHDL::getInstance()->playMenuBack();
        }
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
        AudioHandler::AudioHDL::getInstance()->playMenuBack();
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
    static LX_Physics::LX_FloatPosition old_p = {FNIL, FNIL};
    const LX_Physics::LX_FloatPosition P =
    {
        fbox<int>(ev.getMouseMotion().x), fbox<int>(ev.getMouseMotion().y)
    };

    int i = 0;
    while(i < OptionGUI::NB_BUTTONS)
    {
        // hit boxes from 7 to 9 are related to the text boxes.
        // I don't need to check them
        if(i < 7 || i > 9)
        {
            if(LX_Physics::collisionPointBox(P, button_rect[i]))
            {
                hover_(i);

                if(!LX_Physics::collisionPointBox(old_p, button_rect[i]))
                    AudioHandler::AudioHDL::getInstance()->playMenuSelect();

                break;
            }
        }

        if(i == 7)
            i = 10;
        else
            i += 1;
    }

    old_p = P;

    if(i == OptionGUI::NB_BUTTONS)
        gui->setButtonState(NORMAL);
}


void OptionMenu::mouseClick(LX_EventHandler& ev) noexcept
{
    const LX_Physics::LX_FloatPosition P =
    {
        fbox<int>(ev.getMouseButton().x), fbox<int>(ev.getMouseButton().y)
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
    if( LX_Device::numberOfDevices() > 0 )
        GamepadMenu(win).event();
}

/** Gamepad menu */

GamepadMenu::GamepadMenu(LX_Win::LX_Window& w)
{
    gui = new GamepadGUI(w);
    button_rect = new LX_Physics::LX_FloatingBox[GamepadGUI::NB_BUTTONS];
    gui->getAABBs(button_rect);
}


void GamepadMenu::hover_(int i) noexcept
{
    switch(i)
    {
    case 1:
        gui->setButtonState(GUI_Button_State::GP_SHOT_HOVER);
        break;

    case 2:
        gui->setButtonState(GUI_Button_State::GP_ROCKET_HOVER);
        break;

    case 3:
        gui->setButtonState(GUI_Button_State::GP_BOMB_HOVER);
        break;

    case 4:
        gui->setButtonState(GUI_Button_State::GP_SMODE_HOVER);
        break;

    default:
        gui->setButtonState(BACK_BUTTON_HOVER);
        break;
    }
}

void GamepadMenu::hover(LX_Event::LX_EventHandler& ev) noexcept
{
    static LX_Physics::LX_FloatPosition old_p = {FNIL, FNIL};
    const LX_Physics::LX_FloatPosition P =
    {
        fbox<int>(ev.getMouseMotion().x), fbox<int>(ev.getMouseMotion().y)
    };

    gui->setButtonState(NORMAL);

    int i = 0;
    while(i < GamepadGUI::NB_BUTTONS)
    {
        if(LX_Physics::collisionPointBox(P, button_rect[i]))
        {
            if(i == 0)
                gui->setButtonState(BACK_BUTTON_HOVER);
            else
                hover_(i);

            if(!LX_Physics::collisionPointBox(old_p, button_rect[i]))
                AudioHandler::AudioHDL::getInstance()->playMenuSelect();

            break;
        }
        ++i;
    }

    old_p = P;
}

void GamepadMenu::ignoreInput_() noexcept
{
    LX_Event::LX_EventHandler::ignoreEvent(LX_EventType::QUIT);
    LX_Event::LX_EventHandler::ignoreEvent(LX_EventType::WINDOWEVENT);
    LX_Event::LX_EventHandler::ignoreEvent(LX_EventType::KEYDOWN);
    LX_Event::LX_EventHandler::ignoreEvent(LX_EventType::KEYUP);
    LX_Event::LX_EventHandler::ignoreEvent(LX_EventType::MOUSEBUTTONDOWN);
    LX_Event::LX_EventHandler::ignoreEvent(LX_EventType::MOUSEBUTTONUP);
    LX_Event::LX_EventHandler::ignoreEvent(LX_EventType::MOUSEMOTION);
    LX_Event::LX_EventHandler::ignoreEvent(LX_EventType::MOUSEWHEEL);
    LX_Event::LX_EventHandler::ignoreEvent(LX_EventType::CONTROLLERBUTTONDOWN);
    LX_Event::LX_EventHandler::ignoreEvent(LX_EventType::CONTROLLERDEVICEADDED);
    LX_Event::LX_EventHandler::ignoreEvent(LX_EventType::CONTROLLERDEVICEREMOVED);
    LX_Event::LX_EventHandler::ignoreEvent(LX_EventType::USEREVENT);
}

void GamepadMenu::restoreInput_() noexcept
{
    LX_Event::LX_EventHandler::processEvent(LX_EventType::QUIT);
    LX_Event::LX_EventHandler::processEvent(LX_EventType::WINDOWEVENT);
    LX_Event::LX_EventHandler::processEvent(LX_EventType::KEYDOWN);
    LX_Event::LX_EventHandler::processEvent(LX_EventType::KEYUP);
    LX_Event::LX_EventHandler::processEvent(LX_EventType::MOUSEBUTTONDOWN);
    LX_Event::LX_EventHandler::processEvent(LX_EventType::MOUSEBUTTONUP);
    LX_Event::LX_EventHandler::processEvent(LX_EventType::MOUSEMOTION);
    LX_Event::LX_EventHandler::processEvent(LX_EventType::MOUSEWHEEL);
    LX_Event::LX_EventHandler::processEvent(LX_EventType::CONTROLLERBUTTONDOWN);
    LX_Event::LX_EventHandler::processEvent(LX_EventType::CONTROLLERDEVICEADDED);
    LX_Event::LX_EventHandler::processEvent(LX_EventType::CONTROLLERDEVICEREMOVED);
    LX_Event::LX_EventHandler::processEvent(LX_EventType::USEREVENT);
}

void GamepadMenu::beforeClick_(int i) noexcept
{
    switch(i)
    {
    case 1:
        gui->setButtonState(GUI_Button_State::GP_SHOT_CLICK);
        break;

    case 2:
        gui->setButtonState(GUI_Button_State::GP_ROCKET_CLICK);
        break;

    case 3:
        gui->setButtonState(GUI_Button_State::GP_BOMB_CLICK);
        break;

    case 4:
        gui->setButtonState(GUI_Button_State::GP_SMODE_CLICK);
        break;

    default:
        break;
    }

    gui->draw();
}

void GamepadMenu::click_(int i) noexcept
{
    LX_Event::LX_EventHandler ev;

    beforeClick_(i);
    // I must do this because I just want to get a controller event
    ignoreInput_();
    while( !ev.pollEvent() || ev.getEventType() != LX_EventType::CONTROLLERBUTTONUP );
    restoreInput_();

    afterClick_(ev, i);
}

void GamepadMenu::afterClick_(const LX_Event::LX_EventHandler& ev, int i) noexcept
{
    const UTF8string U8STR = stringOfButton(ev.getButton().value);
    GPconfig::GamepadControl gpcontrol;

    switch(i)
    {
    case 1:
        gpcontrol.updateControl(GPconfig::ActionControl::SHOT, U8STR);
        gui->setButtonState(GUI_Button_State::GP_CMD_CHANGE);
        break;

    case 2:
        gpcontrol.updateControl(GPconfig::ActionControl::ROCKET, U8STR);
        gui->setButtonState(GUI_Button_State::GP_CMD_CHANGE);
        break;

    case 3:
        gpcontrol.updateControl(GPconfig::ActionControl::BOMB, U8STR);
        gui->setButtonState(GUI_Button_State::GP_CMD_CHANGE);
        break;

    case 4:
        gpcontrol.updateControl(GPconfig::ActionControl::SLOW, U8STR);
        gui->setButtonState(GUI_Button_State::GP_CMD_CHANGE);
        break;

    default:
        break;
    }

    gui->getAABBs(button_rect);
}


void GamepadMenu::mouseClick(LX_Event::LX_EventHandler& ev) noexcept
{
    const LX_Physics::LX_FloatPosition P =
    {
        fbox<int>(ev.getMouseButton().x), fbox<int>(ev.getMouseButton().y)
    };

    if(LX_Physics::collisionPointBox(P, button_rect[0]))
    {
        gui->setButtonState(NORMAL);
        _done = true;
    }
    else
    {
        int i = 1;
        while(i < GamepadGUI::NB_BUTTONS)
        {
            if(LX_Physics::collisionPointBox(P, button_rect[i]))
            {
                click_(i);
                break;
            }
            ++i;
        }
    }
}

void GamepadMenu::subEvent() noexcept
{
    const int BTN_OFFSET = 3;

    if(cursor < 0)
        cursor = GamepadGUI::NB_BUTTONS - 1;
    else
        cursor %= GamepadGUI::NB_BUTTONS;

    if(validate)
    {
        if(cursor == 0)
            _done = true;
        else
            click_(cursor);
    }
    else if(navigating)
    {
        gui->setButtonState(NORMAL);
        hover_(cursor);
        AudioHandler::AudioHDL::getInstance()->playMenuSelect();
    }

    validate = false;
    navigating = false;
}

GamepadMenu::~GamepadMenu()
{
    delete [] button_rect;
}
