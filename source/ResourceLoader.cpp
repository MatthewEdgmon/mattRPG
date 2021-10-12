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

// json
#include <nlohmann/json.hpp>

#include "Font.hpp"
#include "GameMap.hpp"
#include "GameMapLayer.hpp"
#include "GameMapTile.hpp"
#include "GameWorld.hpp"
#include "MusicTrack.hpp"
#include "ResourceLoader.hpp"
#include "Shader.hpp"
#include "SoundEffect.hpp"
#include "Texture2D.hpp"

std::map<std::string, Font>        ResourceLoader::fonts;
std::map<std::string, GameWorld>   ResourceLoader::game_worlds;
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

GameWorld& ResourceLoader::LoadGameWorld(const char* filename, std::string game_world_name) {
	game_worlds[game_world_name] = LoadGameWorldFromFile(filename);
	return game_worlds[game_world_name];
}

GameWorld& ResourceLoader::GetGameWorld(std::string game_world_name) {
	return game_worlds[game_world_name];
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

Texture2D ResourceLoader::LoadSubTexture(const char* filename, bool alpha, bool bilinear, std::string texture_name, glm::vec2 top_left, glm::vec2 bottom_right) {
	textures[texture_name] = LoadSubTextureFromFile(filename, alpha, bilinear, top_left, bottom_right);
	return textures[texture_name];
}

Texture2D ResourceLoader::LoadTextureArray(const char* filename, bool alpha, bool bilinear, std::string texture_name, std::uint32_t subimage_size_x, std::uint32_t subimage_size_y) {
	textures[texture_name] = LoadTextureArrayFromFile(filename, alpha, bilinear, subimage_size_x, subimage_size_y);
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

GameWorld ResourceLoader::LoadGameWorldFromFile(const char* filename) {

	std::ifstream input_file(filename);

	if(input_file.fail()) {
		std::cout << "Failed to load a GameWorld from \"" << filename << "\" (file doesn't exist or can't open)." << std::endl;
		return GameWorld(0);
	}

	nlohmann::json input_json;

	input_file >> input_json;
	input_file.close();

	if(input_json.empty()) {
		std::cout << "Failed to load a GameWorld from \"" << filename << "\" (file exists, JSON was empty)." << std::endl;
		return GameWorld(0);
	}

	int tile_size = static_cast<int>(input_json.find<std::string>("defaultGridSize").value());

	GameWorld game_world(tile_size);

	if(input_json.contains("levels")) {

		for(auto level : input_json.find<std::string>("levels").value()) {

			std::string level_identifier = level.find<std::string>("identifier").value();

			int level_width = static_cast<int>(level.find<std::string>("pxWid").value()) / tile_size;
			int level_height = static_cast<int>(level.find<std::string>("pxHei").value()) / tile_size;

			// Add a GameMap for each level.
			game_world.GetMaps().push_back(GameMap(tile_size, level_width, level_height));

			for(auto layer : level.find<std::string>("layerInstances").value()) {

				// Add a GameMapLayer for each layer.
				std::string identifier = layer.find<std::string>("__identifier").value();
				std::string type       = layer.find<std::string>("__type").value();

				game_world.GetMaps().back().GetLayers().push_back(GameMapLayer(type, identifier, tile_size, level_width, level_height));

				// Only load the tiles if GameMapLayerType is Tiles
				if(type == "Tiles") {

					for(auto tile : layer.find<std::string>("gridTiles").value()) {
					
						int tile_x = tile["px"].get<std::vector<int>>()[0] / tile_size;
						int tile_y = tile["px"].get<std::vector<int>>()[1] / tile_size;

						// Add a GameMapTile for each tile.
						//std::cout << "Tile (" << tile_x << "," << tile_y << ") for layer " << layer.find<std::string>("__identifier").value() << " for map " << level_identifier << "." << std::endl;
						game_world.GetMaps().back().GetLayers().back().GetTiles().at(tile_y).at(tile_x) = GameMapTile(static_cast<int>(tile.find<std::string>("t").value()));
					}
				}
			}
		}
	}

	return game_world;
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

Texture2D ResourceLoader::LoadSubTextureFromFile(const char* filename, bool alpha, bool bilinear, glm::vec2 top_left, glm::vec2 bottom_right) {
	
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

Texture2D ResourceLoader::LoadTextureArrayFromFile(const char* filename, bool alpha, bool bilinear, std::uint32_t subimage_size_x, std::uint32_t subimage_size_y) {

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

	texture.GenerateArray(image_surface->w, image_surface->h, subimage_size_x, subimage_size_y, static_cast<std::uint8_t*>(image_surface->pixels));

	return texture;
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