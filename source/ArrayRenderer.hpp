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

#ifndef __ARRAY_RENDERER_HPP__
#define __ARRAY_RENDERER_HPP__

// STL
#include <cstdint>

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Shader.hpp"
#include "Texture2D.hpp"

class ArrayRenderer {

	public:
		ArrayRenderer(Shader& shader);
		~ArrayRenderer();

		void DrawArray(Texture2D& texture, std::uint32_t layer, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f), float rotation = 0.0f, glm::vec3 color = glm::vec3(1.0f));

	private:
		Shader shader;
		std::uint32_t quad_vao;
		std::uint32_t quad_vbo;

		void InitRenderData();
};

#endif /* __ARRAY_RENDERER_HPP__ */