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

#ifndef __SOUND_EFFECT_HPP__
#define __SOUND_EFFECT_HPP__

#include <cstdint>

#include "SDL.h"
#include "SDL_mixer.h"

class SoundEffect {

	public:
		SoundEffect();

		void Generate(std::uint8_t* data);
		void Delete();

		void Play();
		void PlayRepeat(int count);
		void Stop();

	private:


};

#endif /* __SOUND_EFFECT_HPP__ */