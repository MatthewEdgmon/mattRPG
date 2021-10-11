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

#include "GameMapLayer.hpp"

GameMapLayer::GameMapLayer() : tile_size(16), width_tiles(0), height_tiles(0) {

}

GameMapLayer::GameMapLayer(int tile_size, size_t width_tiles, size_t height_tiles) : tile_size(tile_size), width_tiles(width_tiles), height_tiles(height_tiles) {

	// NOTE: We completely fill the 2D vector of tiles with zero tiles in the case of some layers having incomplete/empty tile locations.
	tiles.resize(width_tiles);

	for(auto column : tiles) {
		column.resize(height_tiles, GameMapTile(18));
	}
}

void GameMapLayer::ChangeDimensions(size_t new_width_tiles, size_t new_height_tiles) {
	width_tiles = new_width_tiles;
	height_tiles = new_height_tiles;

	tiles.resize(width_tiles);

	for(auto column : tiles) {
		column.resize(height_tiles, GameMapTile(0));
	}
}