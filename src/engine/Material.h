/**
 * @file Material.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include "ShaderProgram.h"

#include <memory>

/**
 * Material interface.
 */
class IMaterial
{
public:
	~IMaterial() { }

	/// Shader which is used to draw material
	virtual gl::ShaderProgram* shader() = 0;

	/// OpenGL UBO containing material properties 
	virtual gl::IndexedBuffer* uniformBuffer() = 0;
};

/**
 * Simple material which has no properties.
 */
class SimpleMaterial : public IMaterial
{
public:
	virtual gl::IndexedBuffer* uniformBuffer() {
		return nullptr;
	}

	void setShader(std::shared_ptr<gl::ShaderProgram> shader) {
		m_shader = std::move(shader);
	}

	virtual gl::ShaderProgram* shader() {
		return m_shader.get();
	}
private:
	std::shared_ptr<gl::ShaderProgram> m_shader;
};

/**
 * Base class for all materials with properties.
 * Derive this class and as T use struct which describes
 * desired properties don't forget to use glsl std140 alignment.
 */
template <class T>
class MaterialBase : public IMaterial
{
public:
	explicit MaterialBase(gl::Renderer* renderer) {
		m_buffer = renderer->createUniformBuffer<T>();
	}

	void setShader(std::shared_ptr<gl::ShaderProgram> shader) {
		m_shader = std::move(shader);
	}

	virtual gl::ShaderProgram* shader() {
		return m_shader.get();
	}

	virtual gl::IndexedBuffer* uniformBuffer() {
		return m_buffer->internalBuffer();
	}

	UniformBuffer<T>* properties() {
		return m_buffer.get();
	}
private:
	std::shared_ptr<gl::ShaderProgram> m_shader;
	std::unique_ptr<UniformBuffer<T>> m_buffer;
};

#endif // !MATERIAL_H
