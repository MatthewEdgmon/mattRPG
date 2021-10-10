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

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <stdexcept>

// GLAD2
#include <glad/gl.h>

// SDL2
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Font.hpp"
#include "MusicTrack.hpp"
#include "ResourceLoader.hpp"
#include "Shader.hpp"
#include "SoundEffect.hpp"
#include "Texture2D.hpp"

std::map<std::string, Font>        ResourceLoader::fonts;
std::map<std::string, MusicTrack>  ResourceLoader::music_tracks;
std::map<std::string, Shader>      ResourceLoader::shaders;
std::map<std::string, SoundEffect> ResourceLoader::sound_effects;
std::map<std::string, Texture2D>   ResourceLoader::textures;

Font ResourceLoader::LoadFont(const char* filename, int point_size, std::string font_name) {
	fonts[font_name] = LoadFontFromFile(filename, point_size);
	return fonts[font_name];
}

Font ResourceLoader::GetFont(std::string font_name) {
	return fonts[font_name];
}

MusicTrack ResourceLoader::LoadMusicTrack(const char* filename, std::string music_track_name) {
	music_tracks[music_track_name] = LoadMusicTrackFromFile(filename);
	return music_tracks[music_track_name];
}

MusicTrack ResourceLoader::GetMusicTrack(std::string music_track_name) {
	return music_tracks[music_track_name];
}

Shader ResourceLoader::LoadShader(const char* vertex_shader_filename, const char* fragment_shader_filename, const char* geometry_shader_filename, std::string shader_name) {
	shaders[shader_name] = LoadShaderFromFile(vertex_shader_filename, fragment_shader_filename, geometry_shader_filename);
	return shaders[shader_name];
}

Shader ResourceLoader::GetShader(std::string name) {
	return shaders[name];
}

SoundEffect ResourceLoader::LoadSoundEffect(const char* filename, std::string sound_effect_name) {
	sound_effects[sound_effect_name] = LoadSoundEffectFromFile(filename);
	return sound_effects[sound_effect_name];
}

SoundEffect ResourceLoader::GetSoundEffect(std::string name) {
	return sound_effects[name];
}

Texture2D ResourceLoader::LoadTextureAtlas(const char* filename, bool alpha, bool bilinear, std::string texture_name, glm::vec2 top_left, glm::vec2 bottom_right) {
	textures[texture_name] = LoadTextureAtlasFromFile(filename, alpha, bilinear, top_left, bottom_right);
	return textures[texture_name];
}

Texture2D ResourceLoader::LoadTexture(const char* filename, bool alpha, bool bilinear, std::string texture_name) {
	textures[texture_name] = LoadTextureFromFile(filename, alpha, bilinear);
	return textures[texture_name];
}

Texture2D ResourceLoader::GetTexture(std::string name) {
	return textures[name];
}

void ResourceLoader::UnloadAll() {

	for(auto font : fonts) {
		font.second.Delete();
	}

	for(auto music_track : music_tracks) {
		music_track.second.Delete();
	}

	for(auto shader : shaders) {
		shader.second.Delete();
	}

	for(auto sfx : sound_effects) {
		sfx.second.Delete();
	}

	for(auto texture : textures) {
		texture.second.Delete();
	}
}

Font ResourceLoader::LoadFontFromFile(const char* filename, int point_size) {

	TTF_Font* font = TTF_OpenFont(filename, point_size);

	if(font == nullptr) {
		std::cout << "ResourceLoader: Failed to load font from file \"" << filename << "\". TTF_GetError(): " << TTF_GetError() << "\n";
		return Font();
	}

	Font new_font(font);

	return new_font;
}

MusicTrack ResourceLoader::LoadMusicTrackFromFile(const char* filename) {

	MusicTrack music_track;

	return music_track;
}

