/**
 * @file BoundingBox.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Helpers.h"
#include "BoundingBox.h"

#include <algorithm>

BoundingBox::BoundingBox(const glm::vec3& min, const glm::vec3& max) 
	: m_min(min), m_max(max), m_extent(max - min) { }

BoundingBox::BoundingBox(const glm::vec3& point) 
	: m_min(point), m_max(point), m_extent(m_max - m_min) { }

BoundingBox::BoundingBox(const BoundingBox& other)
	: m_min(other.m_min), m_max(other.m_max), m_extent(other.m_extent), m_sizeChanged(true), m_geometry(nullptr) { }

BoundingBox::BoundingBox(BoundingBox&& other) {
	swap(*this, other);
}

BoundingBox& BoundingBox::operator=(BoundingBox other) {
	swap(*this, other);
	return *this;
}

void swap(BoundingBox& first, BoundingBox& second) {
	using std::swap;

	swap(first.m_min, second.m_min);
	swap(first.m_max, second.m_max);
	swap(first.m_extent, second.m_extent);
	swap(first.m_geometry, second.m_geometry);
	swap(first.m_sizeChanged, second.m_sizeChanged);
}

void BoundingBox::expandToInclude(const glm::vec3& point) {
	m_min = glm::min(m_min, point);
	m_max = glm::max(m_max, point);
	m_extent = m_max - m_min;

	m_sizeChanged = true;
}

void BoundingBox::expandToInclude(const BoundingBox& bbox) {
	m_min = glm::min(m_min, bbox.m_min);
	m_max = glm::max(m_max, bbox.m_max);
	m_extent = m_max - m_min;

	m_sizeChanged = true;
}

int BoundingBox::maxDimension() const {
	int result = 0;
	if (m_extent.y > m_extent.x)
		result = 1;
	if (m_extent.z > m_extent.y)
		result = 2;
	return result;
}

gl::GeometryBatch* BoundingBox::drawableGeometry() const {
	if (!m_geometry || m_sizeChanged) {
		m_geometry = std::unique_ptr<gl::GeometryBatch>(new gl::GeometryBatch());

		m_geometry->setPrimitiveType(PrimitiveType::LineList);

		glm::vec3 vertices[] = {
			m_min, glm::vec3(m_max.x, m_min.y, m_min.z), glm::vec3(m_max.x, m_max.y, m_min.z),
			glm::vec3(m_min.x, m_max.y, m_min.z), m_max, glm::vec3(m_max.x, m_min.y, m_max.z),
			glm::vec3(m_min.x, m_min.y, m_max.z), glm::vec3(m_min.x, m_max.y, m_max.z)
		};

		auto vbo = std::make_shared<gl::Buffer>();
		vbo->loadData(vertices, sizeof(vertices));

		auto layout = create_vector<VertexElement>(3);
		m_geometry->setVertices(vbo, sizeof(vertices) / sizeof(*vertices), layout);

		uint16_t indices[] = {
			0, 1, 1, 2, 2, 3, 3, 0, 0, 6, 6, 5, 5, 1, 5, 4, 4, 2, 4, 7, 7, 3, 7, 6
		};
		auto ebo = std::make_shared<gl::Buffer>();
		ebo->loadData(indices, sizeof(indices));

		m_geometry->setIndices(ebo, GL_UNSIGNED_SHORT, sizeof(indices) / sizeof(*indices));

		m_sizeChanged = false;
	}
	return m_geometry.get();
}
