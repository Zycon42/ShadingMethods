/**
 * @file Interfaces.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef INTERFACES_H
#define INTERFACES_H

#include "BoundingBox.h"

class Mesh;
class IMaterial;

namespace gl {
	class IndexedBuffer;
}

class ISceneObject
{
public:
	virtual ~ISceneObject() { }

	/// Gets node bounding box
	virtual BoundingBox boundingBox() const = 0;

	/// Gets node center point
	virtual glm::vec3 centroid() const = 0;

	virtual Mesh* mesh() = 0;

	virtual IMaterial* material() = 0;

	virtual gl::IndexedBuffer* uniformBuffer() = 0;
};

#endif // INTERFACES_H
