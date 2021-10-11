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

void GameMap::ChangeDimensions(size_t new_width_tiles, size_t new_height_tiles) {
	width_tiles = new_width_tiles;
	height_tiles = new_height_tiles;

	for(auto layer : layers) {
		layer.ChangeDimensions(new_width_tiles, new_height_tiles);
	}
}