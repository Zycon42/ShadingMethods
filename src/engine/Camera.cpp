/**
 * @file Camera.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Camera.h"

Camera::Camera(gl::Renderer* renderer) {
	m_buffer = renderer->createUniformBuffer<BufferData>();
}

void Camera::setProjectionMatrix(const glm::mat4& m) {
	m_buffer->data().projection = m;
	computeDerivedData();
	m_buffer->dataChanged();
}

void Camera::setViewMatrix(const glm::mat4& m) {
	m_buffer->data().view = m;
	computeDerivedData();
	m_buffer->dataChanged();
}

void Camera::flushChanges() {
	m_buffer->flushAllData();
}

void Camera::computeDerivedData() {
	auto projection = m_buffer->data().projection;
	auto view = m_buffer->data().view;
	m_buffer->data().viewProjection = projection * view;
}
