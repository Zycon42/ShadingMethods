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

	void flushChanges();
	void setPosition(const glm::vec4& value);
	void setAmbient(const glm::vec4& value);
	void setDiffuse(const glm::vec4& value);
	void setSpecular(const glm::vec4& value);

	gl::IndexedBuffer* uniformBuffer() {
		return m_buffer->internalBuffer();
	}
private:
	struct BufferData
	{
		glm::vec4 pos;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
	};

	std::unique_ptr<UniformBuffer<BufferData>> m_buffer;
};

#endif // !LIGHT_H
