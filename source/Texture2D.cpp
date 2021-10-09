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

#include <cstdint>
#include <iostream>

// GLAD2
#include <glad/gl.h>

// SDL2
#include "SDL.h"

#include "Texture2D.hpp"

// TODO: Investigate GL_REPEAT for wrap_s
Texture2D::Texture2D() : texture_loaded(false), width(0), height(0), format_internal(GL_RGB), format_image(GL_RGB), wrap_s(GL_CLAMP_TO_EDGE), wrap_t(GL_CLAMP_TO_EDGE), filter_min(GL_LINEAR), filter_max(GL_LINEAR) {
	glGenTextures(1, &texture_id);
}

void Texture2D::Generate(std::uint32_t width, std::uint32_t height, std::uint8_t* data) {

	this->width = width;
	this->height = height;

	// Create texture.
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, format_internal, width, height, 0, format_image, GL_UNSIGNED_BYTE, data);

	// Set texture parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

	// Unbind.
	glBindTexture(GL_TEXTURE_2D, 0);

	// Texture is loaded.
	texture_loaded = true;
}

void Texture2D::Delete() {
	if(texture_loaded) {
		glDeleteTextures(1, &texture_id);
	}
}

void Texture2D::Bind() {
	if(texture_loaded) {
		glBindTexture(GL_TEXTURE_2D, texture_id);
	} else {
		std::cout << "Tried to bind to un-generated texture.\n";
	}
}