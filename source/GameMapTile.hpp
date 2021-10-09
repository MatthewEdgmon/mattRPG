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

#ifndef __GAME_MAP_TILE_HPP__
#define __GAME_MAP_TILE_HPP__

#include <string>

#include "SpriteRenderer.hpp"

class GameMapTile {

	public:
		GameMapTile();
		GameMapTile(std::string sprite_name, int position_x, int position_y);
		GameMapTile(std::string sprite_name, int position_x, int position_y, int size);

		void Draw(SpriteRenderer* renderer);
		void DrawTileCoordinate(SpriteRenderer* renderer);

		std::string GetSprite() { return tile_sprite; }

	private:
		std::string tile_sprite;

		int position_x, position_y, size;
};

#endif /* __GAME_MAP_TILE_HPP__ */