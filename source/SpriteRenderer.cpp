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

// GLAD2
#include <glad/gl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Shader.hpp"

#include "SpriteRenderer.hpp"

SpriteRenderer::SpriteRenderer(Shader& shader) : shader(shader) {
	InitRenderData();
}

SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1, &quad_vao);
	glDeleteBuffers(1, &quad_vbo);
}

void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color) {

	shader.Use();

	// If size is set to zero, draw sprite one to one with actual pixel width and height.
	if(size.x == 0) {
		size.x = texture.GetWidth();
	}

	if(size.y == 0) {
		size.y = texture.GetHeight();
	}

	if(texture.IsLoaded() == false) {
		return;
	}

	if(texture.IsArrayTexture()) {
		return;
	}

	// Prepare transformations.
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	shader.SetMatrix4f("model", model);
	shader.SetVector3f("spriteColor", color);

	texture.Bind();

	// Draw the QuadVAO and then bind nothing.
	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SpriteRenderer::InitRenderData() {

	float vertices[] = {
		// POS      // TEX
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glCreateBuffers(1, &quad_vbo);
	glNamedBufferData(quad_vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glCreateVertexArrays(1, &quad_vao);

	glVertexArrayVertexBuffer(quad_vao, 0, quad_vbo, 0, sizeof(float) * 4);

	glEnableVertexArrayAttrib(quad_vao, 0);

	glVertexArrayAttribFormat(quad_vao, 0, 4, GL_FLOAT, GL_FALSE, 0);

	glVertexArrayAttribBinding(quad_vao, 0, 0);
}