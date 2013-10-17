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
	: Camera(renderer), m_direction(1.0f, 0.0f, 0.0f), m_up(0.0f, 0.0f, 1.0f), m_speed(3.0f)
{ }

void FpsCamera::update() {
	setViewMatrix(glm::lookAt(m_position, m_position + m_direction, m_up));
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

void FpsCamera::yaw(float value, float fps) {
	m_direction = glm::mat3(glm::rotate(value / fps, m_up)) * m_direction;
}

void FpsCamera::roll(float value, float fps) {
	m_up = glm::mat3(glm::rotate(value / fps, m_direction)) * m_up;
}

void FpsCamera::pitch(float value, float fps) {
	m_direction = glm::mat3(glm::rotate(value / fps, glm::cross(m_up, m_direction))) * m_direction;
	m_up = glm::mat3(glm::rotate(value / fps, glm::cross(m_up, m_direction))) * m_up;
}
