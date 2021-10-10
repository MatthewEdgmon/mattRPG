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
#include <cstdint>
#include <iostream>

// GLAD2
#include <glad/gl.h>

// SDL2
#include "SDL.h"

#include "TextureArray2D.hpp"

TextureArray2D::TextureArray2D() : texture_id(0), width(0), height(0),
								   format_internal(GL_RGBA8), format_image(GL_RGBA), wrap_s(GL_CLAMP_TO_EDGE),
								   wrap_t(GL_CLAMP_TO_EDGE), filter_min(GL_LINEAR), filter_max(GL_LINEAR),
								   texture_loaded(false) {

}

void TextureArray2D::Generate(std::uint32_t width, std::uint32_t height, std::uint32_t subimage_size, std::uint8_t* data) {

	this->width = width;
	this->height = height;

	size_t tiles_x = width / subimage_size;
	size_t tiles_y = height / subimage_size;
	size_t tile_count = tiles_x * tiles_y;


	// Texture is loaded.
	texture_loaded = true;
}

void TextureArray2D::Delete() {
	if(texture_loaded) {
		glDeleteTextures(1, &texture_id);
	}
}

void TextureArray2D::Bind() {
	if (texture_loaded) {
		glBindTextureUnit(0, texture_id);
	}
	else {
		std::cout << "Tried to bind to un-generated texture.\n";
	}
}