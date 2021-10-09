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

#ifndef __GAME_WINDOW_HPP__
#define __GAME_WINDOW_HPP__

#include <cstdint>
#include <memory>

#include "SDL.h"

#include "InputManager.hpp"

class GameApplication {

	public:
		int Main(int argc, char** argv);

		void Initialize();
		void Loop();
		void Shutdown();

		void RequestExit() { is_running = false; }

	private:
		SDL_Window*         sdl_window;
		SDL_GLContext       sdl_gl_context;
		SDL_AudioDeviceID   sdl_audio_device_id;
		SDL_Event           sdl_event;
		SDL_GameController* sdl_game_controller;

		std::unique_ptr<InputManager> input_manager;

		int window_width = 640;
		int window_height = 480;

		std::uint64_t frame_start;
		std::uint64_t frame_end;
		std::uint64_t perf_freq;
		double frame_time;
		double frame_rate;

		int stored_argc;
		char** stored_argv;

		bool is_running = false;
};

#endif /* __GAME_WINDOW_HPP__ */