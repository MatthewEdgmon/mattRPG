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

#ifndef __GAME_WORLD_HPP__
#define __GAME_WORLD_HPP__

/**
 * Ingame world is structed as follows (a higher level "contains" multiple of the lower level):
 * GameWorld
 * GameMap
 * GameMapLayer
 * GameMapTile
 * 
 * Everything is loaded in when a GameWorld is loaded by ResourceLoader.
 * 
 * A GameWorld represents an entire world, i.e. different locations each with their own GameMap.
 */

// STL
#include <cstdint>
#include <vector>

#include "GameMap.hpp"
#include "GameMapLayer.hpp"
#include "GameMapTile.hpp"

class GameWorld {

	public:
		GameWorld() : tile_size(16) { }
		GameWorld(int tile_size) : tile_size(tile_size) { }

		std::vector<GameMap>& GetMaps() { return maps; }
		int GetTileSize() { return tile_size; }

	private:
		std::vector<GameMap> maps;

		int tile_size;
};

#endif /* __GAME_WORLD_HPP__ */