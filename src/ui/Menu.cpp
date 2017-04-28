
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
#include <LunatiX/LX_Log.hpp>
#include <LunatiX/LX_Event.hpp>
#include <LunatiX/LX_Device.hpp>


using namespace LX_Event;

namespace
{
const uint32_t SLEEP = 33;
};


/** Menu */

Menu::Menu() : gui(nullptr), button_rect(nullptr) {}

void Menu::event()
{
    LX_EventHandler ev;
    bool done = false;

    while(!done)
    {
        while(ev.pollEvent())
        {
            switch(ev.getEventType())
            {
            case LX_QUIT:
                done = true;
                break;

            case LX_MOUSEBUTTONUP:
                mouseClick(ev, done);
                break;

            case LX_MOUSEMOTION:
                hover(ev);
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

MainMenu::MainMenu(LX_Win::LX_Window& w) : win(w), music_menu(nullptr)
{
    gui = new MainGUI(w);
    music_menu = new LX_Mixer::LX_Music(TX_Asset::getInstance()->getLevelMusic(0));
    button_rect = new LX_AABB[MainGUI::NB_BUTTONS];
    gui->getAABBs(button_rect);
    music_menu->play(LX_Mixer::LX_MIXER_LOOP);
    Option::OptionHandler op;

    if(op.getFullscreenFlag() == static_cast<uint8_t>(1))
        win.toggleFullscreen(LX_Win::LX_WINDOW_FULLSCREEN);
}


MainMenu::~MainMenu()
{
    delete music_menu;
    delete gui;
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


void OptionMenu::hover(LX_EventHandler& ev)
{
    const LX_Physics::LX_Point p(ev.getMouseMotion().x, ev.getMouseMotion().y);

    if(LX_Physics::collisionPointRect(p, button_rect[0]))
        gui->setButtonState(GP_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[1]))
        gui->setButtonState(BACK_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[2]))
        gui->setButtonState(OVD_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[3]))
        gui->setButtonState(OVU_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[4]))
        gui->setButtonState(MUD_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[5]))
        gui->setButtonState(MUU_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[6]))
        gui->setButtonState(FXD_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[7]))
        gui->setButtonState(FXU_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p, button_rect[8]))
        gui->setButtonState(FS_BUTTON_HOVER);

    else
        gui->setButtonState(NORMAL);
}


void OptionMenu::mouseClick(LX_EventHandler& ev, bool& done)
{
    const LX_Physics::LX_Point p(ev.getMouseButton(). x, ev.getMouseButton().y);
    OptionGUI *opt_gui = dynamic_cast<OptionGUI*>(gui);

    /// @todo (#2#) v0.4.7 gamepad menu
    if(LX_Physics::collisionPointRect(p, button_rect[0]))
        gamepad();
    else if(LX_Physics::collisionPointRect(p, button_rect[1]))
    {
        gui->setButtonState(NORMAL);
        done = true;
    }
    else
    {
        if(opt_gui != nullptr)
        {
            if(LX_Physics::collisionPointRect(p, button_rect[2]))
                opt_gui->updateVolume(OVD_BUTTON_CLICK,*opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[3]))
                opt_gui->updateVolume(OVU_BUTTON_CLICK,*opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[4]))
                opt_gui->updateVolume(MUD_BUTTON_CLICK,*opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[5]))
                opt_gui->updateVolume(MUU_BUTTON_CLICK,*opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[6]))
                opt_gui->updateVolume(FXD_BUTTON_CLICK,*opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[7]))
                opt_gui->updateVolume(FXU_BUTTON_CLICK,*opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[8]))
                opt_gui->updateFullscreen(FS_BUTTON_CLICK,*opt_handler);
            else if(LX_Physics::collisionPointRect(p, button_rect[9]))
                opt_gui->updateTextVolume(OV_TEXT_CLICK, *opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[10]))
                opt_gui->updateTextVolume(MU_TEXT_CLICK, *opt_handler);

            else if(LX_Physics::collisionPointRect(p, button_rect[11]))
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

GamepadMenu::GamepadMenu(LX_Win::LX_Window& w): gamepad()
{
    gui = new GamepadGUI(w);
    button_rect = new LX_AABB[GamepadGUI::NB_BUTTONS];
    gui->getAABBs(button_rect);
    loadGamepad();
}


void GamepadMenu::loadGamepad()
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


void GamepadMenu::hover(LX_Event::LX_EventHandler& ev)
{
    /// @todo gamepad hover button
    const LX_Physics::LX_Point p(ev.getMouseButton(). x, ev.getMouseButton().y);

    if(LX_Physics::collisionPointRect(p, button_rect[0]))
        gui->setButtonState(BACK_BUTTON_HOVER);
    else
        gui->setButtonState(NORMAL);
}

void GamepadMenu::mouseClick(LX_Event::LX_EventHandler& ev, bool& done)
{
    /// @todo gamepad buttons - click
    const LX_Physics::LX_Point p(ev.getMouseButton(). x, ev.getMouseButton().y);

    if(LX_Physics::collisionPointRect(p, button_rect[0]))
    {
        gui->setButtonState(NORMAL);
        done = true;
    }
}


GamepadMenu::~GamepadMenu()
{
    gamepad.close();
}
