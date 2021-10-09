/**
 * This file is part of mattRPG.
 *
 * mattRPG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mattRPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mattRPG.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "SDL.h"

#include "GameApplication.hpp"
#include "InputManager.hpp"

void InputManager::HandleInput(SDL_Event* input_event) {

    current_event = input_event;

    // TODO: This double-switching on the event is probably a bad idea.
    // Only events explicity handled by input manager are listed here.
    // For a handling of all event types, see GameApplication::Loop.
    switch(current_event->type) {
        case SDL_CONTROLLERAXISMOTION:
            HandleControllerAxis();
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            HandleControllerInputPress();
            break;
        case SDL_CONTROLLERBUTTONUP:
            HandleControllerInputRelease();
            break;
        case SDL_FINGERMOTION:
            HandleTouchMotion();
            break;
        case SDL_FINGERDOWN:
            HandleTouchPress();
            break;
        case SDL_FINGERUP:
            HandleTouchRelease();
            break;
        case SDL_KEYDOWN:
            HandleKeyboardInputPress();
            break;
        case SDL_KEYUP:
            HandleKeyboardInputRelease();
            break;
        case SDL_JOYAXISMOTION:
            //HandleJoystickMotion();
            break;
        case SDL_JOYBALLMOTION:
            //HandleJoystickTrackball();
            break;
        case SDL_JOYHATMOTION:
            //HandleJoystickHat();
            break;
        case SDL_JOYBUTTONDOWN:
            //HandleJoystickButtonPress();
            break;
        case SDL_JOYBUTTONUP:
            //HandleJoystickButtonRelease();
            break;
        case SDL_MOUSEMOTION:
            HandleMouseMovement();
            break;
        case SDL_MOUSEBUTTONDOWN:
            HandleMouseButtonPress();
            break;
        case SDL_MOUSEBUTTONUP:
            HandleMouseButtonRelease();
            break;
        case SDL_MOUSEWHEEL:
            HandleMouseWheel();
            break;
        case SDL_MULTIGESTURE:
            HandleTouchMultiTouch();
            break;
        default:
            break;
    }
}

void InputManager::HandleKeyboardInputPress() {
    
    if(debug_inputs) {
        std::cout << "Keyboard key \"" << SDL_GetKeyName(current_event->key.keysym.sym) << "\" pressed.\n";
    }

    // Act on the physical location of the key on the board, if handling text
    // input in the future, use SDL_Keycode instead of SDL_Scancode
    switch(current_event->key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
            owner->RequestExit();
            break;
        case SDL_SCANCODE_LSHIFT:
            modifier_left_shift = true;
            break;
        case SDL_SCANCODE_RSHIFT:
            modifier_right_shift = true;
            break;
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_D:
            break;
        default:
            break;
    }
}

void InputManager::HandleKeyboardInputRelease() {

    if(debug_inputs) {
        std::cout << "Keyboard key \"" << SDL_GetKeyName(current_event->key.keysym.sym) << "\" released.\n";
    }

    // Act on the physical location of the key on the board, if handling text
    // input in the future, use SDL_Keycode instead of SDL_Scancode
    switch(current_event->key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
        case SDL_SCANCODE_LSHIFT:
            modifier_left_shift = false;
            break;
        case SDL_SCANCODE_RSHIFT:
            modifier_right_shift = false;
            break;
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_D:
            break;
        default:
            break;
    }
}

void InputManager::HandleMouseButtonPress() {

    // Touch events are handled seperately.
    if(current_event->button.which == SDL_TOUCH_MOUSEID) {
        return;
    }

    if(debug_inputs) {
        std::cout << "Mouse button \"" << current_event->button.button << "\" pressed.\n";
    }

    switch(current_event->button.button) {
        case SDL_BUTTON_LEFT:
        case SDL_BUTTON_MIDDLE:
        case SDL_BUTTON_RIGHT:
        case SDL_BUTTON_X1:
        case SDL_BUTTON_X2:
        default:
            break;
    }
}

void InputManager::HandleMouseButtonRelease() {

    // Touch events are handled seperately.
    if(current_event->button.which == SDL_TOUCH_MOUSEID) {
        return;
    }

    if(debug_inputs) {
        std::cout << "Mouse button \"" << current_event->button.button << "\" released.\n";
    }

    switch(current_event->button.button) {
        case SDL_BUTTON_LEFT:
        case SDL_BUTTON_MIDDLE:
        case SDL_BUTTON_RIGHT:
        case SDL_BUTTON_X1:
        case SDL_BUTTON_X2:
        default:
            break;
    }
}

void InputManager::HandleMouseWheel() {
    
    // Touch events are handled seperately.
    if(current_event->wheel.which == SDL_TOUCH_MOUSEID) {
        return;
    }
}

void InputManager::HandleMouseMovement() {

    // Touch events are handled seperately.
    if(current_event->motion.which == SDL_TOUCH_MOUSEID) {
        return;
    }
}

void InputManager::HandleControllerInputPress() {

    if(debug_inputs) {
        std::cout << "Controller button \"" << SDL_GameControllerGetStringForButton(SDL_GameControllerButton(current_event->cbutton.button)) << "\" pressed.\n";
    }

    switch(current_event->cbutton.button) {
        case SDL_CONTROLLER_BUTTON_INVALID:
        case SDL_CONTROLLER_BUTTON_A:
        case SDL_CONTROLLER_BUTTON_B:
        case SDL_CONTROLLER_BUTTON_X:
        case SDL_CONTROLLER_BUTTON_Y:
            break;
        case SDL_CONTROLLER_BUTTON_BACK:
            owner->RequestExit();
            break;
        case SDL_CONTROLLER_BUTTON_GUIDE:
        case SDL_CONTROLLER_BUTTON_START:
        case SDL_CONTROLLER_BUTTON_LEFTSTICK:
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        case SDL_CONTROLLER_BUTTON_MISC1:    /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button */
        case SDL_CONTROLLER_BUTTON_PADDLE1:  /* Xbox Elite paddle P1 */
        case SDL_CONTROLLER_BUTTON_PADDLE2:  /* Xbox Elite paddle P3 */
        case SDL_CONTROLLER_BUTTON_PADDLE3:  /* Xbox Elite paddle P2 */
        case SDL_CONTROLLER_BUTTON_PADDLE4:  /* Xbox Elite paddle P4 */
        case SDL_CONTROLLER_BUTTON_TOUCHPAD: /* PS4/PS5 touchpad button */
        case SDL_CONTROLLER_BUTTON_MAX:
        default:
            break;
    }
}

