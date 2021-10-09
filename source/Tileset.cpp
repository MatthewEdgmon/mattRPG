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

#include <iostream>
#include <string>

#include "ResourceLoader.hpp"
#include "Tileset.hpp"

Tileset::Tileset(std::string name, std::string file_name, int tileset_width_pixels, int tileset_height_pixels) : is_loaded(false), tile_size(16), tile_count(0),
																												 tileset_width_pixels(tileset_width_pixels), tileset_height_pixels(tileset_height_pixels),
																												 name(name), file_name(file_name) {
	std::string tilename = name;
	
	tileset_width = tileset_width_pixels / tile_size;
	tileset_height = tileset_height_pixels / tile_size;

	for(size_t i = 0; i < (tileset_width * tileset_height); i++) {
		tilename = name + std::to_string(i);
		tile_count++;
		tileset_names.push_back(tilename);
	}

	std::cout << "Loaded " << tile_count << " tiles from \"" << file_name << "\" to tileset " << name << "." << std::endl;

}

std::string Tileset::GetTileNameForPosition(int position_x, int position_y) {

	if(is_loaded == false) {
		return "";
	}

	return tileset_names[(position_y * tileset_width) + position_x];
}