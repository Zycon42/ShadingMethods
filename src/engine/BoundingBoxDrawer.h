/**
 * @file BoundingBoxDrawer.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef BOUNDING_BOX_DRAWER_H
#define BOUNDING_BOX_DRAWER_H

#include "Renderer.h"
#include "BoundingBox.h"

#include <vector>
#include <cstdint>

namespace gl {

class BoundingBoxDrawer
{
public:
	BoundingBoxDrawer(std::shared_ptr<ShaderProgram> shader, Renderer::State* state);

	void add(const BoundingBox& bbox);
	void clear();
	void draw();

	void drawSingle(const BoundingBox& bbox);
private:
	std::shared_ptr<ShaderProgram> m_shader;
	Renderer::State* m_state;

	std::vector<glm::vec3> m_vertices;
	std::vector<uint32_t> m_indices;

	VertexArrayObject m_vao;

	static const int BOX_VERTICES = 8;
	static const int BOX_INDICES = 14;

	Buffer m_ebo;
	Buffer m_vbo;
};

}

#endif // BOUNDING_BOX_DRAWER_H
