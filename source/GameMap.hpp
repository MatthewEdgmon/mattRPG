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

#ifndef __GAME_MAP_HPP__
#define __GAME_MAP_HPP__

#include <map>
#include <tuple>
#include <vector>

#include "GameMapTile.hpp"
#include "SpriteRenderer.hpp"

class GameMap {

	public:
		GameMap(int tile_size, int max_x, int max_y);
		~GameMap();

		void Resize(int max_x, int max_y);

		void Draw(SpriteRenderer* renderer);

		void ChangeTile(int position_x, int position_y, GameMapTile new_tile);

		GameMapTile GetTileForMapCoordinate(int position_x, int position_y);
		GameMapTile GetTileForWindowCoordinate(int position_x, int position_y);

		int GetMaxX() { return max_x; }
		int GetMaxY() { return max_y; }

	private:
		std::vector<std::vector<GameMapTile>> tiles;

		int tile_size, max_x, max_y;
};

#endif /* __GAME_MAP_HPP__ */