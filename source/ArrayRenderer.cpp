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

#include "ArrayRenderer.hpp"
#include "Shader.hpp"

ArrayRenderer::ArrayRenderer(Shader& shader) : shader(shader) {
	InitRenderData();
}

ArrayRenderer::~ArrayRenderer() {
	glDeleteVertexArrays(1, &QuadVAO);
	glDeleteBuffers(1, &QuadVBO);
}

void ArrayRenderer::DrawArray(Texture2D& texture, std::uint32_t layer, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color) {

	shader.Use();

	// TODO: These values are hardcoded to subimage size.
	if(size.x == 0) {
		size.x = 16;
	}

	if(size.y == 0) {
		size.y = 16;
	}

	if(texture.IsLoaded() == false) {
		return;
	}

	if(texture.IsArrayTexture() == false) {
		return;
	}

	shader.SetInteger("samplerLoc", GL_TEXTURE0 + 1);

	// For now, using texture unit 1 for array textures.
	glActiveTexture(GL_TEXTURE0 + 1);
	texture.Bind();

}

void ArrayRenderer::InitRenderData() {

	float vertices[] = {
		// POS      // TEX
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &QuadVAO);
	glGenBuffers(1, &QuadVBO);

	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(QuadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}