/**
 * @file Buffer.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <GL/glew.h>

namespace gl {

/**
 * Base of OpenGL buffer objects
 */
class BufferBase
{
public:
	/**
	 * Deletes buffer object.
	 */
	~BufferBase() {
		dispose();
	}

	void dispose() {
		glDeleteBuffers(1, &m_handle);
	}

	/**
	 * Loads data to buffer object.
	 * @param data pointer to data to be copied. May be nullptr in that case data won't be copied
	 * @param size size of data.
	 * @param usage specifies expected usage of data.
	 */
	void loadData(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW) {
		glNamedBufferDataEXT(m_handle, size, data, usage);
	}

	/**
	 * Update data previously loaded with loadData.
	 * @param offset offset into the buffer's data store where data replacement will begin.
	 * @param size size of data.
	 * @param data pointer to new data that will be copied into buffer.
	 */
	void updateData(size_t offset, size_t size, const void* data) {
		glNamedBufferSubDataEXT(m_handle, offset, size, data);
	}
	
	/**
	 * Map buffer data into system memory.
	 * @param access specifies the access policy.
	 * @return pointer to mapped data.
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glMapBuffer.xml
	 */
	void* map(GLenum access) {
		return glMapNamedBufferEXT(m_handle, access);
	}

	/**
	 * Unmaps buffer data. Buffer that is mapped can't be
	 * used in any OpenGL call.
	 */
	void unmap() {
		glUnmapNamedBufferEXT(m_handle);
	}

protected:
	/**
	 * Creates buffer object.
	 * @param target target to which buffer will be bound.
	 */
	BufferBase() {
		glGenBuffers(1, &m_handle);
	}

	GLuint m_handle;
private:
	BufferBase(const BufferBase&);
	BufferBase& operator=(BufferBase);
};

/**
 * Buffer that can bind to target
 */
class Buffer : public BufferBase
{
public:
	/**
	 * Creates buffer object.
	 */
	Buffer() : BufferBase()
	{ }

	/**
	 * Binds this current object to target.
	 */
	void bind(GLenum target = GL_ARRAY_BUFFER) {
		glBindBuffer(target, m_handle);
	}
};

/**
 * Buffer that can bind to target at specified binding point
 */
class IndexedBuffer : public Buffer
{
public:
	/**
	 * Creates indexed buffer object.
	 */
	IndexedBuffer() : Buffer()
	{ }

	/**
	 * Binds buffer to index
	 */
	void bind(GLuint index, GLenum target = GL_UNIFORM_BUFFER) {
		glBindBufferBase(target, index, m_handle);
	}
};

}

#endif // !BUFFER_H
