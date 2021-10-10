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
#include <iostream>

// SDL2
#include "SDL_ttf.h"

#include "Font.hpp"
#include "SpriteRenderer.hpp"
#include "Texture2D.hpp"

Font::Font() : font(nullptr) {

}

Font::Font(TTF_Font* font) : font(font) {

}

void Font::Draw(SpriteRenderer* renderer, const char* text, int position_x, int position_y, uint8_t color_r, uint8_t color_g, uint8_t color_b) {

	Texture2D texture;

	SDL_Color font_color = { color_b, color_g, color_r };

	SDL_Surface* font_surface = TTF_RenderUTF8_Blended(font, text, font_color);

	if(font_surface == NULL) {
		std::cout << "Font: Failed to generate font surface. TTF_GetError(): " << TTF_GetError() << "\n";
		return;
	}

	uint8_t font_bbp = font_surface->format->BytesPerPixel;

	// TODO: Investigate whether anything other than GL_RGBA/GL_RGBA8 is possible.
	// Colors swapped above in font_color to accomadate for now.
	if(font_bbp == 4) {
		if(font_surface->format->Rmask == 0x000000FF || font_surface->format->Rmask == 0x00FF0000) {
			texture.SetImageFormat(GL_RGBA);
			texture.SetInternalFormat(GL_RGBA8);
		} else {
			texture.SetImageFormat(GL_BGRA);
			texture.SetInternalFormat(GL_BGRA8_EXT);
		}
	} else {
		if(font_surface->format->Rmask == 0x000000FF) {
			texture.SetImageFormat(GL_RGB);
			texture.SetInternalFormat(GL_RGB8);
		} else {
			texture.SetImageFormat(GL_BGR);
			texture.SetInternalFormat(GL_BGRA8_EXT);
		}
	}

	texture.Generate(font_surface->w, font_surface->h, static_cast<std::uint8_t*>(font_surface->pixels));

	SDL_FreeSurface(font_surface);

	renderer->DrawSprite(texture, glm::vec2(position_x, position_y), glm::vec2(texture.GetWidth(), texture.GetHeight()));

	texture.Delete();
}

void Font::Delete() {

	TTF_CloseFont(font);
}