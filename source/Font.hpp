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

#ifndef __FONT_HPP__
#define __FONT_HPP__

// SDL2
#include "SDL_ttf.h"

#include "SpriteRenderer.hpp"

class Font {

	public:
		Font();
		Font(TTF_Font* font);

		void Font::Draw(SpriteRenderer* renderer, const char* text, int position_x, int position_y, uint8_t color_r = 0xFF, uint8_t color_g = 0xFF, uint8_t color_b = 0xFF);
		void Delete();

		TTF_Font* GetFont() { return font; }

	private:
		TTF_Font* font;
};

#endif /* __FONT_HPP__ */