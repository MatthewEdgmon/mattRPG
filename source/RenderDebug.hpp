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

#ifndef __RENDER_DEBUG_HPP__
#define __RENDER_DEBUG_HPP__

#include <cstdint>

 // GLAD2
#include <glad/gl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class RenderDebug {

	public:
		static void RenderDebugBox(glm::vec2 top_left, glm::vec2 bottom_right, glm::vec3 color);
		static void RenderDebugLine(glm::vec2 start, glm::vec2 end, glm::vec3 color);

	private:
		RenderDebug() { }
};

#endif /* __RENDER_DEBUG_HPP__ */