/**
 * @file Light.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef LIGHT_H
#define LIGHT_H

#include "Renderer.h"

class Light
{
public:
	explicit Light(gl::Renderer* renderer);

	const glm::vec4& position() const {
		return m_buffer->data().pos;
	}

	const glm::vec4& ambient() const {
		return m_buffer->data().ambient;
	}

	const glm::vec4& diffuse() const {
		return m_buffer->data().diffuse;
	}

	const glm::vec4& specular() const {
		return m_buffer->data().specular;
	}

	const glm::mat4& viewProjection() const {
		return m_buffer->data().viewProjection;
	}

	void flushChanges();
	void setPosition(const glm::vec4& value);
	void setAmbient(const glm::vec4& value);
	void setDiffuse(const glm::vec4& value);
	void setSpecular(const glm::vec4& value);
	void setViewProjection(const glm::mat4& value);

	gl::IndexedBuffer* uniformBuffer() {
		return m_buffer->internalBuffer();
	}

	bool isShadowSource() {
		return m_isShadowSource;
	}

	void toggleShadowSource(bool value) {
		m_isShadowSource = value;
	}
private:
	struct BufferData
	{
		glm::vec4 pos;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
		glm::mat4 viewProjection;
	};

	std::unique_ptr<UniformBuffer<BufferData>> m_buffer;
	bool m_isShadowSource;
};

#endif // !LIGHT_H
