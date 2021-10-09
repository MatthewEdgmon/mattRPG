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

#ifndef __MUSIC_TRACK_HPP__
#define __MUSIC_TRACK_HPP__

#include <cstdint>

class MusicTrack {

	public:
		MusicTrack();

		void Generate(std::uint8_t* data);
		void Delete();

		void FadeIn(float fade_speed);
		void FadeOut(float fade_speed);

		void Play(float track_time = 0.0f);
		void Stop();

	private:
		float track_time;
};

#endif /* __MUSIC_TRACK_HPP__ */