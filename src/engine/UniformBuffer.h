/**
 * @file UniformBuffer.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include "Buffer.h"

#include <cassert>

template <class T>
class UniformBuffer
{
public:
	explicit UniformBuffer(GLenum usage = GL_STATIC_DRAW) : m_dataChanged(false) {
		initBuffer(usage);
	}
	explicit UniformBuffer(T initialData, GLenum usage = GL_STATIC_DRAW) 
		: m_data(std::move(initialData)), m_dataChanged(false) {

		initBuffer(usage);
	}

	template<class U>
	void writeUniform(size_t offset, const U& uniform) {
		assert(offset + sizeof(U) < sizeof(T));
		memcpy(reinterpret_cast<char*>(&m_data) + offset, reinterpret_cast<const char*>(&uniform), sizeof(U));
		m_buffer.updateData(offset, sizeof(U), reinterpret_cast<const void*>(&uniform));
	}

	void writeRawData(size_t offset, size_t size, const void* data) {
		assert(offset + size < sizeof(T));
		memcpy(reinterpret_cast<char*>(&m_data) + offset, data, size);
		m_buffer.updateData(offset, size, data);
	}

	void flushAllData() {
		if (m_dataChanged) {
			m_buffer.updateData(0, sizeof(T), reinterpret_cast<const void*>(&m_data));
			m_dataChanged = false;
		}
	}

	gl::IndexedBuffer* internalBuffer() {
		return &m_buffer;
	}

	T& data() {
		return m_data;
	}

	const T& data() const {
		return m_data;
	}

	void dataChanged() {
		m_dataChanged = true;
	}
private:
	void initBuffer(GLenum usage) {
		m_buffer.loadData(reinterpret_cast<const void*>(&m_data), sizeof(T), usage);
	}

	gl::IndexedBuffer m_buffer;
	T m_data;
	bool m_dataChanged;
};

#endif // !UNIFORM_BUFFER_H
