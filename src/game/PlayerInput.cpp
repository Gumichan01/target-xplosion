
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
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "PlayerInput.hpp"
#include "Game.hpp"

#include "../entities/Player.hpp"
#include "../entities/Missile.hpp"

#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Event.hpp>

#include <ctime>

using namespace LX_Event;


namespace PlayerInput
{

// Private fields
static bool continuous_shot = false;    // Continuous shot for the joystick input
const short JOYSTICK_DEAD_ZONE = 8000;
const short JOYSTICK_HIGH_ZONE = 24000;

void regulateShot(Player& p);
void playerShot(Player& p);
void screenshot(LX_Win::LX_Window *win);

void regulateShot(Player& p)
{
    static char freq = 1;

    if(freq%6 == 0)
    {
        if(!p.isDead())
        {
            playerShot(p);
            freq = 1;
        }
    }
    else
        freq += 1;
}

void playerShot(Player& p)
{
    p.fire(MISSILE_TYPE::DOUBLE_MISSILE_TYPE);
    p.fire(MISSILE_TYPE::WAVE_MISSILE_TYPE);
}


void screenshot(LX_Win::LX_Window *win)
{
    if(win != nullptr)
    {
        static int id_screen = 1;

        const size_t SZ = 256;
        char datestr[SZ] = {'\0'};
        char name[SZ] = {'\0'};

        time_t t = time(nullptr);
        struct tm *tmp = localtime(&t);

        strftime(datestr, SZ, "%Y-%m-%d_%H-%M-%S-tx", tmp);
        sprintf(name, "%s-%d.png", datestr, id_screen++);
        win->screenshot(name);
    }
}


/*
    Gets the player and the condition loop to retrieve input
    from the player and modify its state if necessary
*/
void input(Player& p, bool& done)
{
    LX_EventHandler event;

    // Check the state of the input devices
    keyboardState(p);
    joystickState(p);

    // Handle input
    while(event.pollEvent())
    {
        inputJoystickAxis(event, p);
        inputJoystickButton(event, p);

        switch(event.getEventType())
        {
        case LX_QUIT:
            done = true;
            break;

        case LX_KEYUP:
            switch(event.getKeyCode())
            {
            case SDLK_ESCAPE:
                done = true;
                break;

            case SDLK_p:
                screenshot(LX_Win::LX_WindowManager::getInstance()->getWindow(1));
                break;

            default :
                inputKeyboard(event, p);
                break;
            }
            break;

        default:
            break;
        }
    }
}

void keyboardState(Player& p)
{
    const uint8_t *KEYS = LX_EventHandler::getKeyboardState().state;
    int player_sp = PLAYER_SPEED;

    // Left shift is pressed -> slow mode
    if(KEYS[getScanCodeFrom(SDLK_LSHIFT)])
        player_sp /= 2;

    if(KEYS[SDL_SCANCODE_UP])
        p.setYvel(-player_sp);

    if(KEYS[SDL_SCANCODE_DOWN])
        p.setYvel(player_sp);

    if(KEYS[SDL_SCANCODE_LEFT])
        p.setXvel(-player_sp);

    if(KEYS[SDL_SCANCODE_RIGHT])
        p.setXvel(player_sp);

    if(KEYS[getScanCodeFrom(SDLK_w)])
    {
        regulateShot(p);
    }
}

void joystickState(Player& p)
{
    if(continuous_shot)
        regulateShot(p);
}

void inputKeyboard(LX_EventHandler& event, Player& p)
{
    if(p.isDead())
        return;

    switch(event.getKeyCode())
    {
    // Left/Right
    case SDLK_RIGHT:
    case SDLK_LEFT:
        p.setXvel(0);
        break;

    // Up/Down
    case SDLK_UP:
    case SDLK_DOWN:
        p.setYvel(0);
        break;

    // Shot
    case SDLK_w:
        playerShot(p);
        break;

    // Rocket
    case SDLK_x:
        p.fire(MISSILE_TYPE::ROCKET_TYPE);
        break;

    // Bomb
    case SDLK_c:
        p.fire(MISSILE_TYPE::BOMB_TYPE);
        break;

    // Screenshot
    case SDLK_p:
        screenshot(LX_Win::LX_WindowManager::getInstance()->getWindow(1));
        break;

    default :
        break;
    }
}

void inputJoystickAxis(LX_EventHandler& event, Player& p)
{
    if(event.getEventType() == LX_CONTROLLERAXISMOTION)
    {
        const LX_GAxis ax = event.getAxis();

        if(ax.id == 0) // The first joystick
        {
            if(ax.axis == 0)  /// X axis
            {
                if(ax.value < -JOYSTICK_HIGH_ZONE)
                {
                    p.setXvel(-PLAYER_SPEED);
                }
                else if(ax.value > JOYSTICK_HIGH_ZONE)
                {
                    p.setXvel(PLAYER_SPEED);
                }
                else if(ax.value < -JOYSTICK_DEAD_ZONE)
                {
                    p.setXvel(-(PLAYER_SPEED/2));
                }
                else if(ax.value > JOYSTICK_DEAD_ZONE)
                {
                    p.setXvel(PLAYER_SPEED/2);
                }
                else
                    p.setXvel(0);
            }
            else if(ax.axis == 1) /// Y axis
            {
                if(ax.value < -JOYSTICK_HIGH_ZONE)
                {
                    p.setYvel(-PLAYER_SPEED);
                }
                else if(ax.value > JOYSTICK_HIGH_ZONE)
                {
                    p.setYvel(PLAYER_SPEED);
                }
                else if(ax.value < -JOYSTICK_DEAD_ZONE)
                {
                    p.setYvel(-(PLAYER_SPEED/2));
                }
                else if(ax.value > JOYSTICK_DEAD_ZONE)
                {
                    p.setYvel(PLAYER_SPEED/2);
                }
                else
                    p.setYvel(0);
            }
        }       // If event.caxis.which == 0
    }           // If event.type == LX_JOYAXISMOTION
}

void inputJoystickButton(LX_EventHandler& event, Player& p)
{
    if(event.getEventType() == LX_CONTROLLERBUTTONDOWN
            || event.getEventType() == LX_CONTROLLERBUTTONUP)
    {
        const LX_GButton bu = event.getButton();

        if(bu.which == 0)   // The first joystick
        {
            if(bu.value == 0)
            {
                if(bu.state == LX_BUTTON_PRESSED)
                    p.fire(ROCKET_TYPE);
            }

            if(bu.which == 1)
            {
                if(bu.state == LX_BUTTON_PRESSED)
                    p.fire(BOMB_TYPE);
            }

            if(bu.which == 7)
            {
                if(bu.state == LX_BUTTON_PRESSED)
                    continuous_shot = true;
                else if(bu.state == LX_BUTTON_RELEASED)
                    continuous_shot = false;
            }
        }
    }           // If event.type
}

};
