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

#ifndef __GAME_MAP_LAYER_HPP__
#define __GAME_MAP_LAYER_HPP__

// STL
#include <string>
#include <vector>

#include "GameMapTile.hpp"

enum class GameMapLayerType {
	IntGrid,
	Entities,
	Tiles,
	AutoLayer
};

class GameMapLayer {

	public:
		GameMapLayer();
		GameMapLayer(GameMapLayerType layer_type, std::string tileset_name, int tile_size, size_t width_tiles, size_t height_tiles);
		GameMapLayer(std::string layer_type, std::string tileset_name, int tile_size, size_t width_tiles, size_t height_tiles);

		std::vector<std::vector<GameMapTile>>& GetTiles() { return tiles; }

		GameMapLayerType& GetLayerType() { return layer_type; }

		int GetWidthTiles() { return width_tiles; }
		int GetHeightTiles() { return height_tiles; }

		std::string& GetTileSetName() { return tileset_name; }

		size_t GetTileCount() { return width_tiles * height_tiles; }

	private:
		GameMapLayerType layer_type;

		std::vector<std::vector<GameMapTile>> tiles;

		std::string tileset_name;

		size_t width_tiles, height_tiles;

		int tile_size;
};

#endif /* __GAME_MAP_LAYER_HPP__ */