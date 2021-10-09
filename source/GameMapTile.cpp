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

#include "GameMapTile.hpp"
#include "ResourceLoader.hpp"
#include "SpriteRenderer.hpp"

GameMapTile::GameMapTile() {
	tile_sprite = "grass";
	position_x = 0;
	position_y = 0;
	size = 16;
}

GameMapTile::GameMapTile(std::string tile_sprite, int position_x, int position_y) : tile_sprite(tile_sprite), position_x(position_x), position_y(position_y), size(16) {

}

GameMapTile::GameMapTile(std::string tile_sprite, int position_x, int position_y, int size) : tile_sprite(tile_sprite), position_x(position_x), position_y(position_y), size(size) {

}

void GameMapTile::Draw(SpriteRenderer* renderer) {
	renderer->DrawSprite(ResourceLoader::GetTexture(tile_sprite), glm::vec2(static_cast<float>(position_x), static_cast<float>(position_y)), glm::vec2(size, size));
}

void GameMapTile::DrawTileCoordinate(SpriteRenderer* renderer) {
	renderer->DrawSprite(ResourceLoader::GetTexture(tile_sprite), glm::vec2(static_cast<float>(position_x * size), static_cast<float>(position_y * size)), glm::vec2(size, size));
}