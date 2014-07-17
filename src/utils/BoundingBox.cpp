/**
 * @file BoundingBox.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "BoundingBox.h"

#include <algorithm>

BoundingBox::BoundingBox(const glm::vec3& min, const glm::vec3& max) 
	: m_min(min), m_max(max), m_extent(max - min) { }

BoundingBox::BoundingBox(const glm::vec3& point) 
	: m_min(point), m_max(point), m_extent(m_max - m_min) { }

void BoundingBox::expandToInclude(const glm::vec3& point) {
	m_min = glm::min(m_min, point);
	m_max = glm::max(m_max, point);
	m_extent = m_max - m_min;
}

void BoundingBox::expandToInclude(const BoundingBox& bbox) {
	m_min = glm::min(m_min, bbox.m_min);
	m_max = glm::max(m_max, bbox.m_max);
	m_extent = m_max - m_min;
}

int BoundingBox::maxDimension() const {
	int result = 0;
	if (m_extent.y > m_extent.x)
		result = 1;
	if (m_extent.z > m_extent.y)
		result = 2;
	return result;
}

float BoundingBox::sqrDistance(const glm::vec3& p) const {
	if (this->contains(p))
		return 0.0f;

	// Ericson: Real-Time Collision Detection p. 131
	float dist = 0.0f;
	for (int i = 0; i < 3; ++i) {
		float v = p[i];
		if (v < m_min[i])
			dist += (m_min[i] - v) * (m_min[i] - v);
		if (v > m_max[i])
			dist += (v - m_max[i]) * (v - m_max[i]);
	}
	return dist;
}
