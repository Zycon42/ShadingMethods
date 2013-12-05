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
	BoundingBoxDrawer(Renderer* renderer);

	void add(const BoundingBox& bbox);
	void clear();
	void draw();
private:
	Renderer* m_renderer;

	std::vector<glm::vec3> m_vertices;
	std::vector<uint32_t> m_indices;

	VertexArrayObject m_vao;
};

}

#endif // BOUNDING_BOX_DRAWER_H
