
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

#include "PlayerInput.hpp"

#include "../../entities/Player.hpp"
#include "../../entities/Missile.hpp"
#include "../../resources/WinID.hpp"
#include "../../option/GamepadControl.hpp"

#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Event.hpp>
#include <LunatiX/LX_Log.hpp>

#include <cstdio>
#include <ctime>

using namespace LX_Event;
using namespace FloatBox;

namespace PlayerInput
{

// Private fields
static bool continuous_shot = false;    // Continuous shot for the joystick input
static bool slow_mode = false;          // Slow mode
static bool kboard_event = false;          // Slow mode

const short JOYSTICK_DEAD_ZONE = 8000;
const short JOYSTICK_HIGH_ZONE = 32000;
const float JOYSTICK_HIGH_ZONE_F = static_cast<float>(JOYSTICK_HIGH_ZONE);
const int SHOT_FRAMES = 6;

const UTF8string A_BUTTON("a");
const UTF8string B_BUTTON("b");
const UTF8string X_BUTTON("x");
const UTF8string RB_BUTTON("rightshoulder");
const UTF8string LB_BUTTON("leftshoulder");
const UTF8string START_BUTTON("start");

void regulateShot(Player& p) noexcept;
void screenshot(LX_Win::LX_Window& win) noexcept;


void regulateShot(Player& p) noexcept
{
    static char freq = 1;

    if(freq%SHOT_FRAMES == 0)
    {
        if(!p.isDead() && !p.isDying())
        {
            p.normalShot();
            freq = 1;
        }
    }
    else
        freq += 1;
}

void screenshot(LX_Win::LX_Window& win) noexcept
{
    if(LX_Log::isDebugMode())
    {
        static int id_screen = 1;

        const size_t SZ = 256;
        char datestr[SZ] = {'\0'};
        char name[SZ] = {'\0'};

        time_t t = std::time(nullptr);
        struct tm *tmp = std::localtime(&t);

        std::strftime(datestr, SZ, "%Y-%m-%d_%H-%M-%S-tx", tmp);
        std::sprintf(name, "%s-%d.png", datestr, id_screen++);
        win.screenshot(name);
    }
}


/*
    Gets the player and the condition loop to retrieve input
    from the player and modify its state if necessary
*/
void input(Player& p, bool& done) noexcept
{
    using LX_Win::LX_WindowManager;
    LX_EventHandler event;

    // Check the state of the input devices
    if(kboard_event)
        keyboardState(p);
    else
        joystickState(p);

    // Handle input
    while(event.pollEvent())
    {
        inputJoystickAxis(event, p);
        inputJoystickButton(event, p);

        switch(event.getEventType())
        {
        case LX_EventType::QUIT:
            done = true;
            break;

        case LX_EventType::KEYDOWN:
            kboard_event = true;
            break;

        case LX_EventType::KEYUP:
            kboard_event = true;
            switch(event.getKeyCode())
            {
            case SDLK_ESCAPE:
                done = true;
                break;

            case SDLK_p:
                screenshot(LX_WindowManager::getInstance().getWindow(WinID::getWinID()));
                break;

            default:
                inputKeyboard(event, p);
                break;
            }
            break;

        case LX_EventType::CONTROLLERBUTTONUP:
            if(stringOfButton(event.getButton().value) == START_BUTTON)
                done = true;
            break;

        default:
            break;
        }
    }
}

void keyboardState(Player& p) noexcept
{
    const uint8_t *KEYS = LX_EventHandler::getKeyboardState().state;
    float player_sp = Player::PLAYER_SPEED;

    // Left shift is pressed -> slow mode
    slow_mode = static_cast<bool>(KEYS[getScanCodeFrom(SDLK_LSHIFT)]);

    if(slow_mode)
    {
        player_sp /= Player::PLAYER_SPEED_RATIO;
    }

    if(KEYS[SDL_SCANCODE_UP])
        p.setYvel(-player_sp);

    if(KEYS[SDL_SCANCODE_DOWN])
        p.setYvel(player_sp);

    if(KEYS[SDL_SCANCODE_LEFT])
        p.setXvel(-player_sp);

    if(KEYS[SDL_SCANCODE_RIGHT])
        p.setXvel(player_sp);

    if(KEYS[getScanCodeFrom(SDLK_w)] || KEYS[getScanCodeFrom(SDLK_z)])
    {
        regulateShot(p);
    }

    p.notifySlow(slow_mode);
}

void joystickState(Player& p) noexcept
{
    if(continuous_shot)
        regulateShot(p);

    p.notifySlow(slow_mode);
}

void inputKeyboard(const LX_EventHandler& event, Player& p) noexcept
{
    if(p.isDead() || p.isDying())
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
    case SDLK_z:
        p.normalShot();
        break;

    // Rocket
    case SDLK_x:
        p.rocketShot();
        break;

    // Bomb
    case SDLK_c:
        p.bombShot();
        break;

    // Screenshot
    case SDLK_p:
        screenshot(LX_Win::LX_WindowManager::getInstance().getWindow(WinID::getWinID()));
        break;

    default :
        break;
    }
}

