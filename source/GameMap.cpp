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

#include "GameMap.hpp"
#include "GameMapTile.hpp"

GameMap::GameMap(int tile_size, int max_x, int max_y) : tile_size(tile_size), max_x(max_x), max_y(max_y) {
	Resize(max_x, max_y);
}

GameMap::~GameMap() {

}

void GameMap::Resize(int max_x, int max_y) {
	if(this) {
		this->max_x = max_x;
		this->max_y = max_y;

		tiles.resize(max_y);

		for(size_t i = 0; i < tiles.size(); i++) {
			tiles[i].resize(max_x);
		}
	}
}

void GameMap::Draw(SpriteRenderer* renderer) {
	for(size_t x = 0; x < max_x; x++) {
		for(size_t y = 0; y < max_y; y++) {
			tiles.at(x).at(y).DrawTileCoordinate(renderer);
		}
	}
}

void GameMap::ChangeTile(int position_x, int position_y, GameMapTile new_tile) {
	tiles.at(position_x).at(position_y) = new_tile;
}

GameMapTile GameMap::GetTileForMapCoordinate(int position_x, int position_y) {
	return tiles.at(position_x).at(position_y);
}

GameMapTile GameMap::GetTileForWindowCoordinate(int position_x, int position_y) {
	return tiles.at(position_x / tile_size).at(position_y / tile_size);
}