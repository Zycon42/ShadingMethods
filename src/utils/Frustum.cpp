/**
 * @file Frustum.cpp
 *
 * @author Jan Du�ek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Frustum.h"

Frustum::Frustum(const glm::mat4& vp) {
	m_planes[TOP] = Plane::fromCoeficients(-vp[1] + vp[3]);
	m_planes[BOTTOM] = Plane::fromCoeficients(vp[1] + vp[3]);
	m_planes[LEFT] = Plane::fromCoeficients(vp[0] + vp[3]);
	m_planes[RIGHT] = Plane::fromCoeficients(-vp[0] + vp[3]);
	m_planes[NEAR] = Plane::fromCoeficients(vp[2] + vp[3]);
	m_planes[NEAR] = Plane::fromCoeficients(-vp[2] + vp[3]);
}

glm::vec3 getBBoxPositiveVertex(const BoundingBox& bbox, const glm::vec3& normal) {
	glm::vec3 result = bbox.min();
	if (normal.x >= 0)
		result.x = bbox.max().x;
	if (normal.y >= 0)
		result.y = bbox.max().y;
	if (normal.z >= 0)
		result.z = bbox.max().z;

	return result;
}

glm::vec3 getBBoxNegativeVertex(const BoundingBox& bbox, const glm::vec3& normal) {
	glm::vec3 result = bbox.max();
	if (normal.x >= 0)
		result.x = bbox.min().x;
	if (normal.y >= 0)
		result.y = bbox.min().y;
	if (normal.z >= 0)
		result.z = bbox.min().z;

	return result;
}

Frustum::Intersection Frustum::boundingBoxIntersetion(const BoundingBox& bbox) {
	Intersection result = Intersection::Inside;
	//for each plane do ...
	for(int i = 0; i < 6; i++) {
		// is the positive vertex outside?
		if (m_planes[i].distance(getBBoxPositiveVertex(bbox, m_planes[i].normal())) < 0)
			return Intersection::None;
		// is the negative vertex outside?
		else if (m_planes[i].distance(getBBoxNegativeVertex(bbox, m_planes[i].normal())) < 0)
			result = Intersection::Partialy;
	}
	return result;
}
