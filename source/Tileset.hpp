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

#ifndef __TILESET_HPP__
#define __TILESET_HPP__

#include <string>
#include <vector>

class Tileset {

	public:
		Tileset(std::string name, std::string file_name, int tileset_width_pixels, int tileset_height_pixels);

		std::string GetTileNameForPosition(int tileset_x, int tileset_y);

		int GetWidthPixels() { return tileset_width_pixels; }
		int GetHeightPixels() { return tileset_height_pixels; }

		int GetWidthTiles() { return tileset_width; }
		int GetHeightTiles() { return tileset_height; }

	private:
		bool is_loaded;
		int tile_size;
		int tileset_width_pixels, tileset_height_pixels;
		int tileset_width, tileset_height;
		int tile_count;
		std::string name;
		std::string file_name;
		std::vector<std::string> tileset_names;
};

#endif /* __TILESET_HPP__ */