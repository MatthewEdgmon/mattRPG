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

#ifndef __INPUT_MANAGER_HPP__
#define __INPUT_MANAGER_HPP__

// Handles gathering raw input, translating it to a "GameAction" (name??), and binding between them

#include "SDL.h"

class GameApplication;

// TODO: Maybe this should be called InputMapper?
class InputManager {

	public:
		InputManager(GameApplication* game_application) : owner(game_application), current_event(nullptr),
														  modifier_left_ctrl(false), modifier_right_ctrl(false), 
														  modifier_left_shift(false), modifier_right_shift(false),
														  modifier_left_alt(false), modifier_right_alt(false),
														  modifier_caps_lock(false) { ; }

		void HandleInput(SDL_Event* input_event);

		void AddBinding();
		void RemoveBinding();

		void SetControllerAxisDeadzone(int deadzone_value) { controller_axis_deadzone = deadzone_value; }

		// TODO: Show not only what input was activated, but also which GameAction(s) are bound.
		void ToggleInputDebugging() { debug_inputs = !debug_inputs; }

	private:
		void HandleKeyboardInputPress();
		void HandleKeyboardInputRelease();
		void HandleMouseButtonPress();
		void HandleMouseButtonRelease();
		void HandleMouseWheel();
		void HandleMouseMovement();
		void HandleControllerInputPress();
		void HandleControllerInputRelease();
		void HandleControllerAxis();
		void HandleTouchPress();
		void HandleTouchRelease();
		void HandleTouchMotion();
		void HandleTouchMultiTouch();

		GameApplication* owner;

		SDL_Event* current_event;

		int controller_axis_deadzone = 2048;

		bool modifier_left_ctrl;
		bool modifier_right_ctrl;
		bool modifier_left_shift;
		bool modifier_right_shift;
		bool modifier_left_alt;
		bool modifier_right_alt;
		bool modifier_caps_lock;

		bool debug_inputs = true;
};

#endif /* __INPUT_MANAGER_HPP__ */