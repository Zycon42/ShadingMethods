/**
 * @file Frustum.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Plane.h"
#include "BoundingBox.h"

class Frustum
{
public:
	enum class Intersection { None, Partialy, Inside };

	Frustum(const glm::mat4& vp);

	Intersection boundingBoxIntersetion(const BoundingBox& bbox);
private:
	enum { TOP, BOTTOM, LEFT, RIGHT, NEAR, FAR };
	Plane m_planes[6];
};

#endif // FRUSTUM_H
