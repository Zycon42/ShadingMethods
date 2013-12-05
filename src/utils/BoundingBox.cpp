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
