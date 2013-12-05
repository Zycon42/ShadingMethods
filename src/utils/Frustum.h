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

	/**
	 * Creates frustum from view projection matrix
	 * @see http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf
	 */
	Frustum(const glm::mat4& vp);

	Intersection boundingBoxIntersetion(const BoundingBox& bbox) const;
private:
	Plane m_planes[6];
};

#endif // FRUSTUM_H