void InputManager::HandleControllerInputRelease() {

    if(debug_inputs) {
        std::cout << "Controller button \"" << SDL_GameControllerGetStringForButton(SDL_GameControllerButton(current_event->cbutton.button)) << "\" released.\n";
    }

    switch(current_event->cbutton.button) {
        case SDL_CONTROLLER_BUTTON_INVALID:
        case SDL_CONTROLLER_BUTTON_A:
        case SDL_CONTROLLER_BUTTON_B:
        case SDL_CONTROLLER_BUTTON_X:
        case SDL_CONTROLLER_BUTTON_Y:
        case SDL_CONTROLLER_BUTTON_BACK:
        case SDL_CONTROLLER_BUTTON_GUIDE:
        case SDL_CONTROLLER_BUTTON_START:
        case SDL_CONTROLLER_BUTTON_LEFTSTICK:
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        case SDL_CONTROLLER_BUTTON_MISC1:    /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button */
        case SDL_CONTROLLER_BUTTON_PADDLE1:  /* Xbox Elite paddle P1 */
        case SDL_CONTROLLER_BUTTON_PADDLE2:  /* Xbox Elite paddle P3 */
        case SDL_CONTROLLER_BUTTON_PADDLE3:  /* Xbox Elite paddle P2 */
        case SDL_CONTROLLER_BUTTON_PADDLE4:  /* Xbox Elite paddle P4 */
        case SDL_CONTROLLER_BUTTON_TOUCHPAD: /* PS4/PS5 touchpad button */
        case SDL_CONTROLLER_BUTTON_MAX:
        default:
            break;
    }
}

void InputManager::HandleControllerAxis() {

    if(debug_inputs) {
        if(current_event->caxis.value > controller_axis_deadzone || current_event->caxis.value < (0 - controller_axis_deadzone)) {
            std::cout << "Controller axis \"" << SDL_GameControllerGetStringForAxis(SDL_GameControllerAxis(current_event->caxis.axis)) << "\" is " << current_event->caxis.value << ".\n";
        }
    }

    switch(current_event->caxis.axis) {
        case SDL_CONTROLLER_AXIS_INVALID:
        case SDL_CONTROLLER_AXIS_LEFTX:
        case SDL_CONTROLLER_AXIS_LEFTY:
        case SDL_CONTROLLER_AXIS_RIGHTX:
        case SDL_CONTROLLER_AXIS_RIGHTY:
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
        case SDL_CONTROLLER_AXIS_MAX:
        default:
            break;
    }
}

void InputManager::HandleTouchPress() {

    if(debug_inputs) {
        std::cout << "Screen touched at (" << current_event->tfinger.x << ", " << current_event->tfinger.y << ")\n.";
    }
}

void InputManager::HandleTouchRelease() {

    if(debug_inputs) {
        std::cout << "Screen released at (" << current_event->tfinger.x << ", " << current_event->tfinger.y << ")\n.";
    }
}

void InputManager::HandleTouchMotion() {
        
    if(debug_inputs) {
        std::cout << "Screen touch movement.\n";
    }
}

void InputManager::HandleTouchMultiTouch() {

    if(debug_inputs) {
        std::cout << "Screen touch multi-touch.\n";
    }
}