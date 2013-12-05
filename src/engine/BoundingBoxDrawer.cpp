/**
 * @file BoundingBoxDrawer.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "BoundingBoxDrawer.h"

#include <GL/glew.h>

namespace gl {

BoundingBoxDrawer::BoundingBoxDrawer(gl::Renderer* renderer) : m_renderer(renderer) {
}

void BoundingBoxDrawer::add(const BoundingBox& bbox) {
	auto bmin = bbox.min();
	auto bmax = bbox.max();

	glm::vec3 vertices[] = {
		bmin, glm::vec3(bmax.x, bmin.y, bmin.z), glm::vec3(bmax.x, bmax.y, bmin.z),
		glm::vec3(bmin.x, bmax.y, bmin.z), bmax, glm::vec3(bmax.x, bmin.y, bmax.z),
		glm::vec3(bmin.x, bmin.y, bmax.z), glm::vec3(bmin.x, bmax.y, bmax.z)
	};

	size_t indicesBase = m_vertices.size();
	m_vertices.insert(m_vertices.end(), vertices, vertices + 8);

	uint32_t indices[] = {
		0, 1, 1, 2, 2, 3, 3, 0, 0, 6, 6, 5, 5, 1, 5, 4, 4, 2, 4, 7, 7, 3, 7, 6
	};

	size_t numIndices = sizeof(indices) / sizeof(*indices);
	m_indices.reserve(m_indices.size() + numIndices);
	for (size_t i = 0; i < numIndices; ++i) {
		m_indices.push_back(indices[i] + indicesBase);
	}
}

void BoundingBoxDrawer::clear() {
	m_vertices.clear();
	m_indices.clear();
}

void BoundingBoxDrawer::draw() {
	auto shader = m_renderer->shaderManager()->getGlslProgram("simple");
	shader->use();

	m_vao.bind();

	Buffer vbo;
	vbo.loadData(m_vertices.data(), m_vertices.size() * sizeof(glm::vec3));

	vbo.bind(GL_ARRAY_BUFFER);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<const void*>(0));

	Buffer ebo;
	ebo.bind(GL_ELEMENT_ARRAY_BUFFER);
	ebo.loadData(m_indices.data(), m_indices.size() * sizeof(uint32_t));

	glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, 0);

	VertexArrayObject::unbind();
}

}
