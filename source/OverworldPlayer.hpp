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

#ifndef __OVERWORLD_PLAYER_HPP__
#define __OVERWORLD_PLAYER_HPP__

#include <string>
#include <vector>

enum class OverworldPlayerState {
	IDLE,
	WALK,
	ATTACK,
	HIT,
	DEATH
};

class OverworldPlayer {

	public:
		OverworldPlayer(std::vector<std::string> animation);
		~OverworldPlayer();

		void ChangeState(OverworldPlayerState state);

		std::string GetAnimationFrame();

	private:
		std::vector<std::string> animation_frames;

		int current_animation_frame = 0;
};

#endif /* __OVERWORLD_PLAYER_HPP__ */