/**
 * @file FpsCamera.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include "Camera.h"

#include <glm/glm.hpp>

class FpsCamera : public Camera
{
public:
	explicit FpsCamera(gl::Renderer* renderer);

	virtual void update();

	void goForward(float fps);
	void goBackward(float fps);
	void goLeft(float fps);
	void goRight(float fps);

	void yaw(float value);
	void pitch(float value);
	void roll(float value);

	float movementSpeed() const {
		return m_speed;
	}

	void setMovementSpeed(float value) {
		m_speed = value;
	}

	const glm::vec3& position() const {
		return m_position;
	}

	void setPosition(const glm::vec3& value) {
		m_position = value;
	}

	void setPosition(float x, float y, float z) {
		m_position = glm::vec3(x, y, z);
	}

	const glm::vec3& direction() const {
		return m_direction;
	}

	void setDirection(const glm::vec3& value) {
		m_direction = value;
	}

	void setDirection(float x, float y, float z) {
		m_direction = glm::vec3(x, y, z);
	}

	const glm::vec3& up() const {
		return m_up;
	}

	void setUp(const glm::vec3& value) {
		m_up = value;
	}
private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_up;
	float m_speed;
};

#endif // !FPS_CAMERA_H
