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

#include <iostream>

// GLAD2
#include <glad/gl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Shader.hpp"

Shader& Shader::Use() {
	glUseProgram(program_id);
	return *this;
}

void Shader::Compile(const char* vertex_source, const char* fragment_source, const char* geometry_source) {

	int result = 0;
	char result_log[1024];
	unsigned int vertex_id, fragment_id, geometry_id;

	// Geometry shader is optional, however fragment and vertex are not.
	if(vertex_source == nullptr || fragment_source == nullptr) {
		std::cout << "Shader Error: Vertex or fragment source is nullptr.\n";
		is_ready = false;
		return;
	}

	// Compile and check vertex shader.
	vertex_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_id, 1, &vertex_source, NULL);
	glCompileShader(vertex_id);
	glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &result);

	if(result != GL_TRUE) {
		glGetShaderInfoLog(vertex_id, 1024, NULL, result_log);
		std::cout << "Shader Compile Error: " << result_log << '\n';
		is_ready = false;
		return;
	}

	// Compile and check fragment shader.
	fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_id, 1, &fragment_source, NULL);
	glCompileShader(fragment_id);
	glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &result);

	if(result != GL_TRUE) {
		glGetShaderInfoLog(fragment_id, 1024, NULL, result_log);
		std::cout << "Shader Compile Error: " << result_log << '\n';
		is_ready = false;
		return;
	}

	// Optionally compile and check geomtery shader.
	if(geometry_source != nullptr) {
		geometry_id = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry_id, 1, &geometry_source, NULL);
		glCompileShader(geometry_id);
		glGetShaderiv(geometry_id, GL_COMPILE_STATUS, &result);

		if(result != GL_TRUE) {
			glGetShaderInfoLog(geometry_id, 1024, NULL, result_log);
			std::cout << "Shader Compile Error: " << result_log << '\n';
			is_ready = false;
			return;
		}
	}

	// Link and check shader program.
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex_id);
	glAttachShader(program_id, fragment_id);
	if(geometry_source != nullptr) {
		glAttachShader(program_id, geometry_id);
	}
	glLinkProgram(program_id);
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);

	if(result != GL_TRUE) {
		glGetProgramInfoLog(program_id, 1024, NULL, result_log);
		std::cout << "Shader Link Error: " << result_log << '\n';
		is_ready = false;
		return;
	}

	// Cleanup unused.
	glDeleteProgram(vertex_id);
	glDeleteProgram(fragment_id);
	if(geometry_source != nullptr) {
		glDeleteProgram(geometry_id);
	}

	is_ready = true;
}

void Shader::Delete() {
	if(is_ready) {
		glDeleteProgram(program_id);
	}
}

void Shader::SetInteger(const char* name, int value, bool use_shader) {

	if(use_shader) {
		Use();
	}

	glUniform1i(glGetUniformLocation(program_id, name), value);
}

void Shader::SetFloat(const char* name, float value, bool use_shader) {

	if(use_shader) {
		Use();
	}

	glUniform1f(glGetUniformLocation(program_id, name), value);
}

void Shader::SetVector2f(const char* name, float x, float y, bool use_shader) {

	if(use_shader) {
		Use();
	}

	glUniform2f(glGetUniformLocation(program_id, name), x, y);
}

void Shader::SetVector3f(const char* name, float x, float y, float z, bool use_shader) {

	if(use_shader) {
		Use();
	}

	glUniform3f(glGetUniformLocation(program_id, name), x, y, z);
}

void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool use_shader) {

	if(use_shader) {
		Use();
	}

	glUniform4f(glGetUniformLocation(program_id, name), x, y, z, w);
}

void Shader::SetVector2f(const char* name, const glm::vec2& value, bool use_shader) {

	if(use_shader) {
		Use();
	}

	glUniform2f(glGetUniformLocation(program_id, name), value.x, value.y);
}

void Shader::SetVector3f(const char* name, const glm::vec3& value, bool use_shader) {

	if(use_shader) {
		Use();
	}

	glUniform3f(glGetUniformLocation(program_id, name), value.x, value.y, value.z);
}

void Shader::SetVector4f(const char* name, const glm::vec4& value, bool use_shader) {

	if(use_shader) {
		Use();
	}

	glUniform4f(glGetUniformLocation(program_id, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4f(const char* name, const glm::mat4& value, bool use_shader) {

	if(use_shader) {
		Use();
	}

	glUniformMatrix4fv(glGetUniformLocation(program_id, name), 1, false, glm::value_ptr(value));
}