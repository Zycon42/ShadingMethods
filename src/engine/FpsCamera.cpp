/**
 * @file FpsCamera.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "FpsCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

FpsCamera::FpsCamera(gl::Renderer* renderer) 
	: Camera(renderer), m_direction(1.0f, 0.0f, 0.0f), m_up(0.0f, 0.0f, 1.0f), m_speed(1.0f)
{ }

void FpsCamera::update() {
	buffer()->data().view = glm::lookAt(m_position, m_position + m_direction, m_up);
	buffer()->data().pos = m_position;
	computeDerivedData();

	buffer()->dataChanged();
	flushChanges();
}

void FpsCamera::goForward(float fps) {
	m_position += m_direction * m_speed / fps;
}

void FpsCamera::goBackward(float fps) {
	m_position -= m_direction * m_speed / fps;
}

void FpsCamera::goLeft(float fps) {
	m_position += glm::cross(m_up, m_direction) * m_speed / fps;
}

void FpsCamera::goRight(float fps) {
	m_position -= glm::cross(m_up, m_direction) * m_speed / fps;
}

void FpsCamera::yaw(float value) {
	// remove minus sign before value to invert yaw
	m_direction = glm::mat3(glm::rotate(-value, m_up)) * m_direction;
}

void FpsCamera::roll(float value) {
	m_up = glm::mat3(glm::rotate(value, m_direction)) * m_up;
}

void FpsCamera::pitch(float value) {
	glm::vec3 right = glm::cross(m_up, m_direction);
	m_direction = glm::mat3(glm::rotate(value, right)) * m_direction;
	m_up = glm::mat3(glm::rotate(value, right)) * m_up;
}
