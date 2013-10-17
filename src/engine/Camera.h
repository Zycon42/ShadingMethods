/**
 * @file Camera.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "Renderer.h"

#include <glm/glm.hpp>

/**
 * Base class for all cameras
 */
class Camera
{
public:
	/**
	 * Camera ctor.
	 * @param renderer 
	 */
	explicit Camera(gl::Renderer* renderer);

	/// Updates camera
	virtual void update();

	/// Get OpenGL uniform buffer where camera stores matrices
	gl::IndexedBuffer* uniformBuffer() {
		return m_buffer->internalBuffer();
	}

	/// View matrix
	const glm::mat4& viewMatrix() const {
		return m_buffer->data().view;
	}

	/// Projection matrix
	const glm::mat4& projectionMatrix() const {
		return m_buffer->data().projection;
	}

	void setProjectionMatrix(const glm::mat4& m);
	void setViewMatrix(const glm::mat4& m);
protected:
	void flushChanges();
private:
	struct BufferData
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 viewProjection;
	};

	void computeDerivedData();

	std::unique_ptr<UniformBuffer<BufferData>> m_buffer;
};

#endif // !CAMERA_H

