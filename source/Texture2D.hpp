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

#ifndef __TEXTURE_2D_HPP__
#define __TEXTURE_2D_HPP__

// STL
#include <cstdint>

// GLAD2
#include <glad/gl.h>

// SDL2
#include "SDL.h"

class Texture2D {

	public:
		Texture2D();

		void Generate(std::uint32_t width, std::uint32_t height, std::uint8_t* data);
		void GenerateArray(std::uint32_t width, std::uint32_t height, std::uint32_t subimage_size_x, std::uint32_t subimage_size_y, std::uint8_t* data);

		void Delete();

		void Bind(std::uint32_t texture_unit = 0);

		void SetFilterMinMax(uint32_t min, uint32_t max) { filter_min = min; filter_max = max; }
		void SetInternalFormat(std::uint32_t format) { format_internal = format; }
		void SetImageFormat(std::uint32_t format) { format_image = format; }

		std::uint32_t GetID()            { return texture_id; }
		std::uint32_t GetWidth()         { return width; }
		std::uint32_t GetHeight()        { return height; }
		std::uint32_t GetSubImageSizeX() { return subimage_size_x; }
		std::uint32_t GetSubImageSizeY() { return subimage_size_y; }
		std::uint32_t GetSubImageCount() { return subimage_count; }
		bool          IsLoaded()         { return is_loaded; }
		bool          IsArrayTexture()   { return is_array_texture; }

	private:
		// Actual reference to texture.
		std::uint32_t texture_id;

		// Texture dimensions.
		std::uint32_t width, height;

		// If an array texture, dimensions of the subimages.
		std::uint32_t subimage_size_x, subimage_size_y, subimage_count;

		// Format of internal image and image loaded from.
		std::uint32_t format_internal, format_image;

		std::uint32_t filter_min, filter_max;

		// Wrapping mode on S and T axes.
		std::uint32_t wrap_s, wrap_t;

		// True if 2D texture array.
		bool is_array_texture;

		// Texture is loaded.
		bool is_loaded;
};

#endif /* __TEXTURE_2D_HPP__ */