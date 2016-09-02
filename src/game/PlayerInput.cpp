
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

#include "PlayerInput.hpp"
#include "Game.hpp"
#include "Rank.hpp"

#include "../entities/Player.hpp"
#include "../entities/Missile.hpp"

#include <LunatiX/LX_Timer.hpp>
#include <SDL2/SDL_events.h>

namespace PlayerInput
{

// Private fields
static bool continuous_shot = false;    // Continuous shot for the joystick input
const short JOYSTICK_DEAD_ZONE = 8000;
const short JOYSTICK_HIGH_ZONE = 24000;

void regulateShot(Player& p);
void playerShot(Player& p);

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
    const unsigned int r = Rank::getRank();

    if(r == C_RANK)
    {
        p.fire(MISSILE_TYPE::BASIC_MISSILE_TYPE);
    }
    else if(r == B_RANK)
    {
        p.fire(MISSILE_TYPE::DOUBLE_MISSILE_TYPE);
    }
    else if(r == A_RANK)
    {
        p.fire(MISSILE_TYPE::BASIC_MISSILE_TYPE);
        p.fire(MISSILE_TYPE::WAVE_MISSILE_TYPE);
    }
    else
    {
        p.fire(MISSILE_TYPE::DOUBLE_MISSILE_TYPE);
        p.fire(MISSILE_TYPE::WAVE_MISSILE_TYPE);
    }
}


/*
    Gets the player and the condition loop to retrieve input
    from the player and modify its state if necessary
*/
void input(Player& p, bool& done)
{
    SDL_Event event;
    Game *g = Game::getInstance();

    // Check the state of the input devices
    keyboardState(p);
    joystickState(p);

    // Handle input
    while(SDL_PollEvent(&event))
    {
        inputJoystickAxis(event,p);
        inputJoystickButton(event,p);

        switch(event.type)
        {
        case SDL_QUIT:
            done = true;
            break;

        case SDL_KEYUP:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE :
                g->pause();
                LX_Timer::delay(8000);
                g->resume();
                break;

            default :
                inputKeyboard(event,p);
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
    const uint8_t *KEYS = SDL_GetKeyboardState(nullptr);
    int player_sp = PLAYER_SPEED;

    // Left shift is maitained -> slow mode
    if(KEYS[SDL_GetScancodeFromKey(SDLK_LSHIFT)])
        player_sp /= 2;

    if(KEYS[SDL_SCANCODE_UP])
        p.setYvel(-player_sp);

    if(KEYS[SDL_SCANCODE_DOWN])
        p.setYvel(player_sp);

    if(KEYS[SDL_SCANCODE_LEFT])
        p.setXvel(-player_sp);

    if(KEYS[SDL_SCANCODE_RIGHT])
        p.setXvel(player_sp);

    if(KEYS[SDL_GetScancodeFromKey(SDLK_w)])
    {
        regulateShot(p);
    }
}

void joystickState(Player& p)
{
    if(continuous_shot)
        regulateShot(p);
}

void inputKeyboard(SDL_Event& event, Player& p)
{
    if(p.isDead())
        return;

    switch(event.key.keysym.sym)
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
        Game::getInstance()->acceptPlayerInput();
        break;

    default :
        break;
    }
}

void inputJoystickAxis(SDL_Event& event, Player& p)
{
    if(event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP)
    {
        if(event.jbutton.which == 0)   // The first joystick
        {
            if(event.jbutton.button == 0)
            {
                if(event.jbutton.state == SDL_PRESSED)
                    p.fire(ROCKET_TYPE);
            }

            if(event.jbutton.button == 1)
            {
                if(event.jbutton.state == SDL_PRESSED)
                    p.fire(BOMB_TYPE);
            }

            if(event.jbutton.button == 7)
            {
                if(event.jbutton.state == SDL_PRESSED)
                    continuous_shot = true;
                else if(event.jbutton.state == SDL_RELEASED)
                    continuous_shot = false;
            }
        }
    }           // If event.type
}

void inputJoystickButton(SDL_Event& event, Player& p)
{
    if(event.type == SDL_JOYAXISMOTION)
    {
        if(event.jaxis.which == 0) // The first joystick
        {
            if(event.jaxis.axis == 0)  /// X axis
            {
                if(event.jaxis.value < -JOYSTICK_HIGH_ZONE)
                {
                    p.setXvel(-PLAYER_SPEED);
                }
                else if(event.jaxis.value > JOYSTICK_HIGH_ZONE)
                {
                    p.setXvel(PLAYER_SPEED);
                }
                else if(event.jaxis.value < -JOYSTICK_DEAD_ZONE)
                {
                    p.setXvel(-(PLAYER_SPEED/2));
                }
                else if(event.jaxis.value > JOYSTICK_DEAD_ZONE)
                {
                    p.setXvel(PLAYER_SPEED/2);
                }
                else
                    p.setXvel(0);
            }
            else if(event.jaxis.axis == 1) /// Y axis
            {
                if(event.jaxis.value < -JOYSTICK_HIGH_ZONE)
                {
                    p.setYvel(-PLAYER_SPEED);
                }
                else if(event.jaxis.value > JOYSTICK_HIGH_ZONE)
                {
                    p.setYvel(PLAYER_SPEED);
                }
                else if(event.jaxis.value < -JOYSTICK_DEAD_ZONE)
                {
                    p.setYvel(-(PLAYER_SPEED/2));
                }
                else if(event.jaxis.value > JOYSTICK_DEAD_ZONE)
                {
                    p.setYvel(PLAYER_SPEED/2);
                }
                else
                    p.setYvel(0);
            }
        }       // If event.jaxis.which == 0
    }           // If event.type == SDL_JOYAXISMOTION
}

};
