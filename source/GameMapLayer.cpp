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

// STL
#include <string>
#include <vector>

#include "GameMapLayer.hpp"

GameMapLayer::GameMapLayer() : layer_type(GameMapLayerType::IntGrid), tileset_name(""), tile_size(16), width_tiles(0), height_tiles(0) {

}

GameMapLayer::GameMapLayer(GameMapLayerType layer_type, std::string tileset_name, int tile_size, size_t width_tiles, size_t height_tiles) : layer_type(layer_type), tileset_name(tileset_name), tile_size(tile_size), width_tiles(width_tiles), height_tiles(height_tiles) {

	// NOTE: We completely fill the 2D vector of tiles with zero tiles in the case of some layers having incomplete/empty tile locations.
	tiles.resize(height_tiles);

	for(auto& column : tiles) {
		column.resize(width_tiles, GameMapTile(0));
	}
}

GameMapLayer::GameMapLayer(std::string layer_type, std::string tileset_name, int tile_size, size_t width_tiles, size_t height_tiles) : tileset_name(tileset_name), tile_size(tile_size), width_tiles(width_tiles), height_tiles(height_tiles) {

	// Infer GameMapLayerType from layer_type.
	if(layer_type == "IntGrid") {
		this->layer_type = GameMapLayerType::IntGrid;
	} else if(layer_type == "Entities") {
		this->layer_type = GameMapLayerType::Entities;
	} else if(layer_type == "Tiles") {
		this->layer_type = GameMapLayerType::Tiles;
	} else if(layer_type == "AutoLayer") {
		this->layer_type = GameMapLayerType::AutoLayer;
	} else {
		this->layer_type = GameMapLayerType::IntGrid;
	}

	// NOTE: We completely fill the 2D vector of tiles with zero tiles in the case of some layers having incomplete/empty tile locations.
	tiles.resize(height_tiles);

	for(auto& column : tiles) {
		column.resize(width_tiles, GameMapTile(0));
	}
}