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

#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <cstdint>

// GLAD2
#include <glad/gl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Shader {

	public:
		Shader() : program_id(0) { };

		Shader& Use();

		void Compile(const char* vertex_source, const char* fragment_source, const char* geometry_source);
		void Delete();

		// Raw data types.
		void SetInteger (const char* name, int value, bool use_shader = false);
		void SetFloat   (const char* name, float value, bool use_shader = false);
		void SetVector2f(const char* name, float x, float y, bool use_shader = false);
		void SetVector3f(const char* name, float x, float y, float z, bool use_shader = false);
		void SetVector4f(const char* name, float x, float y, float z, float w, bool use_shader = false);
		
		// GLM data types.
		void SetVector2f(const char* name, const glm::vec2& value, bool use_shader = false);
		void SetVector3f(const char* name, const glm::vec3& value, bool use_shader = false);
		void SetVector4f(const char* name, const glm::vec4& value, bool use_shader = false);
		void SetMatrix4f(const char* name, const glm::mat4& value, bool use_shader = false);

		std::uint64_t GetID() { return program_id; }

	private:
		std::uint64_t program_id;

		bool is_ready = false;
};

#endif /* __SHADER_HPP__ */