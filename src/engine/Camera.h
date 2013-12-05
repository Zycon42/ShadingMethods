/**
 * @file Camera.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "Renderer.h"
#include "Frustum.h"

/**
 * Base class for all cameras
 */
class Camera
{
public:
	/// Updates camera
	virtual void update() = 0;

	/// Get OpenGL uniform buffer where camera stores matrices
	gl::IndexedBuffer* uniformBuffer() {
		return m_buffer->internalBuffer();
	}

	/// Projection matrix
	const glm::mat4& projectionMatrix() const {
		return m_buffer->data().projection;
	}

	void setProjectionMatrix(const glm::mat4& m);

	const glm::mat4& viewMatrix() const {
		return m_buffer->data().view;
	}

	const Frustum& viewFrustum() const {
		return m_viewFrustum;
	}
protected:
	/**
	 * Camera ctor.
	 * @param renderer 
	 */
	explicit Camera(gl::Renderer* renderer);

	struct BufferData
	{
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 viewProjection;
		glm::vec3 pos;
	};

	void flushChanges();
	void computeDerivedData();

	UniformBuffer<BufferData>* buffer() {
		return m_buffer.get();
	}
private:
	std::unique_ptr<UniformBuffer<BufferData>> m_buffer;
	Frustum m_viewFrustum;
};

#endif // !CAMERA_H

