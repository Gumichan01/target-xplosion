
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


#include "Menu.hpp"
#include "GUI.hpp"
#include "../game/Game.hpp"
#include "../game/Rank.hpp"
#include "../game/Result.hpp"
#include "../option/VolumeHandler.hpp"

#include <LunatiX/LX_AABB.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Timer.hpp>

#include <SDL2/SDL_events.h>


namespace
{
const uint32_t SLEEP = 33;
};


/** Menu */

Menu::Menu() : gui(nullptr) {}

void Menu::event()
{
    SDL_Event ev;
    bool done = false;

    while(!done)
    {
        while(SDL_PollEvent(&ev))
        {
            switch(ev.type)
            {
            case SDL_QUIT:
                done = true;
                break;

            case SDL_MOUSEBUTTONUP:
                mouseClick(ev, done);
                break;

            case SDL_MOUSEMOTION:
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

Menu::~Menu() {}


/** Main menu */

MainMenu::MainMenu(LX_Win::LX_Window& w) : button_rect(nullptr),win(w)
{
    gui = new MainGUI(w);
    button_rect = new LX_AABB[MainGUI::NB_BUTTONS];
    gui->getAABBs(button_rect);
    Option::VolumeHandler();
}


MainMenu::~MainMenu()
{
    delete [] button_rect;
    delete gui;
}


void MainMenu::hover(SDL_Event& ev)
{
    const LX_Physics::LX_Point p(ev.motion.x,ev.motion.y);

    if(LX_Physics::collisionPointRect(p,button_rect[0]))
        gui->setButtonState(PLAY_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p,button_rect[1]))
        gui->setButtonState(OPT_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p,button_rect[2]))
        gui->setButtonState(QUIT_BUTTON_HOVER);

    else
        gui->setButtonState(NORMAL);
}


void MainMenu::mouseClick(SDL_Event& ev, bool& done)
{
    const LX_Physics::LX_Point p(ev.button.x,ev.button.y);

    if(LX_Physics::collisionPointRect(p,button_rect[0]))
    {
        play();
        gui->setButtonState(NORMAL);
    }
    else if(LX_Physics::collisionPointRect(p,button_rect[1]))
    {
        option();
        gui->setButtonState(NORMAL);
    }
    else if(LX_Physics::collisionPointRect(p,button_rect[2]))
        done = true;
}


void MainMenu::play()
{
    /// @todo Select the mode (Normal mode, Arcade mode)
    Rank::init();
    ResultInfo info = {0,0,0,0,0,0};
    Game *target_xplosion = Game::init();             // Load the game instance

    for(int i = 1; i < 2; i++)
    {
        Rank::setRank(S_RANK);
        if(target_xplosion->play(info,i) == GAME_FINISH)
        {
            Result::displayResult(info);
        }

        info.nb_killed_enemies = 0;
        info.max_nb_enemies = 0;
        info.nb_death = 0;
        info.score = 0;
    }
    Game::destroy();
}


void MainMenu::option()
{
    OptionMenu opt(win);
    opt.event();
}


/** Option menu */

OptionMenu::OptionMenu(LX_Win::LX_Window& w) : button_rect(nullptr),vhandler(nullptr)
{
    gui = new OptionGUI(w);
    vhandler = new Option::VolumeHandler();
    button_rect = new LX_AABB[OptionGUI::NB_BUTTONS];
    gui->getAABBs(button_rect);
}


OptionMenu::~OptionMenu()
{
    delete [] button_rect;
    delete vhandler;
    delete gui;
}


void OptionMenu::hover(SDL_Event& ev)
{
    const LX_Physics::LX_Point p(ev.motion.x,ev.motion.y);

    if(LX_Physics::collisionPointRect(p,button_rect[0]))
        gui->setButtonState(GP_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p,button_rect[1]))
        gui->setButtonState(BACK_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p,button_rect[2]))
        gui->setButtonState(OVD_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p,button_rect[3]))
        gui->setButtonState(OVU_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p,button_rect[4]))
        gui->setButtonState(MUD_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p,button_rect[5]))
        gui->setButtonState(MUU_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p,button_rect[6]))
        gui->setButtonState(FXD_BUTTON_HOVER);

    else if(LX_Physics::collisionPointRect(p,button_rect[7]))
        gui->setButtonState(FXU_BUTTON_HOVER);

    else
        gui->setButtonState(NORMAL);
}

/// @todo [2] click on the arrows
/**
    Create a VolumeHandler object
    VolumeHandler will be the interface between the menu/GUI and LX_Mixer.
    It is also the class that saves the volume configuration in a file.
    The volume can be retrieved in integer or string value.

    Algo :

    Go to the option menu → Create the Volume Handler and load the file (vo.txconf)
    if possible.
        - If OK save it into the object
        - Otherwise get the value from LX_Mixer

    When the user modify (increase/decrease) a value of the volume, change it
    in LX_Mixer using VolumeHandler and display the updated volume

    when the user go back to the main menu, everything is saved in vo.txconf

*/
void OptionMenu::mouseClick(SDL_Event& ev, bool& done)
{
    const LX_Physics::LX_Point p(ev.button.x,ev.button.y);

    if(LX_Physics::collisionPointRect(p,button_rect[0]));
    /// @todo [3] gamepad menu
    else if(LX_Physics::collisionPointRect(p,button_rect[1]))
    {
        gui->setButtonState(NORMAL);
        done = true;
    }
}

