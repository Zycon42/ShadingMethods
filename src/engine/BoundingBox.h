/**
 * @file BoundingBox.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "RenderBatch.h"

#include <glm/glm.hpp>

#include <memory>

/**
 * Axis aligned bounding box.
 * Can be one, two or three dimensional.
 */
class BoundingBox
{
public:
	/// Creates bounding box representing point in (0, 0, 0).
	BoundingBox() { }
	/// Creates bounding box representing point.
	BoundingBox(const glm::vec3& point);
	/// Creates bounding box from min and max points.
	BoundingBox(const glm::vec3& min, const glm::vec3& max);

	BoundingBox(const BoundingBox& other);
	BoundingBox(BoundingBox&& other);
	BoundingBox& operator=(BoundingBox other);

	friend void swap(BoundingBox& first, BoundingBox& second);

	/// Gets minimum point
	const glm::vec3& min() const {
		return m_min;
	}

	/// Gets maximum point
	const glm::vec3& max() const {
		return m_max;
	}

	/// Enlarges bounding box to include given point
	void expandToInclude(const glm::vec3& point);
	/// Enlarges bounding box to include another bounding box
	void expandToInclude(const BoundingBox& bbox);

	/// Gets index of dimension where bbox has maximum size
	int maxDimension() const;

	/// Gets drawable geometry for bounding box note that necessary buffers
	/// are computed on first run
	gl::GeometryBatch* drawableGeometry() const;
private:
	glm::vec3 m_min;
	glm::vec3 m_max;
	glm::vec3 m_extent;

	mutable bool m_sizeChanged;
	mutable std::unique_ptr<gl::GeometryBatch> m_geometry;
};

#endif // BOUNDING_BOX_H
