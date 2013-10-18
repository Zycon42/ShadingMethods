/**
 * @file Light.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Light.h"

Light::Light(gl::Renderer* renderer) {
	m_buffer = renderer->createUniformBuffer<BufferData>();
}

void Light::flushChanges() {
	m_buffer->flushData();
}

void Light::setPosition(const glm::vec4& value) {
	m_buffer->data().pos = value;
	m_buffer->dataChanged();
}

void Light::setAmbient(const glm::vec4& value) {
	m_buffer->data().ambient = value;
	m_buffer->dataChanged();
}

void Light::setDiffuse(const glm::vec4& value) {
	m_buffer->data().diffuse = value;
	m_buffer->dataChanged();
}

void Light::setSpecular(const glm::vec4& value) {
	m_buffer->data().specular = value;
	m_buffer->dataChanged();
}
