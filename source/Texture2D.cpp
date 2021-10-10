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

#include "Texture2D.hpp"

// TODO: Investigate GL_REPEAT for wrap_s
Texture2D::Texture2D() : texture_id(0), width(0), height(0),
						 subimage_size_x(0), subimage_size_y(0), subimage_count(0),
						 format_internal(GL_RGBA8), format_image(GL_RGBA), wrap_s(GL_CLAMP_TO_EDGE),
						 wrap_t(GL_CLAMP_TO_EDGE), filter_min(GL_LINEAR), filter_max(GL_LINEAR),
						 is_array_texture(false), is_loaded(false) {

}

void Texture2D::Generate(std::uint32_t width, std::uint32_t height, std::uint8_t* data) {

	if(is_loaded) {
		std::cout << "Tried to re-generate a texture." << std::endl;
		return;
	}

	this->width = width;
	this->height = height;

	// Create texture.
	glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
	glTextureStorage2D(texture_id, 1, format_internal, width, height);
	glTextureSubImage2D(texture_id, 0, 0, 0, width, height, format_image, GL_UNSIGNED_BYTE, data);

	// Set texture parameters.
	glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, wrap_s);
	glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, wrap_t);
	glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, filter_min);
	glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, filter_max);

	// Not an array texture.
	is_array_texture = false;

	// Texture is loaded.
	is_loaded = true;
}

void Texture2D::GenerateArray(std::uint32_t width, std::uint32_t height, std::uint32_t subimage_size_x, std::uint32_t subimage_size_y, std::uint8_t* data) {

	if(is_loaded) {
		std::cout << "Tried to re-generate a texture." << std::endl;
		return;
	}

	size_t tiles_x = width / subimage_size_x;
	size_t tiles_y = height / subimage_size_y;
	size_t tile_count = tiles_x * tiles_y;

	this->width = width;
	this->height = height;
	this->subimage_size_x = subimage_size_x;
	this->subimage_size_y = subimage_size_y;
	this->subimage_count = tile_count;

	// Create array texture.
	glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &texture_id);
	glTextureStorage3D(texture_id, 1, format_internal, subimage_size_x, subimage_size_y, tile_count);
	
	// Create temporary helper texture.
	GLuint temporary_texture = 0;
	glCreateTextures(GL_TEXTURE_2D, 1, &temporary_texture);
	glTextureStorage2D(temporary_texture, 1, format_internal, width, height);
	glTextureSubImage2D(temporary_texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// Use helper texture to fill main array texture.
	for(auto i = 0; i < tile_count; i++) {
		auto x = (i % tiles_x) * subimage_size_x;
		auto y = (i / tiles_x) * subimage_size_y;
		glCopyImageSubData(temporary_texture, GL_TEXTURE_2D, 0, x, y, 0, texture_id, GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, subimage_size_x, subimage_size_y, 1);
	}

	glDeleteTextures(1, &temporary_texture);

	// Is an array texture.
	is_array_texture = true;

	// Texture is loaded.
	is_loaded = true;
}

void Texture2D::Delete() {
	if(is_loaded) {
		glDeleteTextures(1, &texture_id);
	}
}

void Texture2D::Bind(std::uint32_t texture_unit) {
	if(is_loaded) {
		glBindTextureUnit(texture_unit, texture_id);
	} else {
		std::cout << "Tried to bind to un-generated texture." << std::endl;
	}
}