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

/**
 * Buffer where uniforms are stored.
 * Contains OpenGL UBO and in memory data
 * @tparam T type that will be stored in UBO, don't forgot to use GLSL std140 alignment
 */
template <class T>
class UniformBuffer
{
public:
	/**
	 * Ctor.
	 * @param usage see Buffer::loadData usage param
	 */
	explicit UniformBuffer(GLenum usage = GL_STATIC_DRAW) : m_dataChanged(false) {
		initBuffer(usage);
	}

	/**
	 * Ctor.
	 * @param initialData 
	 * @param usage see Buffer::loadData usage param
	 */
	explicit UniformBuffer(T initialData, GLenum usage = GL_STATIC_DRAW) 
		: m_data(std::move(initialData)), m_dataChanged(false) {

		initBuffer(usage);
	}

	/**
	 * Write uniform at offset directly to UBO.
	 * @param offset in bytes since UBO beginning where to start writing
	 * @param uniform data to write
	 */
	template<class U>
	void writeUniform(size_t offset, const U& uniform) {
		assert(offset + sizeof(U) < sizeof(T));
		memcpy(reinterpret_cast<char*>(&m_data) + offset, reinterpret_cast<const char*>(&uniform), sizeof(U));
		m_buffer.updateData(offset, sizeof(U), reinterpret_cast<const void*>(&uniform));
	}

	/**
	 * Write data directly to UBO.
	 * @param offset in bytes since UBO beginning where to start writing
	 * @param size size of data in bytes
	 * @param data data to write
	 */
	void writeRawData(size_t offset, size_t size, const void* data) {
		assert(offset + size < sizeof(T));
		memcpy(reinterpret_cast<char*>(&m_data) + offset, data, size);
		m_buffer.updateData(offset, size, data);
	}

	/// Flushes all data in memory to UBO.
	void flushData() {
		if (m_dataChanged) {
			m_buffer.updateData(0, sizeof(T), reinterpret_cast<const void*>(&m_data));
			m_dataChanged = false;
		}
	}

	gl::IndexedBuffer* internalBuffer() {
		return &m_buffer;
	}

	/// In memory data. If you modify it call dataChanged so flushAllData can write it to UBO.
	T& data() {
		return m_data;
	}

	/// In memory data.
	const T& data() const {
		return m_data;
	}

	// Sets data. Call flushData to write it to UBO.
	void setData(const T& data) {
		m_data = data;
		m_dataChanged = true;
	}

	/// Call this if you modified in memory data and you want to update it via flushAllData.
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