Shader ResourceLoader::LoadShaderFromFile(const char* vertex_shader_filename, const char* fragment_shader_filename, const char* geometry_shader_filename) {
	
	std::string vertex_shader_source, fragment_shader_source, geometry_shader_source;
	std::stringstream vertex_shader_stream, fragment_shader_stream, geometry_shader_stream;

	try {
		std::ifstream vertex_shader_file(vertex_shader_filename);
		std::ifstream fragment_shader_file(fragment_shader_filename);
		
		vertex_shader_stream << vertex_shader_file.rdbuf();
		fragment_shader_stream << fragment_shader_file.rdbuf();

		vertex_shader_file.close();
		fragment_shader_file.close();

		vertex_shader_source = vertex_shader_stream.str();
		fragment_shader_source = fragment_shader_stream.str();

		// Load geoemetry shader if present.
		if(geometry_shader_filename != nullptr) {
			std::ifstream geometry_shader_file(geometry_shader_filename);
			geometry_shader_stream << geometry_shader_file.rdbuf();
			geometry_shader_file.close();
			geometry_shader_source = geometry_shader_stream.str();
		}

	} catch(std::exception e) {
		std::cout << "ResourceLoader: Faild to load shader from file.\n";
	}

	Shader shader;

	if(geometry_shader_filename != nullptr) {
		shader.Compile(vertex_shader_source.c_str(), fragment_shader_source.c_str(), geometry_shader_source.c_str());
	} else {
		shader.Compile(vertex_shader_source.c_str(), fragment_shader_source.c_str(), nullptr);
	}

	return shader;
}

SoundEffect ResourceLoader::LoadSoundEffectFromFile(const char* filename) {
	SoundEffect sound_effect;
	return sound_effect;
}

Texture2D ResourceLoader::LoadTextureAtlasFromFile(const char* filename, bool alpha, bool bilinear, glm::vec2 top_left, glm::vec2 bottom_right) {
	
	SDL_Rect atlas_rect;
	Texture2D result_texture;

	if(filename == nullptr) {
		return result_texture;
	}

	if(alpha) {
		result_texture.SetInternalFormat(GL_RGBA8);
		result_texture.SetImageFormat(GL_RGBA);
	}

	if(bilinear) {
		result_texture.SetFilterMinMax(GL_LINEAR, GL_LINEAR);
	}

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    std::uint32_t rmask = 0xFF000000;
	std::uint32_t gmask = 0x00FF0000;
	std::uint32_t bmask = 0x0000FF00;
	std::uint32_t amask = 0x000000FF;
#else
	std::uint32_t rmask = 0x000000FF;
	std::uint32_t gmask = 0x0000FF00;
	std::uint32_t bmask = 0x00FF0000;
	std::uint32_t amask = 0xFF000000;
#endif

	atlas_rect.x = top_left.x;
	atlas_rect.y = top_left.y;
	atlas_rect.w = (bottom_right.x - top_left.x);
	atlas_rect.h = (bottom_right.y - top_left.y);

	SDL_Surface* atlas_surface = IMG_Load(filename);
	SDL_Surface* result_surface = SDL_CreateRGBSurface(0, (bottom_right.x - top_left.x), (bottom_right.y - top_left.y), 32, rmask, gmask, bmask, amask);

	if(atlas_surface == NULL) {
		std::cout << "ResourceLoader: Failed to load texture atlas from file \"" << filename << "\". IMG_GetError(): " << IMG_GetError() << "\n";
		return result_texture;
	}

	if(result_surface == NULL) {
		std::cout << "ResourceLoader: Failed to create blank SDL RGBA surface. SDL_GetError(): " << SDL_GetError() << "\n";
		return result_texture;
	}

	if(SDL_BlitSurface(atlas_surface, &atlas_rect, result_surface, NULL) != 0) {
		std::cout << "ResourceLoader: Failed to SDL_BlitSurface texture atlas. SDL_GetError(): " << SDL_GetError() << "\n";
		return result_texture;
	}

	result_texture.Generate(result_surface->w, result_surface->h, static_cast<std::uint8_t*>(result_surface->pixels));

	return result_texture;
}

Texture2D ResourceLoader::LoadTextureFromFile(const char* filename, bool alpha, bool bilinear) {

	Texture2D texture;

	if(filename == nullptr) {
		return texture;
	}

	if(alpha) {
		texture.SetInternalFormat(GL_RGBA8);
		texture.SetImageFormat(GL_RGBA);
	}

	if(bilinear) {
		texture.SetFilterMinMax(GL_LINEAR, GL_LINEAR);
	}

	SDL_Surface* image_surface = IMG_Load(filename);

	if(image_surface == NULL) {
		std::cout << "ResourceLoader: Failed to load texture from file \"" << filename << "\". IMG_GetError(): " << IMG_GetError() << "\n";
		return texture;
	}

	texture.Generate(image_surface->w, image_surface->h, static_cast<std::uint8_t*>(image_surface->pixels));

	return texture;
}