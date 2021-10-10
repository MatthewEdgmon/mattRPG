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

#ifndef __TEXTURE_ARRAY_2D_HPP__
#define __TEXTURE_ARRAY_2D_HPP__

#include <cstdint>

// GLAD2
#include <glad/gl.h>

// SDL2
#include "SDL.h"

class TextureArray2D {

	public:
		TextureArray2D();

		// Generate from raw pixel data.
		void Generate(std::uint32_t width, std::uint32_t height, std::uint32_t subimage_size, std::uint8_t* data);
		void Delete();

		void Bind();

		void SetFilterMinMax(uint32_t min, uint32_t max) { filter_min = min; filter_max = max; }
		void SetInternalFormat(std::uint32_t format) { format_internal = format; }
		void SetImageFormat(std::uint32_t format) { format_image = format; }

		std::uint32_t GetID() { return texture_id; }
		std::uint32_t GetWidth() { return width; }
		std::uint32_t GetHeight() { return height; }

	private:
		// Actual reference to texture.
		std::uint32_t texture_id;

		// Texture dimensions.
		std::uint32_t width, height;

		// Format of internal image and image loaded from.
		std::uint32_t format_internal, format_image;

		std::uint32_t filter_min, filter_max;

		// Wrapping mode on S and T axes.
		std::uint32_t wrap_s, wrap_t;

		// Texture is loaded.
		bool texture_loaded;
};

#endif /* __TEXTURE_ARRAY_2D_HPP__ */