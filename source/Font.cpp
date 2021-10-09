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

	SDL_Color font_color = { color_r, color_g, color_b };

	SDL_Surface* font_surface = TTF_RenderUTF8_Blended(font, text, font_color);

	if(font_surface == NULL) {
		std::cout << "Font: Failed to generate font surface. TTF_GetError(): " << TTF_GetError() << "\n";
		return;
	}

	uint8_t font_bbp = font_surface->format->BytesPerPixel;

	if(font_bbp == 4) {
		if(font_surface->format->Rmask == 0x000000FF) {
			texture.SetImageFormat(GL_RGBA);
		} else {
			texture.SetImageFormat(GL_BGRA);
		}
	} else {
		if(font_surface->format->Rmask == 0x000000FF) {
			texture.SetImageFormat(GL_RGB);
		} else {
			texture.SetImageFormat(GL_BGR);
		}
	}

	texture.SetInternalFormat(font_bbp);

	texture.Generate(font_surface->w, font_surface->h, static_cast<std::uint8_t*>(font_surface->pixels));

	SDL_FreeSurface(font_surface);

	renderer->DrawSprite(texture, glm::vec2(position_x, position_y), glm::vec2(texture.GetWidth(), texture.GetHeight()));
}

void Font::Delete() {

	TTF_CloseFont(font);
}