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

#ifndef __RESOURCE_LOADER_HPP__
#define __RESOURCE_LOADER_HPP__

// STL
#include <cstdint>
#include <map>
#include <string>

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Font.hpp"
#include "GameWorld.hpp"
#include "MusicTrack.hpp"
#include "Texture2D.hpp"
#include "Shader.hpp"
#include "SoundEffect.hpp"

class ResourceLoader {

	public:
		static Font LoadFont(const char* filename, int point_size, std::string font_name);
		static Font GetFont(std::string name);

		static GameWorld LoadGameWorld(const char* filename, std::string game_world_name);
		static GameWorld GetGameWorld(std::string name);

		static MusicTrack LoadMusicTrack(const char* filename, std::string music_track_name);
		static MusicTrack GetMusicTrack(std::string name);

		static Shader LoadShader(const char* vertex_shader_filename, const char* fragment_shader_filename, const char* geometry_shader_filename, std::string shader_name);
		static Shader GetShader(std::string name);

		static SoundEffect LoadSoundEffect(const char* filename, std::string sound_effect_name);
		static SoundEffect GetSoundEffect(std::string name);

		static Texture2D LoadSubTexture(const char* filename, bool alpha, bool bilinear, std::string texture_name, glm::vec2 top_left, glm::vec2 bottom_right);
		static Texture2D LoadTextureArray(const char* filename, bool alpha, bool bilinear, std::string texture_name, std::uint32_t subimage_size_x, std::uint32_t subimage_size_y);
		static Texture2D LoadTexture(const char* filename, bool alpha, bool bilinear, std::string texture_name);
		static Texture2D GetTexture(std::string name);

		static void UnloadAll();

	private:
		ResourceLoader() { }

		static Font LoadFontFromFile(const char* filename, int point_size);
		static GameWorld LoadGameWorldFromFile(const char* filename);
		static MusicTrack LoadMusicTrackFromFile(const char* filename);
		static Shader LoadShaderFromFile(const char* vertex_shader_filename, const char* fragment_shader_filename, const char* geometry_shader_filename);
		static SoundEffect LoadSoundEffectFromFile(const char* filename);
		static Texture2D LoadSubTextureFromFile(const char* filename, bool alpha, bool bilinear, glm::vec2 top_left, glm::vec2 bottom_right);
		static Texture2D LoadTextureArrayFromFile(const char* filename, bool alpha, bool bilinear, std::uint32_t subimage_size_x, std::uint32_t subimage_size_y);
		static Texture2D LoadTextureFromFile(const char* filename, bool alpha, bool bilinear);

		static std::map<std::string, Font>        fonts;
		static std::map<std::string, GameWorld>   game_worlds;
		static std::map<std::string, MusicTrack>  music_tracks;
		static std::map<std::string, Shader>      shaders;
		static std::map<std::string, SoundEffect> sound_effects;
		static std::map<std::string, Texture2D>   textures;
};

#endif /* __RESOURCE_LOADER_HPP__ */