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

#ifndef __SPRITE_RENDERER_HPP__
#define __SPRITE_RENDERER_HPP__

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Shader.hpp"
#include "Texture2D.hpp"

class SpriteRenderer {

	public:
		SpriteRenderer(Shader& shader);
		~SpriteRenderer();

		void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f), float rotation = 0.0f, glm::vec3 color = glm::vec3(1.0f));

	private:
		Shader shader;
		unsigned int QuadVAO;
		unsigned int QuadVBO;

		void InitRenderData();
};

#endif /* __SPRITE_RENDERER_HPP__ */