void inputJoystickAxis(const LX_EventHandler& event, Player& p) noexcept
{
    if(event.getEventType() == LX_EventType::CONTROLLERAXISMOTION)
    {
        const LX_GAxis AX = event.getAxis();
        const float VP = static_cast<float>(AX.value) * Player::PLAYER_SPEED / JOYSTICK_HIGH_ZONE_F;

        kboard_event = false;

        if(AX.id == 0) // The first joystick
        {
            if(AX.value < -JOYSTICK_DEAD_ZONE || AX.value > JOYSTICK_DEAD_ZONE)
            {
                if(AX.axis == LX_GamepadAxis::LEFTX)        /// X axis
                    p.setXvel(VP);

                else if(AX.axis == LX_GamepadAxis::LEFTY)   /// Y axis
                    p.setYvel(VP);
            }
            else
            {
                if(AX.axis == LX_GamepadAxis::LEFTX)        /// X axis
                    p.setXvel(0.0f);

                else if(AX.axis == LX_GamepadAxis::LEFTY)   /// Y axis
                    p.setYvel(0.0f);
            }
        }       // If event.caxis.which == 0
    }           // If event.type == LX_JOYAXISMOTION
}

void inputJoystickButton(const LX_EventHandler& event, Player& p) noexcept
{
    if(p.isDead() || p.isDying())
        return;

    if(event.getEventType() == LX_EventType::CONTROLLERBUTTONDOWN
            || event.getEventType() == LX_EventType::CONTROLLERBUTTONUP)
    {
        using GPconfig::GamepadControl;
        using GPconfig::ActionControl;

        const LX_GButton bu = event.getButton();
        kboard_event = false;

        if(bu.which == 0)   // The first joystick
        {
            if(stringOfButton(bu.value) == GamepadControl::getControl(ActionControl::ROCKET))
            {
                if(bu.state == LX_State::RELEASED)
                    p.rocketShot();
            }

            if(stringOfButton(bu.value) == GamepadControl::getControl(ActionControl::BOMB))
            {
                if(bu.state == LX_State::RELEASED)
                    p.bombShot();
            }

            if(stringOfButton(bu.value) == GamepadControl::getControl(ActionControl::SHOT))
            {
                if(bu.state == LX_State::PRESSED)
                    continuous_shot = true;
                else    // RELEASED
                    continuous_shot = false;
            }

            if(stringOfButton(bu.value) == GamepadControl::getControl(ActionControl::SLOW))
            {
                if(bu.state == LX_State::PRESSED)
                    slow_mode = true;
                else    // RELEASED
                    slow_mode = false;
            }
        }
    }           // If event.type
}

}
